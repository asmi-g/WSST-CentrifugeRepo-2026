# Git Best Practices
Read through Git Best Practices, [linked here](https://nvie.com/posts/a-successful-git-branching-model/#the-main-branches).

## Overview

This repository follows a **Git Flow–style workflow** to ensure stable releases and clean development history.

### Branch Roles

- **`main`**
  - Always **stable and production-ready**
  - Must compile, run, and pass tests at all times
  - All merges require review from another developer

- **`develop`**
  - Integration branch for upcoming releases
  - Contains the latest completed features
  - Must remain functionally stable

- **Feature branches**
  - Used for all new development
  - Always branch **off `develop`**
  - Merged back **into `develop`**, then into `main`

---

## Workflow
```
main
└── develop
└── feature/*
```

### Step-by-Step

1. **Start a feature**
```main → develop → feature```
    - Create a feature branch from `develop`
    - Implement and commit changes

2. **Finish a feature**
```feature → develop → main```
    - Rebase feature onto latest `develop`
    - Merge feature into `develop`
    - Rebase `develop` onto `main`
    - Merge `develop` into `main`

---

## Branch Naming Convention

All feature branches **must** follow this format:
stage-<stage#>-issue-<issue#>-<short-description>

**Example**
```stage-2-issue-4-high-level-diagram```

---

## Commit Message Convention

Use the following template:
<branch_name>: <brief description>

**Example**
```stage-2-issue-4-high-level-diagram: Added best practices documentation```

---

## Merge Requests (Required)

> **Never push directly to `develop` or `main`.**

### Merge into `develop`

- Rebase your feature branch onto `develop`
- Self-review is acceptable if functionality is verified

### Merge into `main`

- Must be reviewed by **another developer**
- `develop` must be rebased onto `main` first

---


## Linking & Closing Git Issues

All feature work should be associated with a Git issue whenever possible.

### Referencing Issues in Branches (Optional but Recommended)

Include the issue number in the branch name when applicable:

```stage-2-issue-4-high-level-diagram```

When opening a merge request (PR), reference the related issue using one of the following keywords in the **description**:

- `Closes #<issue-number>`
- `Fixes #<issue-number>`
- `Resolves #<issue-number>`

**Example**
```Closes #4```
When the merge request is merged into `main`, the referenced issue will be **automatically closed**.

---
## Pre-Push Checklist

Before pushing or opening a merge request, confirm:

1. I am pushing to a **feature branch**, not `develop` or `main`
2. My branch name follows the naming convention
3. My commit messages follow the required format
4. My feature branch is rebased onto the latest `develop`
5. `develop` is rebased onto `main` before merging into `main`
6. User-facing changes include updated documentation
7. Repository structure remains clean and organized


