---
tag: ZTAPHF
kind: tool
slug: homebrew-pm
name: Homebrew
aliases: [brew, Homebrew, Linuxbrew, formula, formulae, cask, keg, Cellar, tap, bottle, Brewfile, brew bundle, macOS package manager]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Homebrew installs command-line tools and applications on macOS and Linux. It puts each version of a package in its own keg in the Cellar, and links the files into one prefix.
role: [package-manager]
licence: bsd-2-clause
repo: https://github.com/Homebrew/brew
homepage: https://brew.sh/
history:
  year: 2009
  people: [Max Howell]
  note: Max Howell wrote Homebrew in 2009. Version 1.0.0 came out on 21 September 2016, and Linuxbrew was merged back into Homebrew in January 2019.
see_also: [package-manager-concept, apt-tool, nix, other-system-package-managers, cpp-package-managers, dependency-resolution, pin-dependency-versions, lock-files]
sources:
  - url: https://brew.sh/
    title: Homebrew, home page
    locator: Install Homebrew; What Does Homebrew Do?
  - url: https://en.wikipedia.org/wiki/Homebrew_(package_manager)
    title: Wikipedia, Homebrew (package manager)
    locator: History
  - url: https://docs.brew.sh/Formula-Cookbook
    title: Homebrew Documentation, Formula Cookbook
    locator: Homebrew terminology
  - url: https://docs.brew.sh/Installation
    title: Homebrew Documentation, Installation
    locator: the introduction
  - url: https://docs.brew.sh/Manpage
    title: Homebrew Documentation, brew(1) manual page
    locator: install; uninstall; list; search; update; upgrade; outdated; deps; pin; autoremove; cleanup; tap
  - url: https://docs.brew.sh/FAQ
    title: Homebrew Documentation, FAQ
    locator: Why does Homebrew say sudo is bad?; Homebrew is a poor name; What does keg-only mean?
  - url: https://docs.brew.sh/Brew-Bundle-and-Brewfile
    title: Homebrew Documentation, brew bundle and Brewfile
    locator: Basic usage; Types
  - url: https://raw.githubusercontent.com/Homebrew/brew/master/LICENSE.txt
    title: Homebrew, LICENSE.txt
    locator: the first lines
---

## What it is

Homebrew installs "the command-line tools and applications you need across macOS, Linux and WSL".[^1] The command is `brew`. Homebrew is a system package manager like [[apt-tool]]: it installs programs for the computer, not the dependencies of one project. The general ideas are on [[package-manager-concept]].

Homebrew uses words from beer brewing.[^2] The Formula Cookbook defines them:[^3]

| Term | Meaning | Example |
|---|---|---|
| formula | a package definition that builds from upstream sources | `homebrew-core/Formula/f/foo.rb` |
| cask | a package definition that installs pre-compiled binaries, built and signed by upstream | `homebrew-cask/Casks/b/bar.rb` |
| prefix | the path where Homebrew is installed | `/opt/homebrew` |
| keg | the directory of one version of a formula | `/opt/homebrew/Cellar/foo/0.1` |
| rack | a directory with one or more versioned kegs | `/opt/homebrew/Cellar/foo` |
| Cellar | the directory with the racks | `/opt/homebrew/Cellar` |
| opt prefix | a symlink to the active version of a keg | `/opt/homebrew/opt/foo` |
| tap | a directory, usually a Git repository, of formulae, casks or commands | `/opt/homebrew/Library/Taps/homebrew/homebrew-core` |
| bottle | a pre-built keg, poured into a rack instead of a build from source | `qt--6.5.1.ventura.bottle.tar.gz` |

A formula is a file in Ruby. Homebrew uses the class `Formula` for it.[^3]

## Install and first use

The home page gives this command, for the macOS Terminal, a Linux shell or WSL:[^1]

```text
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

The script says what it will do and waits before it does it.[^1] It installs Homebrew in the default prefix: `/opt/homebrew` on Apple silicon, `/home/linuxbrew/.linuxbrew` on Linux and `/usr/local` on Intel Macs.[^1][^4] With the default prefix, you do not need `sudo` to install formulae after the first installation.[^4]

Then the basic commands are these:[^5]

```text
brew update            # get the newest Homebrew and formulae with git
brew search wget       # search formula names and cask tokens
brew install wget      # install a formula and its dependencies
brew list              # list the installed formulae
brew outdated          # list the installed packages that have an update
brew upgrade           # upgrade the outdated packages that are not pinned
brew uninstall wget    # uninstall a formula
```

Homebrew is not on the build machine of this site, so these commands are not run here. Do not run `brew` with `sudo`. The FAQ says that `sudo brew` trusts the owner of the prefix, and anyone else who can change the code of Homebrew, to run any code as root.[^6]

## Concepts of the tool

**Kegs and links.** Homebrew does not install files outside its prefix. It installs each package into its own keg in the Cellar, and then makes symlinks to the files of the keg in the prefix.[^1] The script below is a model of this layout in Python. It does not run Homebrew. It makes a keg for version 0.1 of a formula `foo`, links it, and then does the same for version 0.2.

```python run id=brew-keg
# A model of the layout that Homebrew uses. It does not run Homebrew.
# Each version of a formula is a keg in the Cellar; the prefix links to one keg.
import os, shutil, tempfile

prefix = tempfile.mkdtemp()

def pour(name, version):
    """Make a keg: Cellar/NAME/VERSION/bin/NAME."""
    keg = os.path.join(prefix, "Cellar", name, version)
    os.makedirs(os.path.join(keg, "bin"))
    with open(os.path.join(keg, "bin", name), "w") as f:
        f.write(f"{name} {version}\n")
    return keg

