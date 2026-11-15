---
tag: RMVJ63
kind: concept
slug: transitive-dependencies
name: Transitive dependencies
aliases: [transitive dependency, indirect dependency, dependency of a dependency, dependency tree, dependency graph, diamond dependency, duplicate dependencies, hoisting]
area: tools-and-workflow/package-managers
level: core
summary: >-
  A transitive dependency is a package that a project needs only because one of its dependencies needs it. Shared dependencies form diamonds, and conflicting needs can bring in two versions.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2015
  people: []
  note: npm 3.0.0, released in June 2015, installs dependencies as flat as possible and nests a package only when two packages need conflicting versions.
see_also: [dependency-resolution, package-manager-concept, version-constraints, lock-files]
sources:
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Maven, Introduction to the Dependency Mechanism
    locator: Transitive Dependencies
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: require directive; go directive
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v11/configuring-npm/folders
    title: npm Docs (v11), folders
    locator: More Information; Cycles, Conflicts, and Folder Parsimony; Example
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/concepts/dependency-resolution
    title: NuGet documentation, NuGet Package Dependency Resolution
    locator: Dependency resolution with PackageReference; Dependency resolution rules
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/resolver.html
    title: The Cargo Book, Dependency Resolution
    locator: Resolver algorithm; Version-incompatibility hazards
    accessed: 2026-11-16
  - url: https://github.com/npm/npm/releases/tag/v3.0.0
    title: npm, release v3.0.0, 26 June 2015
    locator: Flat, flat, flat!
    accessed: 2026-11-16
---

## Idea

A project lists its **direct dependencies**: the packages that its own code uses. Each of these packages has its own dependencies, and they have theirs. All the packages that the project gets in this way, but does not list itself, are its **transitive dependencies**, also called indirect dependencies.

Package managers install them without being asked. Maven "avoids the need to discover and specify the libraries that your own dependencies require by including transitive dependencies automatically", and "There is no limit to the number of levels that dependencies can be gathered from".[^1] Maven notes that "the graph of included libraries can quickly grow quite large".[^1]

Go marks the difference in the file `go.mod`. An `// indirect` comment on a requirement "indicates that no package from the required module is directly imported by any package in the main module".[^2]

## How it works

The dependencies of a project form a directed graph: an edge goes from each package to each package that it needs. The package manager walks this graph from the project. Three shapes need care:

- **A diamond.** Two packages need the same third package. In the examples below, `cli` and `web` both need `log`. The package manager installs `log` one time, and both packages use it.
- **Two versions.** Two packages need versions of the same package that are not compatible. In the examples, `cli` needs `json` 0.9 and `web` needs `json` 1.1. Some package managers install both versions; others must choose one or fail. See [[dependency-resolution]].
- **A cycle.** A package can depend, through other packages, on itself. npm stops the loop because a package that is already installed in a parent folder is not installed again.[^3] For Maven, "A problem arises only if a cyclic dependency is discovered".[^1]

npm shows how one tool handles all three in folders. When you install `foo`, npm unpacks it into `./node_modules/foo`, and the dependencies of `foo` go into `./node_modules/foo/node_modules/...`.[^3] To save space, npm "hoists dependencies by default": it installs each dependency at the highest folder possible. If a package needs a version that is not compatible with the copy in a parent folder, npm "must install its own copy" in a nested folder.[^3]

The examples below print the tree of a small project, mark each package that was already shown with `(*)`, and then list the packages that more than one package needs and the packages that are present in more than one version.

## In languages

Each example stores the graph as a map from a package and its version to the list of packages that it needs. A recursive function prints the tree with a prefix of `|   ` or four spaces for each level, and keeps a set of the packages that it has shown. The direct dependencies are the children of `app`; the transitive dependencies are the other packages in the set.

## Variations and related ideas

- **Nearest version wins.** When the tree holds two versions of one artifact, Maven uses the one that is closest to the project. With `A -> B -> C -> D 2.0` and `A -> E -> D 1.0`, Maven uses D 1.0, "because the path from A to D through E is shorter".[^1]
- **A flat graph with rules.** NuGet "adds references to a flat package graph" and resolves conflicts ahead of time, which it calls transitive restore.[^4] When different subgraphs need different versions of one package, NuGet uses "the lowest version that satisfies all version requirements".[^4]
- **One version for each compatible range.** Cargo unifies the requirements on a package when one version fits them all. Two exact requirements on different compatible versions, such as `=0.4.11` and `=0.4.8`, are an error. When the requirements allow only versions that are not compatible, such as rand 0.6 and 0.7, "two different versions (e.g. 0.6.5 and 0.7.3) will be resolved and built". The command `cargo tree -d` shows "duplicate versions and where they come from".[^5]
- **Control by the project.** Maven lets a project take charge of transitive dependencies. The `dependencyManagement` section fixes the version of an artifact wherever it appears in the tree. An `exclusion` element removes a dependency of a dependency, and a library can mark a dependency as `optional`, so that its users do not get it.[^1]
- **Every module in go.mod.** At go 1.17 or higher, the file `go.mod` "includes an explicit require directive for each module that provides any package transitively imported by a package or test in the main module".[^2] This makes the indirect dependencies visible in the project's own file.

## Pitfalls

- **Two copies of one type.** When two versions of a crate are in the graph, the Rust compiler treats their types as different, "even if they have the same name". Cargo gives the example of a downcast that fails at run time because the value comes from version 1.0.0 and the target type from version 2.0.0.[^5] The same problem can occur in any language that loads two copies of a library.
- **A downgrade that you did not see.** With nearest-wins rules, a deep dependency can get a lower version than it asked for. NuGet warns that its direct-dependency-wins rule "can result in a downgrade of the package version, thus potentially breaking other dependencies in the graph".[^4]
- **Code that uses a transitive dependency directly.** If your code imports a package that you get only through another package, the next update of that package can remove it. List each package that your code uses as a direct dependency.
- **Unknown code.** Each transitive dependency is code that runs in your build. Look at the full tree, for example with `npm ls`, which gives "a graphical breakdown of what is installed where".[^3]

## Cost

The number of transitive dependencies grows with each level of the graph, and each one costs download time, disk space and review work. Duplicate versions cost more space and, in some languages, larger programs. npm 3 made installs "maximally flat", so that packages are nested only "when two (or more) modules have conflicting dependencies".[^6]

## History

npm 3.0.0, released on 26 June 2015, changed the folder layout: "Your dependencies will now be installed maximally flat", and installs are "deduped from the start".[^6] Maven, Cargo, NuGet and Go each have their own rule for the shared and duplicate packages that a flat graph creates.[^1][^5][^4][^2]

## References
