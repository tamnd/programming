---
tag: KFRA5C
kind: tool
slug: npm-pm
name: npm
aliases: [npm, npm CLI, node package manager, npx, package.json, package-lock.json, npm workspaces, npm scripts]
area: tools-and-workflow/package-managers
level: core
summary: >-
  npm is the package manager that comes with Node.js. This page shows package.json, package-lock.json, scripts, workspaces, npm ci, the configuration files and the history of the tool.
role: [package-manager]
licence: artistic-2.0
repo: https://github.com/npm/cli
homepage: https://www.npmjs.com/
see_also: [package-manager-concept, lock-files, version-constraints, package-registries, add-a-dependency, javascript, node-test]
sources:
  - url: https://docs.npmjs.com/about-npm
    title: npm Docs, About npm
    locator: the whole page
    accessed: 2026-11-16
  - url: https://github.com/npm/cli/blob/latest/README.md
    title: npm CLI, README
    locator: Installation
    accessed: 2026-11-16
  - url: https://github.com/npm/cli/blob/latest/LICENSE
    title: npm CLI, LICENSE
    locator: the first lines
    accessed: 2026-11-16
  - url: https://github.com/npm/cli/blob/latest/CHANGELOG.md
    title: npm CLI, CHANGELOG
    locator: 12.0.0 (2026-07-08); 12.2.2 (2026-11-03)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/configuring-npm/package-json
    title: npm Docs (v12), package.json
    locator: dependencies; devDependencies; private; workspaces
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/configuring-npm/package-lock-json
    title: npm Docs (v12), package-lock.json
    locator: Description; File Format (lockfileVersion, packages)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/commands/npm-install
    title: npm Docs (v12), npm install
    locator: Description; Configuration (save-exact, allow-scripts)
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/using-npm/scripts
    title: npm Docs (v12), scripts
    locator: Pre & Post Scripts; Life Cycle Scripts; path
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/commands/npm-ci
    title: npm Docs (v12), npm ci
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/using-npm/workspaces
    title: npm Docs (v12), workspaces
    locator: Description; Defining workspaces; Running commands in the context of workspaces
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/commands/npm-exec
    title: npm Docs (v12), npm exec
    locator: Description
    accessed: 2026-11-16
  - url: https://docs.npmjs.com/cli/v12/configuring-npm/npmrc
    title: npm Docs (v12), .npmrc
    locator: Files
    accessed: 2026-11-16
  - url: https://github.blog/news-insights/product-news/presenting-v7-0-0-of-the-npm-cli/
    title: GitHub Blog, Presenting v7.0.0 of the npm CLI (2020-10-13)
    locator: Exciting new features
    accessed: 2026-11-16
  - url: https://github.blog/news-insights/company-news/npm-is-joining-github/
    title: GitHub Blog, npm is joining GitHub (2020-03-16)
    locator: the whole post
    accessed: 2026-11-16
  - url: https://github.com/npm/cli/releases
    title: npm CLI, Releases
    locator: v9.0.0; v10.0.0; v11.0.0
    accessed: 2026-11-16
---

## What it is

npm is the package manager of [[javascript|JavaScript]] on Node.js. The name means three things: "the website", "the Command Line Interface (CLI)" and "the registry".[^1] This page is about the CLI, the command `npm`. The registry is "a large public database of JavaScript software and the meta-information surrounding it"; see [[package-registries]].[^1]

npm "comes bundled with" Node.js, so a computer with Node.js has npm too.[^2] Its licence is the Artistic License 2.0.[^3] The newest release on 2026-11-12 was npm 12.2.2, of 2026-11-03.[^4] The outputs on this page come from npm 11.6.1 on Node.js 24.10. npm 12 changed some defaults; see the table in the last section.

## Install and first use

