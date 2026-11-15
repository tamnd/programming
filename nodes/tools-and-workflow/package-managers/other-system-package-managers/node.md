---
tag: S6CMT5
kind: tool
slug: other-system-package-managers
name: Other system package managers
aliases: [dnf, yum, RPM, pacman, apk, Alpine Package Keeper, winget, Windows Package Manager, Chocolatey, choco, Scoop, MacPorts, port, FreeBSD pkg, FreeBSD Ports Collection]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  dnf, pacman and apk install packages on Linux, winget, Chocolatey and Scoop on Windows, MacPorts on macOS, and pkg on FreeBSD. They install software for the whole computer.
role: [package-manager]
see_also: [apt-tool, homebrew-pm, nix, package-manager-concept, package-registries, dependency-resolution, cpp-package-managers, other-language-package-managers]
sources:
  - url: https://dnf.readthedocs.io/en/latest/command_ref.html
    title: DNF Command Reference
    locator: Description; Install Command; History Command; Autoremove Command
  - url: https://man.archlinux.org/man/pacman.8.en
    title: Arch manual pages, pacman(8)
    locator: DESCRIPTION; -S, --sync; -R, --remove; EXAMPLES
  - url: https://docs.alpinelinux.org/user-handbook/0.1a/Working/apk.html
    title: Alpine Linux User Handbook, Working with the Alpine Package Keeper (apk)
    locator: Repositories, Releases and Mirrors; Adding Packages; World; Upgrading; Virtual Packages
  - url: https://doc.opensuse.org/documentation/leap/reference/html/book-reference/cha-sw-cl.html
    title: Managing software with command line tools, openSUSE Leap 15.6 Reference
    locator: Using Zypper; Installing and removing software with Zypper; Updating software with Zypper
  - url: https://learn.microsoft.com/en-us/windows/package-manager/winget/
    title: Microsoft Learn, Use WinGet to install and manage applications
    locator: the introduction; Use WinGet
  - url: https://docs.chocolatey.org/en-us/getting-started/
    title: Chocolatey documentation, Getting Started
    locator: Requirements; Installing and upgrading a package
  - url: https://github.com/ScoopInstaller/Scoop
    title: Scoop, README
    locator: What does Scoop do?; Known application buckets
  - url: https://guide.macports.org/
    title: MacPorts Guide
    locator: Chapter 1, Introduction; port selfupdate; port install
  - url: https://docs.freebsd.org/en/books/handbook/ports/
    title: FreeBSD Handbook, Chapter 4, Installing Applications, Packages and Ports
    locator: Synopsis; Using pkg for Binary Package Management; Upgrading Installed Packages
---

## What it is

Each operating system has its own way to install software. This page describes the system package managers that are not on the pages [[apt-tool]] (Debian and Ubuntu), [[homebrew-pm]] (macOS and Linux) and [[nix]]. The general ideas are on [[package-manager-concept]].

| System | Tool | What it installs |
|---|---|---|
| RPM-based Linux | dnf | RPM packages; dnf is the major version of YUM that came after it[^1] |
| Arch Linux | pacman | packages in a zipped tar format[^2] |
| Alpine Linux | apk | packages from the Alpine repositories[^3] |
| openSUSE | zypper | packages from the configured repositories[^4] |
| Windows | winget | applications, through the Windows Package Manager service[^5] |
| Windows | Chocolatey | software packages, with PowerShell[^6] |
| Windows | Scoop | apps, installed from the command line[^7] |
| macOS | MacPorts | open source software, built from ports[^8] |
| FreeBSD | pkg and the Ports Collection | pre-built packages, or software built from source[^9] |

dnf, pacman and apk install the dependencies of a package too.[^1][^2][^3] A language package manager, such as [[other-language-package-managers|RubyGems or Composer]], works for one project; a system package manager works for the whole computer.

## Install and first use

WinGet comes with Windows 11, Windows Server 2025 and some versions of Windows 10, as a part of the App Installer.[^5] For the other tools, see the installation steps in their documentation. The commands below change the system, so this page shows them as text and does not run them.

**dnf.** `dnf install` makes sure that the packages that you give, and their dependencies, are installed on the system.[^1]

```text
sudo dnf install PACKAGE
```

**pacman.** The operation `-S` (sync) installs packages from the remote repositories, with all the dependencies that they need to run. `pacman -Syu` updates the package list and then upgrades all packages.[^2]

```text
sudo pacman -S qt
sudo pacman -Syu
```

**apk.** `apk add` adds packages, and you can give more than one at a time. `apk upgrade` is two steps: `apk update` downloads the package index, and then the packages in the world are upgraded with their dependencies.[^3]

