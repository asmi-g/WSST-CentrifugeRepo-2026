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

## Pre-Push Checklist

Before pushing or opening a merge request, confirm:

1. I am pushing to a **feature branch**, not `develop` or `main`
2. My branch name follows the naming convention
3. My commit messages follow the required format
4. My feature branch is rebased onto the latest `develop`
5. `develop` is rebased onto `main` before merging into `main`
6. User-facing changes include updated documentation
7. Repository structure remains clean and organized


### MAIN IDEA:
* Master/Main branch:
    * Main, FUNCTIONAL source code. I.e, code in main is assumed to be in a working state at ALL times
* Develop branch:
    * This is the branch where the source code of HEAD always reflects a state with the latest delivered development changes for the next release. When the source code in the develop branch reaches a stable point and is ready to be released, all of the changes should be merged back into master. 
* Feature branches:
    * Branch OFF of develop
    * When functional, merge back INTO develop

Thus, at a high level, the development workflow looks like:
1. Start feature: main > develop > feature 
    * Implement changes into a copy of the "develop" branch's source code, which is a copy of the "main" branch's source code
2. Merge feature: main < develop < feature 
    * When done, merge changes into the "develop" branch's source code, then into "main"

### Branch Naming
Make sure your feature branches follow the following convention:
"stage-#-issue#-feature-brief"
ex. 
```
stage-2-issue-4-high-level-diagram
```

### Commit Messages

Make sure your commit messages follow the following template:
"branch_name": "overview of commit functionality"
ex. 
```
stage-2-issue-4-high-level-diagram: Added best practices documentation
```

**ALWAYS REMEMBER TO MAKE A MERGE REQUEST INTO BOTH, THE DEVELOP AND MAIN BRANCH**
Merge Request into Main: NEEDS to be reviewed by a different developer
Merge Request into Develop: If it works functionally, can be rebased and merged into develop on your own

### Checklist Before Pushing
Ask yourself:
1. Am I pushing my code to a FEATURE branch, and not directly to develop or main?
2. Does my commit message follow the established convention?
3. If my feature works, have I rebased it with the develop branch before making a merge request into develop?
4. If my develop branch works, have I rebased it with main before making a merge request into main?
5. If I've implemented any code which has changed how a user might use it, have I added the adequate documentation to do so?
6. Is the repo structure more or less the same (i.e, clean) after my changes?