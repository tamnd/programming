---
tag: 8YRV1V
kind: tool
slug: pnpm-and-yarn
name: pnpm and Yarn
aliases: [pnpm, Yarn, Yarn Berry, Yarn Classic, Plug'n'Play, Yarn PnP, pnpm-workspace.yaml, .pnp.cjs, Corepack]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  pnpm and Yarn are the main alternatives to npm for JavaScript. This page shows the content-addressable store of pnpm, its strict node_modules, Yarn Plug'n'Play, workspaces and Corepack.
role: [package-manager]
licence: mit
repo: https://github.com/pnpm/pnpm
homepage: https://pnpm.io/
see_also: [npm-pm, package-manager-concept, lock-files, version-constraints, add-a-dependency, dependency-resolution, package-registries, transitive-dependencies]
sources:
  - url: https://pnpm.io/motivation
    title: pnpm documentation, Motivation
    locator: Saving disk space; Boosting installation speed; Creating a non-flat node_modules directory
    accessed: 2026-11-16
  - url: https://github.com/pnpm/pnpm/blob/main/LICENSE
    title: pnpm, LICENSE
    locator: the first line
    accessed: 2026-11-16
  - url: https://registry.npmjs.org/pnpm
    title: npm registry, the metadata of the package pnpm
    locator: time; license
    accessed: 2026-11-16
  - url: https://yarnpkg.com/getting-started
    title: Yarn documentation, Introduction
    locator: the whole page
    accessed: 2026-11-16
  - url: https://yarnpkg.com/features/pnp
    title: Yarn documentation, Plug'n'Play
    locator: What is Yarn Plug'n'Play?; How does it work?; What are the advantages?
    accessed: 2026-11-16
  - url: https://github.com/yarnpkg/berry/blob/master/LICENSE.md
    title: yarnpkg/berry, LICENSE.md
    locator: the first line
    accessed: 2026-11-16
  - url: https://registry.npmjs.org/@yarnpkg/cli-dist
    title: npm registry, the metadata of the package @yarnpkg/cli-dist
    locator: time; dist-tags; license
    accessed: 2026-11-16
  - url: https://pnpm.io/feature-comparison
    title: pnpm documentation, Feature Comparison
    locator: the table
    accessed: 2026-11-16
  - url: https://yarnpkg.com/features/workspaces
    title: Yarn documentation, Workspaces
    locator: What are workspaces?; Cross-references
    accessed: 2026-11-16
  - url: https://pnpm.io/installation
    title: pnpm documentation, Installation
    locator: Prerequisites; Using pnpm; Using a standalone script
    accessed: 2026-11-16
  - url: https://yarnpkg.com/getting-started/install
    title: Yarn documentation, Installation
    locator: the introduction; Updating Yarn
    accessed: 2026-11-16
  - url: https://github.com/nodejs/corepack
    title: nodejs/corepack, README
    locator: the introduction; Default Installs
    accessed: 2026-11-16
  - url: https://pnpm.io/cli/store
    title: pnpm documentation, pnpm store
    locator: status; add; prune
    accessed: 2026-11-16
  - url: https://pnpm.io/symlinked-node-modules-structure
    title: pnpm documentation, Symlinked node_modules structure
    locator: the whole page
    accessed: 2026-11-16
  - url: https://yarnpkg.com/configuration/yarnrc
    title: Yarn documentation, Settings (.yarnrc.yml)
    locator: nodeLinker; nmMode; npmMinimalAgeGate
    accessed: 2026-11-16
  - url: https://yarnpkg.com/features/caching
    title: Yarn documentation, Offline Cache
    locator: Offline mirror; Zero-installs
    accessed: 2026-11-16
  - url: https://pnpm.io/workspaces
    title: pnpm documentation, Workspace
    locator: Declaring a workspace; Workspace protocol (workspace:)
    accessed: 2026-11-16
  - url: https://pnpm.io/settings
    title: pnpm documentation, Settings (pnpm-workspace.yaml)
    locator: the introduction; the list of settings
    accessed: 2026-11-16
  - url: https://registry.npmjs.org/yarn
    title: npm registry, the metadata of the package yarn
    locator: time; dist-tags; license
    accessed: 2026-11-16
---

## What it is

pnpm and Yarn are package managers for JavaScript. They read the same `package.json` as npm and install packages from the same npm registry, but they put the packages on the disk in other ways; see [[npm-pm]].

- **pnpm** keeps one copy of each package file in a content-addressable store. "When packages are installed, their files are hard-linked or cloned (copy-on-write) from that single place, consuming no additional disk space."[^1] The licence of pnpm is the MIT License.[^2][^3]
- **Yarn** "is an established open-source package manager used to manage dependencies in JavaScript projects."[^4] Its default way to install is Plug'n'Play, which writes a loader file in place of the `node_modules` folder.[^5] The licence of Yarn is the BSD 2-Clause License.[^6][^7]

