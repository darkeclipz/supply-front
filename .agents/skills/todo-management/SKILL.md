---
name: todo-management
description: Manage Supply Front todos in todo/open and todo/closed. Use when creating, listing, updating, closing, or reopening tasks, including progress updates made by the tutor.
---

# Todo management

Use repository-root paths. Each todo is one Markdown file in `todo/open/`
or `todo/closed/`; its folder determines whether it is open or closed.
Keep the system to these folders and individual todos. Do not create an
overview, index, separate progress log, or todo metadata.

## Format

A todo contains only a title, description, and acceptance criteria:

```markdown
# Add a fixed simulation tick

## Description

Separate simulation updates from rendering so gameplay advances at a fixed rate.

## Acceptance criteria

- [ ] Simulation advances at the configured fixed tick rate.
- [ ] Rendering frame rate does not change the resulting simulation state.
- [ ] A headless test verifies the behavior.
```

Use concrete, observable acceptance criteria. Check a criterion only when
its outcome has been verified; recording a plan does not complete it.
Keep progress notes, decisions, verification evidence, and the next action
inside the description when needed. Preserve these notes when editing a todo.
Do not add other top-level sections or frontmatter.

## Operations

- **Create:** Inspect both folders for an existing matching task. Reuse it
  where appropriate; otherwise write `todo/open/<short-kebab-case-title>.md`.
  Use a distinct filename if it would collide in either folder.
- **List:** Read todos in the requested folder and report their titles and
  paths briefly. Read their contents before reporting progress.
- **Update:** Edit the existing file, preserving its scope and filename.
  Do not weaken acceptance criteria merely to call a task complete.
- **Close:** Verify every criterion, record brief evidence in the description,
  then move the same file to `todo/closed/`. If evidence is missing, keep it
  open and state what remains. Never overwrite a different closed todo.
- **Reopen:** Move the same file back to `todo/open/`, uncheck criteria that
  no longer hold, and explain the remaining work in the description.
  Never overwrite a different open todo.

Create missing folders when needed. Empty folders may contain `.gitkeep`;
ignore that file when listing todos. Do not delete todos as a substitute for
closing them.
