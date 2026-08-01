# AGENTS.md — learning project context

This is a learning project. The primary goals are:

1. **Learn CMake** — write idiomatic `CMakeLists.txt` from scratch, handle external dependencies (Boost), configure install targets, and produce a clean build system.

2. **Learn GitHub Actions** — set up CI workflows that build the project across platforms, run smoke tests, and possibly lint/format check.

3. **Practice modern C++** — use C++23 features (`std::filesystem`, `std::format`) alongside real Boost libraries.

4. **clang-tidy discipline** — prefer fixing issues in the code over adding blanket exclusions to `.clang-tidy`. When a rule legitimately cannot be followed (e.g. platform compatibility), use `// NOLINTNEXTLINE(...)` or `// NOLINTBEGIN/END(...)` comments at the specific site, with a comment explaining why.

Remember to explain any actions you do related to CMake and GitHub Actions to the user and teach them to them. Specifically when using working on complex aspects of CMake and GitHub Actions and introducing new CMake and GitHub Actions features.

The code itself (`src/timestamp.cpp`) is a deliberately small, single-file tool so that build-system and CI concerns are the focus, not application complexity.

## Git workflow

- **Branches**: each task gets a `feat/`, `fix/`, or `chore/` branch (e.g. `feat/static-boost-linkage`).
- **No direct pushes to `main`** — enforced by branch protection: pull requests are required, `main` needs 1 approving review, and `enforce_admins` is on, so even the repo owner cannot push straight to `main`.
- **Reviewers**: PRs are reviewed by the second account `orange-crow-code` (simulates a real team; that account needs write access to the repo). Request the review only once CI is green.
- **PRs require CI to pass** before merging — required checks: `format`, `lint`, and `build` on `ubuntu-latest`, `macos-latest`, and `windows-latest`.
- **Merge strategy**: merge through the GitHub PR UI using **Squash and merge**. The repo enforces `required_linear_history`, so merge commits are not allowed, and a local `git merge --ff-only` followed by `git push` no longer works because pushes to `main` are blocked by protection. Squash and merge folds the branch into a single commit on `main`; the commit message is authored in the merge dialog.
- **Keep the PR up to date**: `strict` status checks require the branch to be rebased on the latest `main` before merging:
  ```bash
  git checkout feat/my-thing
  git rebase main
  git push --force-with-lease
  ```
- **No pushes after approval**: the repo sets `dismiss_stale_reviews`, so any new push dismisses the reviewer's approval and re-triggers CI. Finalize the code before requesting review.
- **Single-commit history** — each PR lands on `main` as exactly one commit (via squash-and-merge); multiple WIP commits on the branch are fine and get squashed at merge time.

## Current state

- Single file `src/timestamp.cpp`
- `CMakeLists.txt` — done
- Smoke tests in `cmake/tests/`
- Single git commit
- CI workflow in `.github/workflows/build.yml` — build + test on push/PR

## Things to do

See [TODO.md](TODO.md)