Both tools have a lock file and workspaces for monorepos.[^8][^9] The lock file of pnpm is `pnpm-lock.yaml`.[^8] See [[lock-files]] and [[monorepos]].

## Install and first use

pnpm 12 "is a native executable and does not require Node.js after it is installed."[^10] On POSIX systems, the documentation gives a standalone script:[^10]

```text
curl -fsSL https://get.pnpm.io/install.sh | sh -
```

The pnpm npm package also "installs on Node.js 18 or newer", and `pnpm self-update` updates an installed pnpm.[^10]

For Yarn, the documentation says: "Install Corepack, an intermediary tool that will let you configure your package manager version on a per-project basis".[^11] Then `yarn init -2` makes a new project, and `yarn set version stable` moves a project to the newest stable Yarn.[^11] "Corepack is distributed with Node.js from version 14.19.0 up to (but not including) 25.0.0."[^12] With Node.js 25 or later, install it with `npm install -g corepack`.[^11][^12]

This script shows the store of pnpm. It packs two local packages, `words` and `greet`, where `greet` depends on `words`. Then two projects install `greet` with one shared store. The packages are local tarballs, so the run needs no network. The output comes from pnpm 10.28.2.

```bash run timeout=30 id=pnpm-first
d=$(mktemp -d)
cd "$d"
export XDG_CACHE_HOME="$d/cache" XDG_DATA_HOME="$d/data" XDG_STATE_HOME="$d/state" XDG_CONFIG_HOME="$d/config"
mkdir words greet app1 app2
# Two local packages: greet depends on words.
printf '{ "name": "words", "version": "1.0.0", "main": "index.js" }\n' > words/package.json
printf 'module.exports = { hello: "Hello" };\n' > words/index.js
(cd words && pnpm pack --pack-destination "$d" >/dev/null)
printf '{ "name": "greet", "version": "1.0.0", "main": "index.js", "dependencies": { "words": "file:%s/words-1.0.0.tgz" } }\n' "$d" > greet/package.json
printf 'const w = require("words");\nmodule.exports = (n) => w.hello + ", " + n + "!";\n' > greet/index.js
(cd greet && pnpm pack --pack-destination "$d" >/dev/null)
# Two projects install greet with one shared store.
for a in app1 app2; do
  cd "$d/$a"
  printf '{ "name": "%s", "version": "1.0.0", "private": true }\n' "$a" > package.json
  printf '%s: ' "$a"
  pnpm add "$d/greet-1.0.0.tgz" --store-dir "$d/store" --offline 2>&1 | grep 'done$'
done
cd "$d/app1"
echo "node_modules: $(ls node_modules | paste -sd ' ' -)"
echo "node_modules/.pnpm: $(ls node_modules/.pnpm | grep -v '^node_modules$' | paste -sd ' ' -)"
node -e 'console.log(require("greet")("pnpm"))'
node -e 'try { require("words") } catch (e) { console.log("require(\"words\"):", e.code) }'
f=$(find node_modules/.pnpm -path '*node_modules/greet/index.js')
echo "hard links to greet/index.js: $(stat -c '%h' "$f")"
cd /
rm -rf "${d:?}"
```

```output for=pnpm-first
app1: Progress: resolved 2, reused 0, downloaded 2, added 2, done
app2: Progress: resolved 2, reused 2, downloaded 0, added 2, done
node_modules: greet
node_modules/.pnpm: greet@file+..+greet-1.0.0.tgz lock.yaml words@file+..+words-1.0.0.tgz
Hello, pnpm!
require("words"): MODULE_NOT_FOUND
hard links to greet/index.js: 3
```

The first project downloads the two packages into the store. The second project reuses them and downloads nothing. The file `greet/index.js` has three hard links: one in the store and one in each project. The project can load `greet`, but it cannot load `words`, because `words` is not a direct dependency of the project.

## Concepts of the tool

**The content-addressable store of pnpm.** With npm, "if you have 100 projects using a dependency, you will have 100 copies of that dependency saved on disk."[^1] pnpm stores each file once. "If you depend on different versions of the dependency, only the files that differ are added to the store."[^1] `pnpm store prune` "Removes unreferenced packages from the store."[^13]

**The strict node_modules of pnpm.** "When installing dependencies with npm or Yarn Classic, all packages are hoisted to the root of the modules directory. As a result, source code has access to dependencies that are not added as dependencies to the project."[^1] "By default, pnpm uses symlinks to add only the direct dependencies of the project into the root of the modules directory."[^1] The real files are in `node_modules/.pnpm`, and "Every file of every package inside node_modules is a hard link to the content-addressable store."[^14] For the example of the documentation, where `foo` depends on `bar`, the layout is:[^14]

