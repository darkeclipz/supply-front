#include "sim/GameEntityId.hpp"
#include "engine/Assets.hpp"
#include "engine/Scene.hpp"
#include "app/GameSession.hpp"
#include "app/Selection.hpp"

#include <raylib.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#if SEED_WITH_EDITOR
#include <imgui.h>
#include <rlImGui.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <chrono>
#include <limits>

namespace {
namespace fs = std::filesystem;

void logRaylib(int logLevel, const char* text, va_list args) {
    spdlog::level::level_enum level;
    switch (logLevel) {
        case LOG_TRACE: level = spdlog::level::trace; break;
        case LOG_DEBUG: level = spdlog::level::debug; break;
        case LOG_INFO: level = spdlog::level::info; break;
        case LOG_WARNING: level = spdlog::level::warn; break;
        case LOG_ERROR: level = spdlog::level::err; break;
        case LOG_FATAL: level = spdlog::level::critical; break;
        default: return;
    }

    // Raylib uses printf formatting; render it before passing it to spdlog.
    va_list copy;
    va_copy(copy, args);
    const int length = std::vsnprintf(nullptr, 0, text, copy);
    va_end(copy);
    if (length >= 0) {
        std::string message(static_cast<std::size_t>(length) + 1, '\0');
        va_copy(copy, args);
        std::vsnprintf(message.data(), message.size(), text, copy);
        va_end(copy);
        message.resize(static_cast<std::size_t>(length));
        // The console sink supplies the newline.
        while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
            message.pop_back();
        }
        spdlog::log(level, "[raylib] {}", message);
    } else {
        spdlog::error("[raylib] Failed to format log message");
    }

    // Raylib returns immediately after invoking a callback, even for LOG_FATAL.
    if (logLevel == LOG_FATAL) {
        spdlog::default_logger()->flush();
        std::exit(EXIT_FAILURE);
    }
}

struct Options {
    fs::path assets = fs::path(GetApplicationDirectory()) / "assets";
    fs::path scene;
    fs::path save = fs::current_path() / "scene.saved.json";
    bool smokeTest = false;
};

Options parseOptions(int argc, char** argv) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        const std::string argument(argv[i]);
        if (argument == "--smoke-test") {
            options.smokeTest = true;
        } else if (argument == "--assets" || argument == "--scene" || argument == "--save") {
            if (++i >= argc) throw std::runtime_error("Missing value for " + argument);
            if (argument == "--assets") options.assets = argv[i];
            else if (argument == "--scene") options.scene = argv[i];
            else options.save = argv[i];
        } else {
            throw std::runtime_error("Unknown option: " + argument);
        }
    }
    options.assets = fs::absolute(options.assets);
    if (options.scene.empty()) options.scene = options.assets / "scenes/demo.json";
    return options;
}

class Window final {
public:
    Window() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
        InitWindow(1280, 800, "C++20 engine seed");
        if (!IsWindowReady()) throw std::runtime_error("Window initialization failed");
        SetWindowMinSize(720, 480);
        // Handle Escape ourselves, so UI keyboard capture can prevent it from closing.
        SetExitKey(KEY_NULL);
        SetTargetFPS(144);
    }
    ~Window() { CloseWindow(); }
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

struct OrbitCamera {
    Camera3D native{{6.0f, 4.0f, 6.0f}, {0.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE};
    float yaw = 0.75f;
    float pitch = 0.9f;
    float distance = 32.0f;

    void update(bool mouse_captured) {
        if (!mouse_captured) {
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                const auto delta = GetMouseDelta();
                yaw -= delta.x * 0.006f;
                pitch = std::clamp(pitch + delta.y * 0.006f, -1.35f, 1.35f);
            }
            distance = std::clamp(distance * std::exp(-GetMouseWheelMove() * 0.12f),
                                  1.0f, 100.0f);
        }
        const float horizontal = distance * std::cos(pitch);
        native.position = {
            native.target.x + horizontal * std::sin(yaw),
            native.target.y + distance * std::sin(pitch),
            native.target.z + horizontal * std::cos(yaw)
        };
    }
};