A project is a directory with a file `package.json`, the manifest. `npm init` writes one, and `npm install <name>` adds a dependency to it; see [[add-a-dependency]]. This script makes a small project with two workspaces, `@demo/greet` and `@demo/app`, where the app depends on the library. The packages are local, so the run needs no network.

```bash run timeout=30 id=npm-first
d=$(mktemp -d)
cd "$d"
export npm_config_cache="$d/npm-cache"
mkdir -p demo/packages/greet demo/packages/app
cd demo
# The root package.json lists the workspaces and the scripts.
cat >package.json <<'END'
{
  "name": "demo",
  "version": "1.0.0",
  "private": true,
  "workspaces": ["packages/*"],
  "scripts": {
    "start": "node packages/app/index.js",
    "prestart": "echo before start"
  }
}
END
cat >packages/greet/package.json <<'END'
{ "name": "@demo/greet", "version": "1.0.0", "main": "index.js",
  "scripts": { "where": "echo in $npm_package_name" } }
END
echo 'exports.hello = (name) => `Hello, ${name}!`;' >packages/greet/index.js
cat >packages/app/package.json <<'END'
{ "name": "@demo/app", "version": "1.0.0",
  "dependencies": { "@demo/greet": "1.0.0" },
  "scripts": { "where": "echo in $npm_package_name" } }
END
echo 'console.log(require("@demo/greet").hello("workspace"));' >packages/app/index.js

# npm install links the workspaces into node_modules and writes package-lock.json.
npm install --offline --no-audit --no-fund --silent
readlink node_modules/@demo/greet
node -p 'const l = require("./package-lock.json"); l.lockfileVersion + "\n" + Object.keys(l.packages).map((k) => k || "(root)").join("\n")'

# Scripts: npm start runs prestart first. --workspaces runs a script in each workspace.
npm start --silent
npm run where --workspaces --silent

# npm ci deletes node_modules and installs exactly what the lock file says.
npm ci --offline --no-audit --no-fund --silent
npm ls --all | sed "s|$d|DIR|" | sed "/^$/d"
cd /
rm -rf "${d:?}"
```

```output for=npm-first
../../packages/greet
3
(root)
node_modules/@demo/app
node_modules/@demo/greet
packages/app
packages/greet
before start
Hello, workspace!
in @demo/app
in @demo/greet
demo@1.0.0 DIR/demo
├─┬ @demo/app@1.0.0 -> ./packages/app
│ └── @demo/greet@1.0.0 deduped -> ./packages/greet
└── @demo/greet@1.0.0 -> ./packages/greet
```

The output shows each part of npm that the next section explains: the link in `node_modules`, the lock file with `lockfileVersion` 3, the script `prestart` that runs before `start`, a script in each workspace, and the tree of `npm ls`.

## Concepts of the tool

**package.json.** The manifest gives the name, the version, the scripts and the dependencies. In `dependencies`, each entry "maps a package name to a version range"; see [[version-constraints]].[^5] Test and build tools go in `devDependencies`, because a user of the package does not need them.[^5] With `"private": true`, "npm will refuse to publish it".[^5]

**package-lock.json.** npm writes the lock file "for any operations where npm modifies either the node_modules tree, or package.json". It "describes the exact tree that was generated, such that subsequent installs are able to generate identical trees", and it "is intended to be committed into source repositories".[^6] Its key `packages` maps each location in the tree to the version, the source and the hash. `lockfileVersion` 1 is from npm 5 and 6, 2 from npm 7 and 8, and 3 from npm 9 and later.[^6] See [[lock-files]].

**node_modules.** `npm install` without arguments installs the dependencies into the folder `node_modules` of the project.[^7] If the lock file satisfies the ranges in `package.json`, npm installs the exact versions of the lock file; if not, `package.json` wins, and npm updates the lock file.[^7] The commands of the installed packages go to `node_modules/.bin`, and npm adds that directory to the `PATH` of the scripts.[^8]

