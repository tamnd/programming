# Programming

[![CI](https://github.com/tamnd/programming/actions/workflows/ci.yml/badge.svg)](https://github.com/tamnd/programming/actions/workflows/ci.yml)
[![Site](https://img.shields.io/badge/site-programming.tamnd.com-2b59c3)](https://programming.tamnd.com)
[![REUSE](https://img.shields.io/badge/REUSE-3.3-green)](https://reuse.software)

This repository is the content of [programming.tamnd.com](https://programming.tamnd.com). The site is a reference for programming. It has pages for concepts, language features, languages and their releases, tasks, patterns, tools, formats, standards, weaknesses, compiler diagnostics and the people and works of the field. Each page explains one thing in plain words. Each code example runs in a pinned toolchain, and the output on the page is the output of that run. Each fact names its source.

The toolchain `prog` is in [tamnd/programming-agent](https://github.com/tamnd/programming-agent). The plan of the project is in the specification, which the site publishes under `/spec/`.

## What a node is

Each page comes from one directory under `nodes/`:

```text
nodes/functions-and-abstraction/closures-and-scope/closures/
  node.md                 front matter and prose
  examples/<language>.md  a short example for each language, with its output
  impl/<language>/        for a task: the code and its tests
  viz/scene.ts            a visualization, if the node has one
```

There are 22 kinds of node. The main kinds are concept, feature, language, release, task, pattern, technique, practice, tool, library, format, standard, weakness and diagnostic. The kind sets the sections of the page. A feature, a release or a diagnostic belongs to one language or tool, so its slug has the owner in front, for example `python/match-statement` or `rust/errors/E0382`.

The language guides are under `nodes/languages/<language>/`. Each guide has the same 25 parts, from "Getting started" to "Versions and migration". `topics/languages.yaml` lists them.

## Work on the content

1. Install the toolchain: `go install github.com/tamnd/programming-agent/cmd/prog@latest`.
2. Make a node: `prog new concept closures -area functions-and-abstraction/closures-and-scope`.
3. Make a node that has an owner: `prog new feature match-statement -owner python -area languages/python/control-flow`.
4. Put the files in canonical form: `prog fmt`.
5. Check your work: `prog check -changed origin/main`.
6. Run the tests of a task: `prog test -unsafe-local <slug>`. Read the code first, because this runs it on your computer.
7. Look at the page: `prog site serve`, then open http://localhost:8150/closures/.
8. Before the merge, give the node its permanent tag: `prog mint`.

Read [CONTRIBUTING.md](CONTRIBUTING.md) before you open a pull request.

## How the site is built

Each push to `main` runs the CI workflow. It runs the audit and the tests of each language in its own job, renders the site, and publishes the test results to the release `ci-results`. A build server deploys `main` to Cloudflare with `scripts/deploy-cloudflare.sh`: a cron job runs it every 5 minutes, and it deploys only when `main`, the pinned `prog` or the test results changed. So a merge reaches the site in minutes and does not wait for the tests. A pull request from this repository gets a preview site from the CI. To deploy production from the CI, run the CI workflow by hand on `main`.

## Roadmap

The work is in six milestones, M0 to M5, from November 2026 to December 2028.

| Milestone | Goal |
| --- | --- |
| M0 | Make the system work end to end on a gold set of 17 nodes, with every example run and the site live. |
| M1 | Harvest the sources into cards, give every item of the universe an outcome, and ship search. |
| M2 | Write the 400 core concepts and the tier 1 language guides. |
| M3 | Add the core tasks in all tier 1 and tier 2 languages, the patterns, the security area, the tools and the tier 2 guides. |
| M4 | Map each section of each tier 1 reference, and add the full standard libraries as cards. |
| M5 | Close each scope of the universe with no open items. |

The [milestones](https://github.com/tamnd/programming/milestones) list the open issues. Issues with the label `good first issue` are small and clear.

## Licence

| Part | Licence |
| --- | --- |
| Prose | [CC BY-SA 4.0](LICENSES/CC-BY-SA-4.0.txt) |
| Code, examples, tests and scenes | [MIT](LICENSES/MIT.txt) |
| Metadata | [CC0 1.0](LICENSES/CC0-1.0.txt) |

[REUSE.toml](REUSE.toml) gives the licence of each path.
