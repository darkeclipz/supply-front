cmake_minimum_required(VERSION 3.25)
project(CppGameTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SEED_BUILD_SANDBOX "Build the desktop 3D demo" ON)
option(SEED_WITH_EDITOR "Include the ImGui debug inspector" ON)
option(SEED_BUILD_TESTS "Build non-graphical scene tests" ON)

include(cmake/Dependencies.cmake)

function(seed_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# This library has no graphics/window dependency and can be tested headlessly.
add_library(engine_scene STATIC src/engine/Scene.cpp)
target_include_directories(engine_scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine_scene PUBLIC EnTT::EnTT nlohmann_json::nlohmann_json)
target_compile_features(engine_scene PUBLIC cxx_std_20)
seed_warnings(engine_scene)

add_library(supply_sim STATIC 
    src/sim/Simulation.cpp
    src/sim/SimulationScheduler.cpp
)
target_include_directories(supply_sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(supply_sim PUBLIC EnTT::EnTT)
target_compile_features(supply_sim PUBLIC cxx_std_20)
seed_warnings(supply_sim)

add_library(supply_session STATIC
    src/app/GameSession.cpp
    src/app/Selection.cpp
)
target_link_libraries(supply_session PUBLIC supply_sim)
seed_warnings(supply_session)

if(SEED_BUILD_SANDBOX)
    add_library(engine_render STATIC src/engine/Assets.cpp)
    target_link_libraries(engine_render PUBLIC engine_scene raylib)
    seed_warnings(engine_render)

    add_executable(sandbox src/main.cpp)
    target_link_libraries(sandbox 
        PRIVATE engine_render supply_session spdlog::spdlog)
    target_compile_definitions(sandbox PRIVATE SEED_WITH_EDITOR=$<BOOL:${SEED_WITH_EDITOR}>)
    if(SEED_WITH_EDITOR)
        target_link_libraries(sandbox PRIVATE seed_rlimgui)
    endif()
    set_target_properties(sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    seed_warnings(sandbox)

    # Always stage assets, even when only an asset changed and the executable did not relink.
    add_custom_target(seed_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:sandbox>/assets"
        VERBATIM
    )
    add_dependencies(sandbox seed_assets)
endif()

if(SEED_BUILD_TESTS)
    enable_testing()
    add_executable(scene_tests tests/scene_tests.cpp)
    target_link_libraries(scene_tests PRIVATE engine_scene Catch2::Catch2WithMain)
    seed_warnings(scene_tests)
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(Catch)
    catch_discover_tests(scene_tests)
    add_executable(simulation_tests tests/simulation_tests.cpp)
    target_link_libraries(simulation_tests
        PRIVATE supply_sim Catch2::Catch2WithMain)
    seed_warnings(simulation_tests)
    catch_discover_tests(simulation_tests)
    add_executable(selection_tests tests/selection_tests.cpp)
    target_link_libraries(selection_tests
        PRIVATE supply_session Catch2::Catch2WithMain)
    seed_warnings(selection_tests)
    catch_discover_tests(selection_tests)
endif()