**npm ci.** `npm ci` is for "automated environments such as test platforms, continuous integration, and deployment".[^9] It needs a lock file, it stops with an error if the lock file does not match `package.json`, it deletes `node_modules` first, and "It will never write to package.json or package-lock.json".[^9]

**Scripts.** `npm run <name>` runs a command from the object `scripts`.[^8] A script named `pre<name>` runs before it, and `post<name>` after it.[^8] `npm start` and `npm test` run the scripts `start` and `test`, with their pre and post scripts.[^8] The install-time life cycle scripts are `preinstall`, `install`, `postinstall`, and `prepare` for dependencies that do not come from a registry.[^7] In npm 12, the install scripts of dependencies are "blocked by default unless allowed by the root package's `allowScripts` policy".[^4][^7]

**Workspaces.** Workspaces manage "multiple packages from your local file system from within a singular top-level, root package".[^10] The field `workspaces` of the root `package.json` lists them, and `npm install` links each workspace into `node_modules`.[^10] `--workspace <name>` and `--workspaces` run a command in one or in each workspace.[^10]

**npm exec and npx.** `npm exec` runs "an arbitrary command from an npm package (either one installed locally, or fetched remotely)".[^11] `npx` runs the same code: "The npx binary was rewritten in npm v7.0.0", and it "uses the npm exec command".[^11]

## Common tasks

```text
npm init                          # write package.json
npm install                       # install what package.json and the lock file give
npm install lodash                # add a dependency
npm install --save-dev vitest     # add a development dependency
npm install --save-exact lodash   # record 1.2.3, not ^1.2.3
npm ci                            # clean install from the lock file, for CI
npm run build                     # run the script build
npm test                          # run the script test
npm ls --all                      # show the tree of installed packages
npm exec -- eslint .              # run the command of a package
```

`--save-exact` saves "an exact version rather than using npm's default semver range operator".[^7] See [[dependency-updates]] for updates, and [[publish-a-package]] for `npm pack` and `npm publish`.

## Configuration

npm reads its settings from the command line, from environment variables and from `npmrc` files.[^12] There are four files: the project file `.npmrc`, the user file `~/.npmrc`, the global file `$PREFIX/etc/npmrc` and the built-in file of npm.[^12] Each file is "an ini-formatted list of key = value parameters", and a value can use `${VARIABLE_NAME}`.[^12] "a setting in the userconfig file would override the setting in the globalconfig file".[^12]

```text
# .npmrc of a project
registry=https://npm.example.com/
@demo:registry=https://npm.pkg.github.com/
save-exact=true
//npm.example.com/:_authToken=${NPM_TOKEN}
```

The first line changes the registry, and the second gives a registry for one scope. The last line gives the secret for one registry host, and it reads the secret from an environment variable, not from the file.[^12] See [[package-registries]].

## Alternatives

pnpm and Yarn are two other package managers for Node.js. See [[pnpm-and-yarn]].

## History and versions

The announcement of npm 7 says that "just over 11 years ago the JavaScript community didn’t have npm".[^13] On 2020-03-16, GitHub announced that it "has signed an agreement to acquire npm", which then had "over 1.3 million packages with 75 billion downloads a month".[^14]

| Version | Date | Notes |
| --- | --- | --- |
| 7.0.0 | 2020-10-13 | Workspaces, automatic install of peer dependencies, lock file version 2[^13][^6] |
| 9.0.0 | 2022-10-19 | Lock file version 3; `npm pack` ignores `.gitignore` and `.npmignore` when `files` is present[^15][^6] |
| 10.0.0 | 2023-08-31 | Support for Node.js 14 and 16 is dropped[^15] |
| 11.0.0 | 2024-12-16 | `--ignore-scripts` applies to all life cycle scripts, `prepare` too[^15] |
| 12.0.0 | 2026-07-08 | Install scripts of dependencies are blocked by default; `npm-shrinkwrap.json` is no longer read; unknown command-line flags are errors[^4] |

## References
