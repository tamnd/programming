---
tag: NFASQD
kind: tool
slug: uv
name: uv
aliases: [uv, uvx, uv.lock, uv run, uv sync, uv add, uv pip, astral uv]
area: tools-and-workflow/package-managers
level: core
summary: >-
  uv is a fast package and project manager for Python, written in Rust. This page shows uv projects, uv.lock, uv run, uv sync, Python versions, tools, scripts, the pip interface and the history.
role: [package-manager]
licence: mit-or-apache-2.0
repo: https://github.com/astral-sh/uv
homepage: https://docs.astral.sh/uv/
see_also: [pip-tool, package-manager-concept, python, lock-files, pin-dependency-versions, dependency-resolution, add-a-dependency, package-registries]
sources:
  - url: https://docs.astral.sh/uv/
    title: uv documentation, uv
    locator: the introduction; Highlights
    accessed: 2026-11-16
  - url: https://github.com/astral-sh/uv/blob/main/README.md
    title: uv, README
    locator: License
    accessed: 2026-11-16
  - url: https://pypi.org/pypi/uv/json
    title: PyPI, uv (JSON API)
    locator: info.version; releases (upload_time)
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/getting-started/installation/
    title: uv documentation, Installing uv
    locator: Standalone installer; Uninstallation
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/reference/environment/
    title: uv documentation, Environment variables
    locator: UV_OFFLINE; UV_PYTHON_DOWNLOADS
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/projects/dependencies/
    title: uv documentation, Managing dependencies
    locator: Dependency fields
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/projects/layout/
    title: uv documentation, Project structure and files
    locator: The project environment; The lockfile
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/projects/sync/
    title: uv documentation, Locking and syncing
    locator: Automatic lock and sync
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/python-versions/
    title: uv documentation, Python versions
    locator: Python version files; Automatic Python downloads
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/tools/
    title: uv documentation, Tools
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/guides/scripts/
    title: uv documentation, Running scripts
    locator: Declaring script dependencies
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/pip/compatibility/
    title: uv documentation, Compatibility with pip
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/concepts/configuration-files/
    title: uv documentation, Configuration files
    locator: the introduction; Environment variable files
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/reference/settings/
    title: uv documentation, Settings
    locator: index; python-downloads
    accessed: 2026-11-16
  - url: https://astral.sh/blog/uv
    title: "Astral blog, uv: Python packaging in Rust (2024-02-15)"
    locator: the introduction
    accessed: 2026-11-16
  - url: https://astral.sh/blog/uv-unified-python-packaging
    title: "Astral blog, uv: Unified Python packaging (2024-08-20)"
    locator: TL;DR
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/reference/policies/versioning/
    title: uv documentation, Versioning
    locator: the whole page
    accessed: 2026-11-16
---

## What it is

uv is "An extremely fast Python package and project manager, written in Rust".[^1] It aims to be "A single tool to replace pip , pip-tools , pipx , poetry , pyenv , twine , virtualenv , and more".[^1] It manages projects with a lock file, runs scripts, "Installs and manages Python versions", and runs tools from Python packages.[^1] It also has an interface like pip; see [[pip-tool]].

The company Astral makes uv. uv is "licensed under either of" the Apache License 2.0 and the MIT license.[^2] The newest release on 2026-11-12 was uv 0.13.8.[^3] The outputs on this page come from uv 0.11.23.

## Install and first use

The standalone installer is `curl -LsSf https://astral.sh/uv/install.sh | sh`; it puts the commands `uv` and `uvx` in `~/.local/bin`.[^4] uv is also "available in the core Homebrew packages", and "uv is published to PyPI".[^4]

The script makes a library `greet` and an application `app` that depends on it through a path. It locks, runs and syncs the application, and shows the tree. The script runs offline: with `UV_OFFLINE`, "uv will disable network access", and `UV_PYTHON_DOWNLOADS=never` is "Equivalent to the python-downloads setting" with the value `never`, so uv uses a Python of the system.[^5]

```bash run timeout=30 id=uv-first
d=$(mktemp -d)
cd "$d"
export PATH="$HOME/.local/bin:$PATH" UV_CACHE_DIR="$d/cache" UV_PYTHON_DOWNLOADS=never UV_OFFLINE=1 NO_COLOR=1
# A library in a directory next to the application.
uv init --quiet --lib --vcs none --python 3.12 greet
cat >greet/src/greet/__init__.py <<'END'
def hello(name):
    return f"Hello, {name}!"
END
# The application, with the library as a path dependency.
uv init --quiet --app --vcs none --python 3.12 app
cd app
uv add --quiet ../greet
cat >main.py <<'END'
import greet
print(greet.hello("uv"))
END
sed -n '/^dependencies/,/^]/p;/tool.uv.sources/,$p' pyproject.toml
uv run --quiet main.py
grep -E '^(version|requires-python|name)' uv.lock
uv lock --check --quiet && echo "uv.lock is up to date"
uv sync --locked --quiet && echo "synced from uv.lock"
uv tree --quiet | sed "s|$d|DIR|"
```