```text
apk add bash zsh
apk update
apk upgrade
```

**zypper.** The openSUSE reference calls Zypper "a command line package manager for installing, updating, and removing packages".[^4] `zypper update` updates all installed packages, but it ignores problematic packages: for example, it does not update a locked package.[^4]

```text
sudo zypper install PACKAGE_NAME
sudo zypper remove PACKAGE_NAME
sudo zypper update
```

**winget.** `winget search` finds an application, and `winget install` starts its installer.[^5]

```text
winget search <appname>
winget install <appname>
```

**Chocolatey.** The guide says to open PowerShell as an administrator, because almost all software needs administrative privilege to install, upgrade or uninstall.[^6]

```text
choco install notepadplusplus -y
choco upgrade notepadplusplus -y
```

**Scoop.** Scoop installs apps from the command line. It avoids the User Account Control prompts and the wizards of graphical installers, and it does not fill the `PATH` with entries.[^7]

```text
scoop install python ruby go perl
scoop bucket add extras
```

**MacPorts.** `port selfupdate` updates the local ports tree from the MacPorts repository and upgrades MacPorts itself when necessary. `port install` installs a port.[^8]

```text
sudo port selfupdate
sudo port install apache2
```

**FreeBSD pkg.** `pkg install` installs a binary package. `pkg upgrade` compares the installed versions with those in the repository catalogue and upgrades them.[^9]

```text
pkg install PACKAGE
pkg upgrade
```

## Concepts of the tool

**Binary packages and building from source.** FreeBSD has two complementary ways to install third-party software: the Ports Collection builds from source, and packages are pre-built binaries.[^9] MacPorts is a system "for compiling, installing, and managing open source software". A port is a set of specifications in a file `Portfile`.[^8]

**Repositories.** apk gets the package information and the packages from mirrors. A mirror is a website that hosts repositories, and a repository is a category of packages.[^3] Scoop calls its repositories buckets: `scoop bucket add extras` adds the bucket `extras`.[^7] pacman can sync the local machine with a remote repository to upgrade packages.[^2]

**The packages that you asked for.** apk lists the packages that you installed on purpose in the "world file", `/etc/apk/world`. The handbook says that it is safe to edit it by hand; after an edit, `apk add` with no arguments brings the system to a consistent state.[^3] A virtual package groups other packages: `apk add a b c -t abc` adds `a`, `b` and `c` as the dependencies of `abc`, and `apk del abc` removes all of them, unless something else needs them.[^3]

**History.** `dnf history` shows what happened in past transactions, and can act on that information.[^1]

**Front ends and libraries.** pacman 3.0.0 and later versions are a front end to the library libalpm, so other front ends can be written for the same library.[^2] DNF has an API for extensions and plugins, and a plugin can add commands.[^1]

## Common tasks

| Task | dnf | pacman | apk | winget | MacPorts | FreeBSD pkg |
|---|---|---|---|---|---|---|
| Install | `dnf install P`[^1] | `pacman -S P`[^2] | `apk add P`[^3] | `winget install P`[^5] | `port install P`[^8] | `pkg install P`[^9] |
| Remove | `dnf remove P`[^1] | `pacman -R P`[^2] | `apk del P`[^3] | | `port uninstall P`[^8] | |
| Upgrade all | `dnf upgrade`[^1] | `pacman -Syu`[^2] | `apk upgrade`[^3] | | `port selfupdate`, then upgrade[^8] | `pkg upgrade`[^9] |
| Search | | | | `winget search P`[^5] | `port search P`[^8] | |

An empty cell means that the source of this page does not give the command, not that the tool has no such command.

## Configuration

- pacman: the file `pacman.conf`.[^2]
- apk: the world file `/etc/apk/world`.[^3]
- Scoop: the buckets, which `scoop bucket add` adds.[^7]
- MacPorts: the variants of a port, for example `sudo port install apache2 -preforkmpm +workermpm`, and the file `variants.conf`.[^8]

## Alternatives

[[apt-tool]] is the system package manager of Debian and Ubuntu. [[homebrew-pm]] works on macOS and Linux without root after its installation. [[nix]] keeps each version of a package in its own path, so versions do not conflict. For the libraries of a project, use the package manager of the language: see [[other-language-package-managers]] and [[cpp-package-managers]].

## History and versions

The sources of this page give few dates. Two version facts are in them: pacman became a front end to libalpm in version 3.0.0,[^2] and DNF is the major version of YUM that came after it, and it keeps the command line of YUM roughly the same.[^1]

## References
