---
tag: 3M0H3Z
kind: concept
slug: package-manager-concept
name: Package manager
aliases: [package manager, package management system, dependency manager, language package manager, system package manager, install a package]
area: tools-and-workflow/package-managers
level: intro
summary: >-
  A package manager installs, updates and removes packages, and the packages that they depend on. Language package managers serve one language; system package managers install programs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: []
  note: The Comprehensive Perl Archive Network (CPAN) has been online since October 1995.
see_also: [dependency-resolution, version-constraints, lock-files, transitive-dependencies, semantic-versioning, build-system-concept]
sources:
  - url: https://doc.rust-lang.org/cargo/guide/why-cargo-exists.html
    title: The Cargo Book, Why Cargo Exists
    locator: Preliminaries; Enter Cargo
    accessed: 2026-11-16
  - url: https://www.debian.org/doc/manuals/debian-faq/pkg-basics.en.html
    title: The Debian GNU/Linux FAQ, Chapter 7, Basics of the Debian package management system
    locator: 7.1 What is a Debian package; 7.9 Depends, Recommends, Suggests, Conflicts, Replaces, Breaks or Provides
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/
    title: pip documentation
    locator: the first paragraph
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/about-npm
    title: npm Docs, About npm
    locator: the first paragraphs; Use npm to
    accessed: 2026-11-16
  - url: https://manpages.debian.org/trixie/apt/apt.8.en.html
    title: Debian trixie manual pages, apt(8)
    locator: DESCRIPTION; update; install; remove; autoremove
    accessed: 2026-11-16
  - url: https://arxiv.org/abs/2011.07851
    title: Pietro Abate, Roberto Di Cosmo, Georgios Gousios and Stefano Zacchiroli, Dependency Solving Is Still Hard, but We Are Getting Better at It, SANER 2020
    locator: abstract
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/folders
    title: npm Docs (v11), folders
    locator: Cycles, Conflicts, and Folder Parsimony
    accessed: 2026-11-16
  - url: https://edolstra.github.io/pubs/phd-thesis.pdf
    title: Eelco Dolstra, The Purely Functional Software Deployment Model, PhD thesis, Utrecht University, 2006
    locator: title page; 1.5 Contributions; the discussion of DLL hell in chapter 1
    accessed: 2026-11-16
  - url: https://research.swtch.com/vgo-mvs
    title: Russ Cox, Minimal Version Selection (Go and Versioning, Part 4), 2018
    locator: the introduction
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/specifications/externally-managed-environments/
    title: Python Packaging User Guide, Externally Managed Environments
    locator: the introduction; Overview
    accessed: 2026-11-16
  - url: https://www.cpan.org/
    title: The Comprehensive Perl Archive Network (CPAN)
    locator: Welcome to CPAN
    accessed: 2026-11-16
---

## Idea

A program uses code that other people wrote. To build or run it, you need that code in the right version, and the code that it uses in turn. A **package manager** is the tool that gets this code, installs it, updates it and removes it. The Cargo Book gives the reason for one: most programs depend on external libraries, and so they depend transitively on the dependencies of those libraries. To get the correct versions of all of them and keep them up to date by hand "would be hard and error-prone".[^1]

A **package** is the unit that the tool installs. It holds files, a name, a version and a list of the packages that it needs. A Debian binary package holds "executables, configuration files, man/info pages, copyright information, and other documentation", and its control file lists its dependencies.[^2] A Rust package has the file `Cargo.toml` with the same kind of information.[^1]

There are two large groups:

- **Language package managers** install libraries for one language into one project. Examples are Cargo for Rust, which "is the Rust package manager", pip, which "is the package installer for Python", and npm for JavaScript.[^1][^3][^4]
- **System package managers** install programs and shared libraries for the whole operating system. On Debian and Ubuntu, `dpkg` installs a `.deb` file, and `apt` is the front end that finds packages and their dependencies.[^2][^5]

The smallest use is one command. With apt, `apt install` installs a package and the packages that it depends on, and `apt remove` removes it.[^5]

## How it works

A package manager does five jobs.

