---
tag: AHKFSH
kind: tool
slug: apt-tool
name: APT
aliases: [apt, apt-get, apt-cache, Advanced Package Tool, dpkg, dpkg-query, .deb, sources.list, apt_preferences, Debian package manager, Ubuntu package manager]
area: tools-and-workflow/package-managers
level: core
summary: >-
  APT is the package manager of Debian and Ubuntu. It reads package lists from the sources in sources.list, chooses versions, and installs .deb packages and their dependencies with dpkg.
role: [package-manager]
licence: gpl-2.0-or-later
repo: https://salsa.debian.org/apt-team/apt
homepage: https://wiki.debian.org/AptCLI
history:
  year: 1998
  people: [Scott K. Ellis, Jason Gunthorpe]
  note: APT 0.0.1 is dated 31 March 1998 in the changelog. The command apt came with APT 1.0 on 1 April 2014.
see_also: [package-manager-concept, dependency-resolution, version-constraints, homebrew-pm, nix, other-system-package-managers, cpp-package-managers, reproducible-builds]
sources:
  - url: https://www.debian.org/doc/manuals/debian-reference/ch02.en.html
    title: Debian Reference, Chapter 2, Debian package management
    locator: Debian package management prerequisites; Basic package management operations
  - url: https://manpages.debian.org/bookworm/apt/apt-get.8.en.html
    title: Debian manual pages, apt-get(8)
    locator: update; upgrade; install; --simulate
  - url: https://manpages.debian.org/bookworm/apt/apt.8.en.html
    title: Debian manual pages, apt(8)
    locator: DESCRIPTION; SCRIPT USAGE AND DIFFERENCES FROM OTHER APT TOOLS
  - url: https://manpages.debian.org/bookworm/apt/apt-cache.8.en.html
    title: Debian manual pages, apt-cache(8)
    locator: show; depends; policy
  - url: https://manpages.debian.org/bookworm/dpkg/dpkg-query.1.en.html
    title: Debian manual pages, dpkg-query(1)
    locator: -W, --show; -S, --search
  - url: https://manpages.debian.org/bookworm/apt/apt_preferences.5.en.html
    title: Debian manual pages, apt_preferences(5)
    locator: APT's Default Priority Assignments
  - url: https://manpages.debian.org/bookworm/apt/sources.list.5.en.html
    title: Debian manual pages, sources.list(5)
    locator: DESCRIPTION; ONE-LINE-STYLE FORMAT; DEB822-STYLE FORMAT
  - url: https://metadata.ftp-master.debian.org/changelogs/main/a/apt/apt_2.2.4_changelog
    title: The Debian Project, apt package changelog (apt 2.2.4)
    locator: the entries for apt 0.0.1 and apt 1.0
  - url: https://metadata.ftp-master.debian.org/changelogs/main/a/apt/apt_2.2.4_copyright
    title: The Debian Project, apt package copyright file (apt 2.2.4)
    locator: the first lines
---

## What it is

APT is the package manager of Debian. Ubuntu uses it too: the output on this page comes from Ubuntu 24.04. A package is a `.deb` file. The Debian Reference lists `dpkg` as the "low level package management system for Debian (file based)", and APT as the front end that manages packages from the command line with `apt`, `apt-get` and `apt-cache`.[^1] APT gets the lists of packages from the sources that the system names, chooses a version of each package, finds the dependencies, and then lets dpkg install the files.[^2][^1]

APT is a system package manager. It installs programs and libraries for the whole system. The commands on this page that change the system start with `sudo`, as in the Debian Reference.[^1] A language package manager, such as [[cargo]] or [[npm-pm]], installs the dependencies of one project. The general ideas are on [[package-manager-concept]].

There are three commands for the user:

- `apt` is "a high-level commandline interface for the package management system". It is for people at a terminal.[^3]
- `apt-get` installs, upgrades and removes packages, and gets the package lists.[^2]
- `apt-cache` reads the package lists and shows information about packages. It does not change the system.[^4]

The manual of `apt` says that its command line "may change behavior between versions". All its features are also in `apt-get` and `apt-cache`, and you should prefer those in scripts.[^3]

## Install and first use

APT comes with Debian and Ubuntu. The basic steps are these:

```text
sudo apt update             # get the package lists from the sources
apt show cmake              # show information about a package
sudo apt install cmake      # install a package and its dependencies
sudo apt upgrade            # upgrade the installed packages
sudo apt remove cmake       # remove a package
```

These commands change the system, so this page does not run them. The command `apt-get install -s` (or `--simulate`) shows what an install would do, but does "not actually change the system", and a user that is not root can run it.[^2]

The script below runs only commands that read. It asks dpkg which version of `pkg-config` is installed and which package owns the file `/usr/bin/pkg-config`.[^5] Then it asks APT about the dependencies, the version that APT would choose, and some fields of the package record.[^4] The output is from the build machine of this site, which runs Ubuntu 24.04.

```bash run timeout=30 id=apt-query
# Read-only queries. None of these commands changes the system.
apt --version
echo "--- dpkg-query: the installed version"
dpkg-query -W -f='${Package} ${Version} ${Status}\n' pkg-config pkgconf
echo "--- dpkg -S: which package owns a file"
dpkg -S /usr/bin/pkg-config
echo "--- apt-cache depends"
apt-cache depends pkg-config
echo "--- apt-cache policy"
apt-cache policy pkg-config
echo "--- apt-cache show (some fields)"
apt-cache show pkg-config | grep -E '^(Package|Version|Section|Depends|Filename):'
```

