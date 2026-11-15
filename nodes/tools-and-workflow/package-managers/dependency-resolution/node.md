---
tag: 2X57E3
kind: concept
slug: dependency-resolution
name: Dependency resolution
aliases: [dependency resolution, version resolution, version solving, dependency solver, resolver, backtracking resolver, PubGrub, minimal version selection, MVS]
area: tools-and-workflow/package-managers
level: core
summary: >-
  Dependency resolution chooses one version of each package so that every version requirement in the graph is true. Resolvers use backtracking, conflict learning or minimal version selection.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2018
  people: [Russ Cox, Natalie Weizenbaum]
  note: In 2018 Russ Cox proposed minimal version selection for Go, and Natalie Weizenbaum introduced the PubGrub algorithm for the Dart package manager.
see_also: [package-manager-concept, version-constraints, lock-files, transitive-dependencies, semantic-versioning]
sources:
  - url: https://github.com/dart-lang/pub/blob/master/doc/solver.md
    title: Dart pub repository, doc/solver.md, the PubGrub version solving algorithm
    locator: Overview; Conflict Resolution
    accessed: 2026-11-16
  - url: https://arxiv.org/abs/2011.07851
    title: Pietro Abate, Roberto Di Cosmo, Georgios Gousios and Stefano Zacchiroli, Dependency Solving Is Still Hard, but We Are Getting Better at It, SANER 2020
    locator: abstract
    accessed: 2026-11-16
  - url: https://pip.pypa.io/en/stable/topics/dependency-resolution/
    title: pip documentation, Dependency Resolution
    locator: Backtracking
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/cargo/reference/resolver.html
    title: The Cargo Book, Dependency Resolution
    locator: Resolver algorithm (key steps); Version numbers; Lock file
    accessed: 2026-11-16
  - url: https://github.com/pubgrub-rs/pubgrub
    title: pubgrub-rs, PubGrub (README)
    locator: The PubGrub Algorithm
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/reference/internals/resolver/
    title: uv documentation, Reference, Internals, Resolver
    locator: Resolver
    accessed: 2026-11-16
  - url: https://go.dev/ref/mod
    title: Go Modules Reference
    locator: require directive
    accessed: 2026-11-16
  - url: https://research.swtch.com/vgo-mvs
    title: Russ Cox, Minimal Version Selection (Go and Versioning, Part 4), 2018
    locator: the introduction; Algorithm 1, Construct Build List
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/nuget/concepts/dependency-resolution
    title: NuGet documentation, NuGet Package Dependency Resolution
    locator: Dependency resolution rules
    accessed: 2026-11-16
  - url: https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html
    title: Maven, Introduction to the Dependency Mechanism
    locator: Transitive Dependencies; dependency mediation
    accessed: 2026-11-16
---

## Idea

A project names its direct dependencies, and each one gives a range of versions that it accepts. Each version of each dependency has its own requirements. **Dependency resolution**, also called version solving, chooses one version of each package so that all these requirements are true at the same time. The pub documentation states the goal: all dependencies are satisfied, "only one version of each package is selected", and every selected package is reachable from the root package.[^1]

The problem is hard. Abate and others write that "Dependency solving is a hard (NP-complete) problem in all non-trivial component models", because two versions of the same package cannot both be installed, or because packages declare conflicts.[^2] In practice, resolvers solve the common cases fast and give an error for the rest.

The result of a resolution is usually written to a lock file, so that the next install does not resolve again; see [[lock-files]].

## How it works

A resolver keeps a partial solution: the packages that it has chosen so far, and the requirements that it must still check. The basic algorithm is a search with backtracking:

1. Take the next requirement. If the package is already chosen, check that the chosen version fits the range. If it does not, the partial solution has a conflict.
2. Otherwise, try the versions of the package in order of preference, usually the newest first. Choose one, add its own requirements to the list, and continue.
3. On a conflict, go back to the last choice and try the next version. If no version is left, go back further. If the first choice has no version left, there is no solution.

pip works this way. When it finds that an earlier assumption is wrong, "it has to backtrack, which means also discarding some of the work that has already been done, and going back to choose another path". The pip documentation adds that this "is not unexpected behaviour or a bug".[^3] The Cargo resolver also walks the dependencies, tries a version, and backtracks when no valid solution is found.[^4]

The examples below resolve two cases. `app` needs `web` from 1.0 up to 3.0 and `log` from 1.0 up to 2.0. The newest `web`, 2.0, needs `log` 2.0 or later, so it conflicts with the requirement of `app`. The resolver tries the three versions of `log` that fit, then goes back and chooses `web` 1.1. In the second case `app` needs `web` 2.0, and the resolver reports that there is no solution.

## In languages

Each example stores the requirements as triples of a name, the lowest allowed version and the first version that is not allowed. `solve` takes the list of open requirements and the map of chosen versions. It returns the map, or nothing when the branch fails, so the caller tries the next version. The map is copied for each choice, so going back needs no undo step.