#if SEED_WITH_EDITOR
class Editor final {
public:
    Editor() {
        rlImGuiSetup(true);
        ImGui::GetIO().IniFilename = nullptr;
    }
    ~Editor() { rlImGuiShutdown(); }
    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    void draw(entt::registry& world, engine::AssetCache& assets,
              const Options& options, bool& playing, int& simulation_speed) {
        ImGui::SetNextWindowPos(ImVec2{16.0f, 16.0f}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2{330.0f, 600.0f}, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Scene inspector")) {
            ImGui::Text("%d FPS | %zu cached models", GetFPS(), assets.size());
            ImGui::Checkbox("Play fixed-step simulation", &playing);
            ImGui::RadioButton("1x", &simulation_speed, 1);
            ImGui::SameLine();
            ImGui::RadioButton("2x", &simulation_speed, 2);
            ImGui::SameLine();
            ImGui::RadioButton("4x", &simulation_speed, 4);
            ImGui::SameLine();
            ImGui::RadioButton("8x", &simulation_speed, 8);
            ImGui::TextWrapped("Right-drag outside this panel to orbit. Scroll to zoom.");
            ImGui::Separator();
            if (ImGui::Button("Save snapshot")) {
                try {
                    engine::saveScene(world, options.save);
                    status_ = "Saved: " + options.save.string();
                    spdlog::info("[engine] Saved scene to {}", options.save.string());
                } catch (const std::exception& error) {
                    status_ = error.what();
                    spdlog::error("[engine] Failed to save scene to {}: {}", options.save.string(), error.what());
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Load snapshot")) reload(world, assets, options.save);
            if (ImGui::Button("Reload startup scene")) reload(world, assets, options.scene);
            if (!status_.empty()) ImGui::TextWrapped("%s", status_.c_str());
            ImGui::SeparatorText("Entities");
            for (const auto [entity, identity, name] :
                 world.view<const engine::Identity, const engine::Name>().each()) {
                ImGui::PushID(identity.value.c_str());
                if (ImGui::Selectable(name.value.c_str(), entity == selected_)) selected_ = entity;
                ImGui::PopID();
            }
            if (world.valid(selected_) && world.all_of<engine::Transform3D>(selected_)) {
                ImGui::SeparatorText("Transform");
                auto& transform = world.get<engine::Transform3D>(selected_);
                ImGui::DragFloat3("Position", transform.position.data(), 0.02f);
                ImGui::DragFloat3("Rotation (deg)", transform.rotationDegrees.data(), 0.5f);
                ImGui::DragFloat3("Scale", transform.scale.data(), 0.01f, 0.01f, 20.0f,
                                 "%.2f", ImGuiSliderFlags_AlwaysClamp);
                if (auto* spin = world.try_get<engine::Spin>(selected_)) {
                    ImGui::DragFloat("Spin (deg/s)", &spin->degreesPerSecond, 1.0f,
                                     -360.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
                }
                if (const auto* model = world.try_get<engine::ModelRef>(selected_)) {
                    ImGui::TextWrapped("Model: %s", model->path.c_str());
                }
            } else {
                ImGui::TextWrapped("Select an entity to edit its transform.");
            }
        }
        ImGui::End();
    }

private:
    void reload(entt::registry& world, engine::AssetCache& assets, const fs::path& path) {
        try {
            auto replacement = engine::loadScene(path);
            assets.preload(replacement);
            world = std::move(replacement);
            selected_ = entt::null;
            status_ = "Loaded: " + path.string();
            spdlog::info("[engine] Reloaded scene from {}", path.string());
        } catch (const std::exception& error) {
            status_ = error.what();
            spdlog::error("[engine] Failed to reload scene from {}: {}", path.string(), error.what());
        }
    }
    entt::entity selected_ = entt::null;
    std::string status_;
};
#endif

struct SelectionDrag {
    bool active = false;
    bool dragging = false;
    bool completed = false;
    bool additive = false;
    Vector2 start{};
    Vector2 current{};

    Rectangle rectangle() const {
        return Rectangle{
            std::min(start.x, current.x),
            std::min(start.y, current.y),
            std::abs(current.x - start.x),
            std::abs(current.y - start.y)
        };
    }

    void update(bool mouse_captured, bool keyboard_captured) {
        completed = false;

        if (mouse_captured) {
            active = false;
            dragging = false;
            return;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            active = true;
            dragging = false;
            start = GetMousePosition();
            current = start;
            additive = !keyboard_captured
                    && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
        }

        if (!active) return;

        current = GetMousePosition();
        const float dx = current.x - start.x;
        const float dy = current.y - start.y;

        if (dx * dx + dy * dy >= 4.0f * 4.0f) {
            dragging = true;
        }

        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            completed = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
            active = false;
        }
    }

    void draw() const {
        if (!active || !dragging) return;

        const Rectangle bounds = rectangle();
        DrawRectangleRec(bounds, Fade(YELLOW, 0.12f));
        DrawRectangleLinesEx(bounds, 1.0f, YELLOW);
    }
};

void apply_box_selection(
    const app::GameSession& session,
    app::Selection& selection,
    const Camera3D& camera,
    const SelectionDrag& drag)
{
    if (!drag.additive) {
        selection.clear();
    }

    const Rectangle bounds = drag.rectangle();
    const Vector3 forward{
        camera.target.x - camera.position.x,
        camera.target.y - camera.position.y,
        camera.target.z - camera.position.z
    };

    for (const auto id : session.prototype_entities()) {
        const auto position = session.position(id);
        if (!position) continue;

        const Vector3 center {
            static_cast<float>(position->x) / 1000.0f,
            0.5f,
            static_cast<float>(position->z) / 1000.0f
        };
        const Vector3 offset {
            center.x - camera.position.x,
            center.y - camera.position.y,
            center.z - camera.position.z
        };
        const float depth =
            offset.x * forward.x +
            offset.y * forward.y +
            offset.z * forward.z;

        if (depth <= 0.0f) continue;

        const Vector2 screen = GetWorldToScreen(center, camera);
        if (CheckCollisionPointRec(screen, bounds)) {
            selection.add(id);
        }
    }
}

void update_and_draw_prototypes(
    const app::GameSession& session,
    app::Selection& selection,
    const Camera3D& camera,
    const SelectionDrag& drag)
{
    if (drag.completed && drag.dragging) {
        apply_box_selection(session, selection, camera, drag);
    }
    else if(drag.completed) {
        const Ray ray = GetScreenToWorldRay(GetMousePosition(), camera);
        sim::GameEntityId closest{};
        float closest_distance = std::numeric_limits<float>::infinity();

        for (const auto id : session.prototype_entities()) {
            const auto position = session.position(id);
            if (!position) continue;

            const float x = static_cast<float>(position->x) / 1000.0f;
            const float z = static_cast<float>(position->z) / 1000.0f;
            const BoundingBox bounds {
                Vector3{x - 0.4f, 0.0f, z - 0.4f},
                Vector3{x + 0.4f, 1.0f, z + 0.4f}
            };
            const RayCollision hit = GetRayCollisionBox(ray, bounds);

            if (hit.hit && (hit.distance < closest_distance || (hit.distance == closest_distance && id.value < closest.value))) {
                closest = id;
                closest_distance = hit.distance;
            }
        }

        if (drag.additive) {
            selection.add(closest);
        }
        else {
            selection.select(closest);
        }
    }

    for (const auto id : session.prototype_entities()) {
        const auto position = session.position(id);
        if (!position) continue;

        const Vector3 center {
            static_cast<float>(position->x) / 1000.0f,
            0.5f,
            static_cast<float>(position->z) / 1000.0f
        };

        DrawCube(center, 0.8f, 1.0f, 0.8f, BLUE);

        if (selection.contains(id)) {
            DrawCubeWires(center, 0.84f, 1.04f, 0.84f, YELLOW);
        }
    }
}

int run(const Options& options) {
    spdlog::info("[engine] Starting sandbox; asset root: {}", options.assets.string());
    auto world = engine::loadScene(options.scene);
    spdlog::info("[engine] Loaded scene from {}", options.scene.string());
    Window window; // Must outlive every GPU resource and the ImGui backend.
    engine::AssetCache assets(options.assets);
    assets.preload(world);
    spdlog::info("[engine] Preloaded {} model(s)", assets.size());
    OrbitCamera camera;
#if SEED_WITH_EDITOR
    Editor editor;
#endif
    bool playing = true;
    int simulation_speed = 1;
    bool running = true;
    int frames = 0;
    constexpr double step = 1.0 / 60.0;
    double accumulator = 0.0;
    app::GameSession session;
    app::Selection selection;
    SelectionDrag selection_drag;

    while (running && !WindowShouldClose()) {
        const double frame_elapsed = std::max(static_cast<double>(GetFrameTime()), 0.0);
        const double elapsed = std::clamp(frame_elapsed, 0.0, 0.25);
        const auto simulation_elapsed = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::duration<double>{frame_elapsed});
        BeginDrawing();
        ClearBackground(Color{28, 31, 40, 255});
        bool mouse_captured = false;
        bool keyboard_captured = false;
#if SEED_WITH_EDITOR
        // Backend gathers input and begins an ImGui frame. Its draw data is rendered last.
        rlImGuiBegin();
        editor.draw(world, assets, options, playing, simulation_speed);
        mouse_captured = ImGui::GetIO().WantCaptureMouse;
        keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;
#endif
        if (!keyboard_captured && IsKeyPressed(KEY_ESCAPE)) running = false;
        camera.update(mouse_captured);
        selection_drag.update(mouse_captured, keyboard_captured);
        RayCollision ground_hit{};
        if (!mouse_captured) {
            const Ray ray = GetScreenToWorldRay(GetMousePosition(), camera.native);
            ground_hit = GetRayCollisionQuad(
                ray,
                Vector3{-10.0f, 0.0f, -10.0f},
                Vector3{-10.0f, 0.0f, 10.0f},
                Vector3{10.0f, 0.0f, 10.0f},
                Vector3{10.0f, 0.0f, -10.0f});
        }
        session.advance(simulation_elapsed, !playing, simulation_speed);
        if (playing) {
            accumulator += elapsed;
            int steps = 0;
            while (accumulator >= step && steps < 8) {
                engine::fixedUpdate(world, static_cast<float>(step));
                accumulator -= step;
                ++steps;
            }
            // Favor responsiveness after a long stall instead of an unbounded catch-up loop.
            if (accumulator >= step) accumulator = std::fmod(accumulator, step);
        } else {
            accumulator = 0.0;
        }
        BeginMode3D(camera.native);
        DrawPlane(
            Vector3{0.0f, 0.0f, 0.0f},
            Vector2{20.0f, 20.0f},
            Color{74, 83, 67, 255});
        DrawLine3D(
            Vector3{-10.0f, 0.01f, 0.0f},
            Vector3{10.0f, 0.01f, 0.0f},
            RED);
        DrawLine3D(
            Vector3{0.0f, 0.01f, -10.0f},
            Vector3{0.0f, 0.01f, 10.0f},
            BLUE);
        engine::drawScene(world, assets);
        update_and_draw_prototypes(session, selection, camera.native, selection_drag);
        if (ground_hit.hit) {
            DrawSphere(
                Vector3{ground_hit.point.x, 0.12f, ground_hit.point.z},
                0.12f,
                YELLOW);
        }
        EndMode3D();
        selection_drag.draw();
        const auto tick_label = std::string{"Simulation tick: "}
            + std::to_string(session.current_tick());
        DrawText(tick_label.c_str(), 16, GetScreenHeight() - 55, 18, RAYWHITE);
#if SEED_WITH_EDITOR
        rlImGuiEnd();
#else
        DrawFPS(16, 16);
#endif
        EndDrawing();
        if (options.smokeTest && ++frames >= 3) {
            // A manual-frame-control raylib build can open a window without ever
            // presenting frames or updating timing. Do not report that as success.
            if (!(GetFrameTime() > 0.0f)) {
                throw std::runtime_error("Frame timing did not advance; disable raylib custom frame control");
            }
            running = false;
            spdlog::info("[engine] Smoke test passed ({} frames)", frames);
        }
    }
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    try {
        spdlog::set_default_logger(spdlog::stdout_color_mt("sandbox"));
        spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
        spdlog::set_level(spdlog::level::info);
        SetTraceLogCallback(logRaylib);
        // Let spdlog control filtering for both application and raylib messages.
        SetTraceLogLevel(LOG_ALL);
        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--help") {
                std::cout << "sandbox [--assets DIR] [--scene FILE] [--save FILE] [--smoke-test]\n";
                return 0;
            }
        }
        const int result = run(parseOptions(argc, argv));
        spdlog::info("[engine] Sandbox shut down");
        return result;
    } catch (const std::exception& error) {
        spdlog::critical("[engine] Fatal: {}", error.what());
        return 1;
    }
}