```output for=apt-query
apt 2.8.4 (amd64)
--- dpkg-query: the installed version
pkg-config 1.8.1-2build1 install ok installed
pkgconf 1.8.1-2build1 install ok installed
--- dpkg -S: which package owns a file
pkgconf:amd64: /usr/bin/pkg-config
--- apt-cache depends
pkg-config
  Depends: pkgconf
--- apt-cache policy
pkg-config:
  Installed: 1.8.1-2build1
  Candidate: 1.8.1-2build1
  Version table:
 *** 1.8.1-2build1 500
        500 http://archive.ubuntu.com/ubuntu noble/main amd64 Packages
        100 /var/lib/dpkg/status
--- apt-cache show (some fields)
Package: pkg-config
Version: 1.8.1-2build1
Section: devel
Depends: pkgconf (>= 1.8.0-7~)
Filename: pool/main/p/pkgconf/pkg-config_1.8.1-2build1_amd64.deb
```

On this system, `pkg-config` is a small package that depends on `pkgconf`, and the file `/usr/bin/pkg-config` belongs to `pkgconf`. `apt-cache policy` prints the details of the priority selection for the package: the installed version, the candidate version, and the priority of each source.[^4] The archive has priority 500 and the installed copy in `/var/lib/dpkg/status` has priority 100. These are the default priorities in the manual of `apt_preferences`.[^6]

## Concepts of the tool

**Sources.** The file `/etc/apt/sources.list`, and the files in the directory `/etc/apt/sources.list.d/`, name the archives that APT uses.[^7] There are two formats. The one-line style puts one source on each line:[^7]

```text
deb http://deb.debian.org/debian bookworm main contrib non-free non-free-firmware
```

The deb822 style uses fields. The Debian Reference calls the one-line style "older deprecated", and the command `sudo apt modernize-sources` changes the old files to the deb822 style. The deb822 files end in `.sources`.[^1][^7]

```text
Types: deb
URIs: http://deb.debian.org/debian
Suites: bookworm bookworm-updates
Components: main contrib non-free non-free-firmware
```

**Package lists.** `apt-get update` gets the index files of the packages from the sources, so that APT knows about the packages that were added or updated. The manual says that an update should always come before an upgrade.[^2] The other commands use the lists on the computer, and the lists can be old.

**Candidate versions and priorities.** For each package, APT chooses one candidate version. Each version gets a priority. If no target release is set, APT gives priority 100 to the installed versions and priority 500 to the versions that are not installed.[^6] The file `/etc/apt/preferences`, and the files in `/etc/apt/preferences.d/`, can change the priorities, for example to hold a package at one version.[^6] `apt-cache policy` shows the result, and the manual says that it is meant to "help debug issues relating to the preferences file".[^4]

**Dependencies.** A Debian package declares its dependencies in fields of its control file, with version constraints such as `pkgconf (>= 1.8.0-7~)` in the output above. All the packages in the field `Depends` must be installed with the package. `Pre-Depends` is like `Depends`, but the listed packages must be fully installed first. `Recommends` is a strong dependency, but not an absolute one.[^1] `apt autoremove` removes packages that were installed only as dependencies and that nothing needs now.[^3]

**upgrade and full-upgrade.** `upgrade` upgrades the installed packages, but it does not remove packages. If an upgrade needs to remove a package, APT does not do that upgrade. `full-upgrade` can remove packages if that is necessary to upgrade the system as a whole.[^3]

## Common tasks

| Task | Command |
|---|---|
| Get the package lists | `sudo apt update`[^3] |
| Search the package lists | `apt search WORD`[^3] |
| Show information about a package | `apt show PACKAGE` or `apt-cache show PACKAGE`[^3][^4] |
| Install a package, or a given version | `sudo apt install PACKAGE` or `sudo apt install PACKAGE=VERSION`[^3] |
| See what an install would do, with no change | `apt-get install -s PACKAGE`[^2] |
| Upgrade the installed packages | `sudo apt upgrade` or `sudo apt full-upgrade`[^3] |
| Remove a package, or remove it with its configuration files | `sudo apt remove PACKAGE` or `sudo apt purge PACKAGE`[^3] |
| Remove the dependencies that nothing needs | `sudo apt autoremove`[^3] |
| Show the dependencies of a package | `apt-cache depends PACKAGE`[^4] |
| Show the candidate version and the priorities | `apt-cache policy PACKAGE`[^4] |
| Show the installed version | `dpkg-query -W PACKAGE`[^5] |
| Find the package that owns a file | `dpkg -S PATH`[^5] |

## Configuration

- `/etc/apt/sources.list` and `/etc/apt/sources.list.d/`: the sources.[^7]
- `/etc/apt/preferences` and `/etc/apt/preferences.d/`: the priorities, which control which version APT chooses.[^6]
- `apt.conf`: the options of the APT tools. The command `apt` only changes the default values of some of these options.[^3]

## Alternatives

Other system package managers are on [[other-system-package-managers]]: for example dnf and pacman. [[homebrew-pm]] installs packages on macOS and Linux. [[nix]] works on Linux and macOS and installs each version of a package in its own directory. For C and C++ libraries in a project, see [[cpp-package-managers]].

## History and versions

| Date | Event |
|---|---|
| 31 March 1998 | APT 0.0.1, the first entry in the changelog, by Scott K. Ellis.[^8] |
| 1 April 2014 | APT 1.0. The changelog of this version has the entry "install apt binary", which added the command `apt`.[^8] |

The copyright file of APT says: "Apt is copyright 1997, 1998, 1999 Jason Gunthorpe and others." APT is under the GNU General Public License, version 2 or later.[^9]

## References