## Variations and related ideas

- **Conflict-driven clause learning.** Simple backtracking can try the same failing combination again and again. In the example, the resolver tries all three versions of `log` with `web` 2.0, although the cause of the conflict is `web` 2.0 itself. PubGrub backtracks to the package that caused the conflict, and it "also records the root cause of that conflict", so it does not repeat the same dead end.[^1] It is "a SAT solver using conflict-driven clause learning".[^5] uv uses pubgrub-rs, the Rust implementation of PubGrub.[^6] The records of the conflicts also give a clear error message that names the packages involved.[^6]
- **Generic solvers.** Some package managers give the problem to a general SAT, PBO or MILP solver. Abate and others found that SAT-based dependency solving is starting to take off.[^2]
- **Minimal version selection.** Go modules avoid the search. A `require` directive "declares a minimum required version", and the go command uses minimal version selection (MVS) to produce the build list.[^7] To construct the build list, Cox writes, start with the target and append the build list of each requirement, and "If a module appears in the list multiple times, keep only the newest version".[^8] So the result is the highest of the minimums, not the newest version in the registry. Because a requirement has no upper limit, there are no conflicts to backtrack from. The program below follows this definition:

```python run id=minimal-version-selection
# Minimal version selection: visit every requirement, then keep the highest of the minimums.
REQUIRES = {
    ("A", "1.0"): [("B", "1.2"), ("C", "1.2")],
    ("B", "1.2"): [("D", "1.3")],
    ("C", "1.2"): [("D", "1.4")],
    ("D", "1.3"): [],
    ("D", "1.4"): [],
    ("D", "1.5"): [],  # newer, but nothing asks for it
}


def key(version):
    return tuple(int(part) for part in version.split("."))


def build_list(target):
    seen, todo = set(), [target]
    while todo:
        module = todo.pop()
        if module not in seen:
            seen.add(module)
            todo.extend(REQUIRES[module])
    print("reachable:", ", ".join(f"{m} {v}" for m, v in sorted(seen)))
    newest = {}
    for module, version in seen:
        if module not in newest or key(version) > key(newest[module]):
            newest[module] = version
    return sorted(newest.items())


print("build list:", ", ".join(f"{m} {v}" for m, v in build_list(("A", "1.0"))))
```

```output
reachable: A 1.0, B 1.2, C 1.2, D 1.3, D 1.4
build list: A 1.0, B 1.2, C 1.2, D 1.4
```

- **Lowest applicable version.** NuGet restores "the lowest possible version of a package as defined by its dependencies". When a package is also a direct dependency of the application, the direct version wins, and when two subgraphs need different versions, NuGet uses the lowest version that satisfies all of the requirements.[^9]
- **Nearest definition.** Maven does not search. When the tree holds several versions of one artifact, Maven picks the "nearest definition", the version that is closest to the project in the tree. If two versions are at the same depth, the first declaration wins.[^10]
- **Unification.** Cargo prefers the highest version that is available, and it tries to reuse one version of a package for all the packages that need it. If conflicts stop this, Cargo backtracks, "erroring if no solution is found, rather than selecting multiple versions". When multiple versions of one crate do appear in the graph, the Rust compiler treats their types as different types, and `cargo tree -d` shows where the duplicates come from.[^4] See [[transitive-dependencies]].

## Pitfalls

- **Long backtracking.** On a large graph a backtracking resolver can try many versions. pip may download several versions of the same package while it backtracks.[^3] Narrow the ranges of your direct dependencies, or use a lock file.
- **Silent downgrades.** Under the rule that a direct dependency wins, NuGet can pick a lower version than a transitive package needs. NuGet warns that this "can result in a downgrade of the package version, thus potentially breaking other dependencies in the graph", and it gives a warning.[^9]
- **A version that the build did not test.** With Maven's nearest definition, a deep dependency that needs version 2.0 can get version 1.0, because 1.0 is nearer to the project.[^10] Declare the version in your own POM to choose it.
- **Resolving again by accident.** A resolver gives a different result when the registry changes. Cargo "gives the highest priority to versions contained in the Cargo.lock file", to balance reproducible builds with changes in the manifest.[^4] Commit the lock file of an application.

## Cost

The general problem is NP-complete, so no known resolver is fast on every input.[^2] A backtracking resolver can try many combinations before it finds a solution, and pip must download the distribution files of a package to get its dependencies.[^3] Conflict learning cuts the repeated work on many real graphs.[^1] Minimal version selection is easy to compute: Cox describes it as efficient to implement "using nothing more complex than recursive graph traversals".[^8]

## History

In 2018 Russ Cox proposed minimal version selection for Go, in the post of February 21, 2018.[^8] In the same year Natalie Weizenbaum introduced PubGrub in a blog post, for the Dart package manager.[^5] pip got a backtracking resolver in version 20.3.[^3] In 2020 Abate and others reviewed the research on dependency solving and the solvers of the main package managers.[^2]

## References