```output for=uv-first
dependencies = [
    "greet",
]
[tool.uv.sources]
greet = { path = "../greet" }
Hello, uv!
version = 1
requires-python = ">=3.12"
name = "app"
version = "0.1.0"
name = "greet"
version = "0.1.0"
uv.lock is up to date
synced from uv.lock
app v0.1.0
└── greet v0.1.0
```

## Concepts of the tool

**Projects.** A uv project is a directory with a `pyproject.toml`. `uv init` makes one, and `uv add` adds a dependency to the table `[project]`. The dependencies go in `project.dependencies`; the table `tool.uv.sources` gives "Alternative sources for dependencies during development", such as the path in the script.[^6] See [[add-a-dependency]].

**The project environment.** uv "manages a persistent environment with the project and its dependencies in a .venv directory next to the pyproject.toml".[^7] `uv run` runs a command in it.

**uv.lock.** "uv.lock is a universal or cross-platform lockfile that captures the packages that would be installed across all possible Python markers such as operating system, architecture, and Python version."[^7] "This file should be checked into version control".[^7] One lock file serves Linux, macOS and Windows; see [[lock-files]].

**Lock and sync.** "Locking and syncing are automatic in uv. For example, when uv run is used, the project is locked and synced before invoking the requested command."[^8] With `--locked`, "If the lockfile is not up-to-date, uv will raise an error instead of updating the lockfile"; with `--frozen`, uv uses the lock file without the check.[^8] Use `uv sync --locked` in CI.

**Python versions.** "By default, uv will automatically download Python versions if they cannot be found on the system."[^9] The setting `python-downloads` turns this off.[^9] A file `.python-version` gives the default version of a project, and `uv python install 3.13` installs one.[^9]

**Tools.** `uv tool run` runs a command from a package without an install, "in a temporary virtual environment isolated from the current project".[^10] `uvx` is an alias: "the two commands are exactly equivalent".[^10] With `uv tool install`, the "executables are available on the PATH".[^10]

**Scripts.** A single file can declare its own dependencies. `uv add --script example.py 'requests<3' 'rich'` writes a block that starts with `# /// script` at the top of the file, and `uv run example.py` installs them.[^11]

**The pip interface.** `uv pip install` and the other `uv pip` commands work like pip and pip-tools: in most cases, `uv pip install` can take the place of `pip install` with no other change.[^12] But "uv is not intended to be an exact clone of pip".[^12]

## Common tasks

```text
uv init app                       # make a project
uv add requests                   # add a dependency, lock and sync
uv add --dev pytest               # add a development dependency
uv remove requests                # remove a dependency
uv lock                           # update uv.lock
uv lock --upgrade-package requests  # move one package to a newer version
uv sync --locked                  # install from uv.lock, fail if it is out of date
uv run pytest                     # run a command in the project environment
uv run --with rich script.py      # run with one more package for this run
uv tree                           # show the dependency tree
uv python install 3.13            # install a Python version
uv python pin 3.13                # write .python-version
uvx ruff check                    # run a tool without an install
uv tool install ruff              # install a tool for the user
uv build                          # build the wheel and the source distribution
uv publish                        # upload them to a registry
uv pip install -r requirements.txt  # the pip interface
```

See [[publish-a-package]] for `uv build` and `uv publish`, and [[dependency-updates]] for upgrades.

## Configuration

"uv supports persistent configuration files at both the project- and user-level."[^13] uv reads the table `[tool.uv]` of `pyproject.toml`, or a file `uv.toml`, in the current directory or the nearest parent directory; a user file is, for example, `~/.config/uv/uv.toml`.[^13] Many settings also have an environment variable, as the script shows.[^5]

```toml
# pyproject.toml
[tool.uv]
python-downloads = "manual"   # "automatic", "manual" or "never"

[[tool.uv.index]]
name = "company"
url = "https://pypi.example.com/simple/"
```

The value `manual` means: "Do not automatically download managed Python installations; require explicit installation".[^14] The table `[[tool.uv.index]]` adds a package index with a `name` and a `url`.[^14]

## Alternatives

pip with a virtual environment and a requirements file is the basic tool set of Python; see [[pip-tool]]. Poetry, PDM and Hatch manage projects too; see [[python-packaging-tools]].

## History and versions

Astral first released uv on 2024-02-15 as "an extremely fast Python package resolver and installer, written in Rust".[^15] On 2024-08-20, Astral added "features that extend uv beyond a pip alternative, and into an end-to-end solution for managing Python projects, command-line tools, single-file scripts, and even Python itself".[^16] uv does not use semantic versioning: "the minor version number is bumped for breaking changes, and the patch version number is bumped for bug fixes, enhancements, and other non-breaking changes".[^17]

| Version | Date | Notes |
| --- | --- | --- |
| 0.1.0 | 2024-02-15 | A package resolver and installer[^15][^3] |
| 0.3.0 | 2024-08-20 | Projects, tools, scripts and Python versions[^16][^3] |
| 0.5.0 | 2024-11-07 | The installer uses `~/.local/bin`, not `~/.cargo/bin`[^4][^3] |
| 0.13.8 | 2026-11-12 | The newest release on 2026-11-12[^3] |

## References