def link(name, version):
    """Point opt/NAME (the opt prefix) at one keg, and bin/NAME at its program."""
    keg = os.path.join("..", "Cellar", name, version)
    for d, target in (("opt", keg), ("bin", os.path.join(keg, "bin", name))):
        os.makedirs(os.path.join(prefix, d), exist_ok=True)
        path = os.path.join(prefix, d, name)
        if os.path.islink(path):
            os.remove(path)
        os.symlink(target, path)

def show():
    for root, dirs, files in sorted(os.walk(prefix)):
        dirs.sort()
        for n in sorted(dirs + files):
            p = os.path.join(root, n)
            rel = os.path.relpath(p, prefix)
            if os.path.islink(p):
                print(f"{rel} -> {os.readlink(p)}")
            elif os.path.isfile(p):
                print(rel)
    with open(os.path.join(prefix, "bin", "foo")) as f:
        print("bin/foo runs:", f.read().strip())

pour("foo", "0.1")
link("foo", "0.1")
show()
print("--- pour foo 0.2 and link it")
pour("foo", "0.2")
link("foo", "0.2")
show()
shutil.rmtree(prefix)
```

```output for=brew-keg
Cellar/foo/0.1/bin/foo
bin/foo -> ../Cellar/foo/0.1/bin/foo
opt/foo -> ../Cellar/foo/0.1
bin/foo runs: foo 0.1
--- pour foo 0.2 and link it
Cellar/foo/0.1/bin/foo
Cellar/foo/0.2/bin/foo
bin/foo -> ../Cellar/foo/0.2/bin/foo
opt/foo -> ../Cellar/foo/0.2
bin/foo runs: foo 0.2
```

The two kegs are side by side in the rack `Cellar/foo`, and the links in `bin` and `opt` choose one of them. `brew cleanup` removes old versions of installed formulae, and downloads that are old.[^5]

**Keg-only formulae.** A formula is keg-only if Homebrew does not link it into the prefix. The Formula Cookbook gives the formula `openjdk` as an example.[^3]

**Bottles.** A bottle is a keg that was built before. Homebrew pours it into a rack of the Cellar, so that it does not need to build the formula from the upstream sources.[^3]

**Taps.** A tap adds a repository of formulae, casks or commands. `brew tap user/repo` gets the repository from GitHub with HTTPS.[^5] The main formulae and casks are in the taps `homebrew/core` and `homebrew/cask`.[^3][^5]

**Dependencies.** `brew deps FORMULA` shows the dependencies of a formula. By default it shows the required and the recommended dependencies.[^5] `brew autoremove` uninstalls the formulae that were installed only as a dependency and that nothing needs now.[^5]

**Versions.** `brew pin` keeps a formula at its version, so that `brew upgrade` does not upgrade it. The manual warns that other packages that need a later version of a pinned formula "might not install or run correctly".[^5] Some formulae have the version in the name, for example `postgresql@16` in a Brewfile.[^7]

## Common tasks

| Task | Command |
|---|---|
| Update Homebrew and the formulae | `brew update`[^5] |
| Search | `brew search TEXT`[^5] |
| Install a formula or a cask | `brew install NAME` or `brew install --cask NAME`[^5] |
| List the installed formulae | `brew list`[^5] |
| List the packages that have an update | `brew outdated`[^5] |
| Upgrade all, or one | `brew upgrade` or `brew upgrade NAME`[^5] |
| Keep a formula at its version | `brew pin NAME`, and `brew unpin NAME` to undo it[^5] |
| Show the dependencies | `brew deps NAME`[^5] |
| Remove the dependencies that nothing needs | `brew autoremove`[^5] |
| Remove old versions and old downloads | `brew cleanup`[^5] |
| Install everything in a Brewfile | `brew bundle install`, or `brew bundle`[^7] |

## Configuration

**Brewfile.** `brew bundle` reads a file `Brewfile` and installs what it lists. For the line `brew "ruby"`, it runs `brew install ruby` if Ruby is not installed, and `brew upgrade ruby` if it is outdated.[^7] A Brewfile can list taps, formulae, casks and other kinds of packages:[^7]

```text
brew "postgresql@16", restart_service: true
cask "firefox"
```

`brew bundle dump` writes a Brewfile from what is installed.[^7]

**Environment variables.** Many settings of `brew` are environment variables. For example, if `HOMEBREW_NO_INSTALL_CLEANUP` is set, `brew install` does not run `brew cleanup` after the installation.[^5]

## Alternatives

On Debian and Ubuntu, the system package manager is [[apt-tool]]. [[nix]] also runs on macOS and Linux and keeps each version of a package in its own directory in the Nix store. MacPorts, winget and others are on [[other-system-package-managers]]. For the libraries of a C or C++ project, see [[cpp-package-managers]].

## History and versions

| Date | Event |
|---|---|
| 2009 | Max Howell wrote Homebrew.[^2] |
| 21 September 2016 | Homebrew 1.0.0.[^2] |
| January 2019 | Linuxbrew, the port to Linux, was merged back into Homebrew, with beta support for Linux and WSL.[^2] |
| 5 February 2021 | Homebrew 3.0.0 added official support for Macs with Apple silicon.[^2] |
| 16 February 2023 | Homebrew 4.0.0.[^2] |

The FAQ says that the creator of Homebrew did not think much about the beer theme of the name, and did not think that the project would become popular.[^6] Homebrew is under the BSD 2-Clause License.[^8]

## References