1. **Find.** It reads an index of the available packages from a source, which is called a registry, a repository or an index. `apt update` downloads this index "from all configured sources".[^5] pip installs packages from the Python Package Index and other indexes.[^3]
2. **Resolve.** It reads the dependencies of the package, then the dependencies of those dependencies, and it chooses one version of each package that satisfies all the requirements. This step is hard in general. Abate and others show that dependency solving is NP-complete "in all non-trivial component models".[^6] The [[dependency-resolution]] page explains the algorithms.
3. **Fetch and install.** It downloads each package and puts its files in place, dependencies first. A Debian package can name another package that "absolutely must be installed" before it runs, and it can name packages that it conflicts with.[^2]
4. **Record.** It writes down what it installed, and why. apt keeps a mark for each package that it installed only to satisfy a dependency.[^5] A language package manager writes a lock file with the exact versions; see [[lock-files]].
5. **Remove.** It removes a package only if no other installed package needs it. `apt autoremove` removes packages "that were automatically installed to satisfy dependencies for other packages and are now no longer needed".[^5]

The examples below show these jobs on a small registry. `app` needs `web` and `log`, `web` needs `http` and `log`, and `cli` needs `log`. The program installs `app` and `cli`, refuses to remove `log` while other packages need it, removes `app`, and then removes the dependencies that nothing needs.

## In languages

Each example keeps a map from each installed package to a flag: true if the user asked for the package, false if it came as a dependency. This is the same information as the automatic mark of apt.[^5] `install` installs the dependencies first, `remove` refuses to break an installed package, and `autoremove` repeats until no unused dependency is left.

## Variations and related ideas

- **Project and global installs.** A language package manager usually installs into one project, for example into the folder `node_modules` of a Node.js project, so that two projects can use different versions of a library. A system package manager installs into shared directories for all users.[^7][^2]
- **One version or many.** apt installs one version of each package on the system. npm can install two versions of a package in nested `node_modules` folders when two packages need versions that are not compatible.[^7] See [[transitive-dependencies]].
- **Purely functional deployment.** Nix stores each package in a path that depends on all the inputs that built it. In Dolstra's words, "a binary component is uniquely defined by the declared inputs used to build the component", so many versions can be installed side by side, and an upgrade does not overwrite a package that another program uses.[^8]
- **Minimal versions.** Go modules do not pick the newest version in the registry. Each module gives only a minimum version for each requirement, and the build uses the highest of these minimums. Cox calls this minimal version selection.[^9]
- **Build tools with a package manager.** Cargo also builds the code: it fetches and builds the dependencies, and it calls `rustc` with the correct parameters.[^1]

## Pitfalls

- **Breaking a shared dependency.** If an upgrade replaces a library that another program needs, the other program can fail. Dolstra describes this as "the well-known DLL hell, where upgrading or installing one application can cause a failure in another application due to shared dynamic libraries".[^8] Use the package manager, not manual copies, so that it can check the dependencies.
- **Removing too much.** `apt autoremove` removes the packages that are marked as automatic. The apt manual asks you to check that the list does not include applications that you use, even if they were once installed only as a dependency, and to mark such a package as manually installed.[^5]
- **Mixing package managers.** If pip and the system package manager both install packages into the same Python installation, they can conflict. The Python packaging specification for externally managed environments says that this can "break the entire underlying operating system at worst". When an installation has this marker file, an installer such as pip should refuse to install outside a virtual environment, and it can offer an override such as `--break-system-packages`.[^10] Install language packages into a project or a virtual environment.
- **Unpinned versions.** Without a lock file, the same command can install other versions next week. See [[lock-files]].

## Cost

A package manager saves the work of finding, downloading and checking each dependency by hand. The costs are these:

- **Resolution time.** Because dependency solving is NP-complete in general, a solver can take a long time on a large set of constraints, or fail to find a solution that exists.[^6]
- **Disk space.** Project installs and side-by-side versions keep many copies of the same library. Nix keeps the versions side by side, and it has automatic garbage collection of unused components.[^8]
- **Trust.** Each dependency is code from another person that runs in your build or your program.

## History

CPAN, the archive of Perl modules, "has been online since October 1995".[^11] Debian uses `dpkg` and apt, with a model of packages that declare their dependencies and conflicts.[^2] Language package managers such as pip, npm and Cargo followed for most languages.[^3][^4][^1] In 2006 Eelco Dolstra described the purely functional deployment model of Nix in a PhD thesis at Utrecht University.[^8] In 2018 Russ Cox proposed minimal version selection for Go.[^9] In 2020 Abate and others looked back at the research on dependency solving, and found that some proposals, such as SAT-based dependency solving, were starting to take off in package managers.[^6]

## References