```text
node_modules
├── foo -> ./.pnpm/foo@1.0.0/node_modules/foo
└── .pnpm
    ├── bar@1.0.0
    │   └── node_modules
    │       └── bar/ (files linked from <store>)
    └── foo@1.0.0
        └── node_modules
            ├── foo/ (files linked from <store>)
            └── bar -> ../../bar@1.0.0/node_modules/bar
```

Some tools do not work with symlinks. For them, the setting `nodeLinker: hoisted` makes "a node_modules directory that is similar to those created by npm and Yarn Classic."[^1]

**Yarn Plug'n'Play.** "Yarn Plug'n'Play (generally referred to as Yarn PnP) is the default installation strategy in modern releases of Yarn."[^5] Yarn writes "a single Node.js loader file in place of the typical node_modules folder."[^5] The file `.pnp.cjs` "contains all information about your project's dependency tree".[^5] Because Yarn knows all the packages and their dependencies, "it can prevent accesses to dependencies unaccounted for during resolution".[^5] The documentation calls this "Ghost dependencies protection".[^5]

**The linkers of Yarn.** The setting `nodeLinker` has three values: `pnp` for the loader file, `pnpm` for "a node-modules will be created using symlinks and hardlinks to a global content-addressable store", and `node-modules` for a regular `node_modules` folder.[^15]

**Zero-installs.** Yarn can keep its package cache in the repository, by default in `.yarn/cache`.[^16] "As long as your project uses Yarn PnP and the offline mirror, all you have to do is add the loader files to Git, and you can forget yarn install most of the time."[^16]

**Workspaces.** pnpm needs "a pnpm-workspace.yaml file in its root", and its `packages` field lists the directories of the projects.[^17] Yarn uses the `workspaces` field of the root `package.json`, for example `"workspaces": ["packages/*"]`.[^9] Both tools have the `workspace:` protocol. With it, pnpm "will refuse to resolve to anything other than a local workspace package."[^17] In Yarn, `"@my-org/utils": "workspace:^"` resolves the dependency to the workspace of that name.[^9]

## Common tasks

| Task | npm | pnpm | Yarn |
|---|---|---|---|
| Install from the lock file | `npm install` | `pnpm install` | `yarn install` |
| Add a dependency | `npm install lodash` | `pnpm add lodash` | `yarn add lodash` |
| Add a development dependency | `npm install -D vitest` | `pnpm add -D vitest` | `yarn add -D vitest` |
| Remove a dependency | `npm uninstall lodash` | `pnpm remove lodash` | `yarn remove lodash` |
| Run a script | `npm run test` | `pnpm run test` | `yarn run test` |
| Run a package once | `npx cowsay` | `pnpm dlx cowsay` | `yarn dlx cowsay` |

**Install for CI.** Install from the lock file and fail if the lock file is not up to date: `pnpm install --frozen-lockfile`, and `yarn install --immutable` for Yarn. See [[lock-files]].

**Prepare an offline install.** `pnpm store add ./tarballs` puts tarballs in the store, and then `pnpm install --offline --frozen-lockfile` installs "without network access".[^13]

## Configuration

pnpm reads its settings "from the command line, environment variables, and pnpm-workspace.yaml".[^18] "Only auth, registry, TLS, proxy, and local-address settings are read from .npmrc files."[^18] The settings include `nodeLinker` and `minimumReleaseAge`.[^18]

Yarn reads its settings from files that "must be called .yarnrc.yml".[^15] The setting `npmMinimalAgeGate` gives the "Minimum age of a package version according to the publish date on the npm registry to be considered for installation."[^15] With `nmMode: hardlinks-global` and `nodeLinker: node-modules`, Yarn uses "hardlinks to a global content-addressable store".[^15]

## Alternatives

npm is the package manager that comes with Node.js; see [[npm-pm]]. The feature comparison of pnpm lists features that pnpm has and npm does not have, for example "Content-addressable storage", "Plug'n'Play" and "Managing versions of itself".[^8]

## History and versions

The dates of the major versions of pnpm on npm:[^3]

| Version | Released |
|---|---|
| 1.0.0 | 2017-06-28 |
| 5.0.0 | 2020-05-27 |
| 8.0.0 | 2023-03-27 |
| 9.0.0 | 2024-04-16 |
| 10.0.0 | 2025-01-07 |
| 11.0.0 | 2026-04-28 |
| 12.0.0 | 2026-08-26 |

"pnpm 12 is the current release line."[^10]

Yarn 1.0.0 of the package `yarn` was released on 2017-09-05, and the last version of this line is 1.22.22, from 2024-03-09.[^19] The newer Yarn is published as `@yarnpkg/cli-dist`: Yarn 3.0.0 was released on 2021-07-26, Yarn 4.0.0 on 2023-10-22, and version 4.18.1 on 2026-09-24.[^7]

## References
