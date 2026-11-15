---
tag: JKW8M0
kind: tool
slug: vite
name: Vite
aliases: [Vite, vite build, vite preview, vite.config.js, create-vite, dependency pre-bundling, Rolldown]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Vite is a build tool for the web. Its dev server serves the source as native ES modules and pre-bundles the dependencies, and its build command bundles the application with Rolldown for production.
role: [build]
licence: mit
repo: https://github.com/vitejs/vite
homepage: https://vite.dev/
see_also: [javascript-bundlers, webpack, javascript, typescript, build-system-concept, make, incremental-builds, build-graph]
sources:
  - url: https://vite.dev/guide/
    title: Vite documentation, Getting Started
    locator: Overview; Browser Support; Scaffolding Your First Vite Project; Manual Installation; index.html and Project Root; Command Line Interface
  - url: https://vite.dev/guide/why
    title: Vite documentation, Why Vite
    locator: The Origins; Growing with the Ecosystem; A Unified Toolchain
  - url: https://github.com/vitejs/vite/blob/main/LICENSE
    title: Vite, LICENSE (MIT)
    locator: the first lines
  - url: https://vite.dev/config/build-options
    title: Vite documentation, Build Options
    locator: build.modulePreload; build.sourcemap; build.outDir
  - url: https://vite.dev/guide/build
    title: Vite documentation, Building for Production
    locator: the introduction; Browser Compatibility; Customizing the Build; Chunking Strategy
  - url: https://vite.dev/guide/features
    title: Vite documentation, Features
    locator: npm Dependency Resolving and Pre-Bundling; Hot Module Replacement; TypeScript (Transpile Only); Preload Directives Generation
  - url: https://vite.dev/guide/dep-pre-bundling
    title: Vite documentation, Dependency Pre-Bundling
    locator: The Why
  - url: https://vite.dev/guide/env-and-mode
    title: Vite documentation, Env Variables and Modes
    locator: Env Files; Modes
  - url: https://vite.dev/guide/cli
    title: Vite documentation, Command Line Interface
    locator: vite build; vite preview
  - url: https://vite.dev/config/
    title: Vite documentation, Configuring Vite
    locator: Config File
  - url: https://registry.npmjs.org/vite
    title: npm registry, package vite
    locator: the time field (release dates of 0.1.0, 2.0.0 and the first release of each major version to 8.0.0)
  - url: https://vite.dev/releases
    title: Vite documentation, Releases
    locator: Supported Versions
---

## What it is

Vite, the French word for "quick", is a build tool for web projects. It has two major parts: a dev server that serves the source code over native ES modules, with fast Hot Module Replacement (HMR), and a build command that bundles the code with Rolldown into static files for production.[^1] Vite comes with defaults for common projects, and plugins add support for frameworks and other tools.[^1]

So Vite uses two methods. During development, it does not bundle the source code of the application: the browser loads each module with a native `import`, and Vite transforms each file when the browser asks for it.[^2] For production, Vite bundles the code, because unbundled modules are "still inefficient" in production: the nested imports cause more network round trips.[^2] The page [[javascript-bundlers]] explains bundles, tree shaking and code splitting in general.

Vite needs Node.js 20.19 or later, or 22.12 or later.[^1] It is free software under the MIT License, "Copyright (c) 2019-present, VoidZero Inc. and Vite contributors".[^3]

## Install and first use

`npm create vite@latest` makes a project from a template, such as `vanilla`, `vue`, `react` or `svelte`, or the same template with TypeScript, such as `react-ts`.[^1] A project can also install Vite by hand with `npm install -D vite`. Then `npx vite` starts the dev server, and the page is at `http://localhost:5173`.[^1] A project made from a template has three npm scripts:[^1]

| Script | Command | What it does |
|---|---|---|
| `dev` | `vite` | start the dev server |
| `build` | `vite build` | build for production |
| `preview` | `vite preview` | serve the production build on the local computer |

In a Vite project, `index.html` is at the root of the project, and it is the entry point of the application. Vite treats it as source code and as a part of the module graph: it follows each `<script type="module" src="...">` in it.[^1]

The build machine of this site has no Vite, so the script below does not run when the site is built. It ran once on the build machine, in a temporary directory, with Vite 8.3.8 installed from npm, on Node.js 24.10.0. The output after it is the recorded output of that run. The script writes a page with one module script, two modules and a configuration file, and makes a production build. The configuration turns off the module preload polyfill, a small piece of code that Vite adds to each HTML entry by default, so that the output shows only the code of the application.[^4] It also turns on source maps.[^4] Then the script shows the files that the build made. The `sed` removes the color codes from the output of `vite build`, and the `grep` removes the line with the time of the build.

```text
# Build a page with two modules for production with Vite.
# vite must be installed in node_modules.
cat > index.html <<'HTML'
<!doctype html>
<html>
  <body>
    <p id="out"></p>
    <script type="module" src="/main.js"></script>
  </body>
</html>
HTML
cat > math.js <<'JS'
export function square(x) {
  return x * x;
}

export function cube(x) {
  return x * x * x;
}
JS
cat > main.js <<'JS'
import { cube } from './math.js';

document.querySelector('#out').textContent = `5 cubed is ${cube(5)}`;
JS
cat > vite.config.mjs <<'JS'
export default {
  build: {
    modulePreload: { polyfill: false },
    sourcemap: true,
  },
};
JS
npx vite build 2>&1 | sed -e 's/\x1b\[[0-9;]*m//g' | grep -v -E 'built in'
echo "--- the files"
find dist -type f | sort
echo "--- dist/index.html"
cat dist/index.html
echo "--- the JavaScript"
cat dist/assets/*.js; echo
echo "--- the source map"
node -e 'const fs = require("fs"); const f = fs.readdirSync("dist/assets").find((n) => n.endsWith(".map")); const m = JSON.parse(fs.readFileSync("dist/assets/" + f)); console.log("version:", m.version); console.log("sources:", m.sources.join(", ")); console.log("names:", m.names.join(", "));'
```

The output of that run:

```text
vite v8.3.8 building client environment for production...
transforming...
✓ 4 modules transformed.
rendering chunks...
computing gzip size...
dist/index.html                0.15 kB │ gzip: 0.13 kB
dist/assets/index-BG0i8z4i.js  0.13 kB │ gzip: 0.14 kB │ map: 0.45 kB

--- the files
dist/assets/index-BG0i8z4i.js
dist/assets/index-BG0i8z4i.js.map
dist/index.html
--- dist/index.html
<!doctype html>
<html>
  <script type="module" crossorigin src="/assets/index-BG0i8z4i.js"></script>

  <body>
    <p id="out"></p>
  </body>
</html>
--- the JavaScript
function e(e){return e*e*e}document.querySelector(`#out`).textContent=`5 cubed is ${e(5)}`;
//# sourceMappingURL=index-BG0i8z4i.js.map
--- the source map
version: 3
sources: ../../math.js, ../../main.js
names: cube, x
```

`vite build` used `index.html` as the entry point, the default, and wrote the output to `dist`, the default output directory.[^5][^4] In `dist/index.html`, the script tag points to the bundle, `index-BG0i8z4i.js`. The bundle has the code of `main.js` and of `cube` only: the function `square` is not used, so the build dropped it. The source map lists the two source files, and its `names` are the original names, `cube` and `x`, that the minifier changed to `e`.

## Concepts of the tool

**Native ES modules in development.** When Vite was first made, browsers had started to give wide support to ES modules. A bundler processes the whole application before the browser can show anything, so with a large application, the start of a bundle-based dev server is slow. Vite splits the work into two parts. It pre-bundles the dependencies, which seldom change, once, and it serves the source code, which changes often, on demand over native ESM.[^2] So the start of the dev server does not depend on the size of the application.[^2] The documentation names earlier tools with this idea: Snowpack, WMR of the Preact team, and `@web/dev-server`.[^2]

**Dependency pre-bundling.** A browser cannot load a bare import, such as `import { someMethod } from 'my-dep'`. Vite finds such imports, pre-bundles the dependencies with Rolldown, and changes the imports to URLs such as `/node_modules/.vite/deps/my-dep.js`.[^6] Pre-bundling has two purposes. First, compatibility: Vite serves all code as native ESM, so it must convert the dependencies in CommonJS or UMD format to ESM. Second, performance: some packages, such as `lodash-es` with over 600 internal modules, would make the browser send over 600 HTTP requests at the same time, and pre-bundling makes one module of them.[^7]

**Hot Module Replacement.** Vite has an HMR API over native ESM. Frameworks use it to update a module in the page without a reload and without a loss of the state of the application. Vite has HMR integrations for Vue single-file components and React Fast Refresh.[^6]

**TypeScript.** Vite can import `.ts` files, but it only transpiles them and does not check the types. The documentation explains that transpilation works on one file at a time, which fits the on-demand model of Vite, but type checking needs the whole module graph. The IDE and the build process must check the types.[^6]

**The production build.** `vite build` uses `<root>/index.html` as the entry point by default, and it makes a bundle for a static hosting service.[^5] The option `build.rolldownOptions` sets the options of Rolldown directly, and `build.rolldownOptions.output.codeSplitting` controls how the build splits the code into chunks.[^5] Vite also writes `<link rel="modulepreload">` directives for the entry chunks and their direct imports into the built HTML.[^6]

**Browser targets.** During development, Vite assumes a browser that "supports most of the latest JavaScript and CSS features", and it sets the transform target `esnext`.[^1] For production, the default target of Vite 8 is the browsers of Baseline Widely Available: Chrome 111, Edge 111, Firefox 114 and Safari 16.4 or later. The option `build.target` can set a lower target, down to `es2015`.[^5] By default, Vite transforms only the syntax and does not add polyfills. The plugin `@vitejs/plugin-legacy` supports legacy browsers.[^5]

**Modes and environment variables.** The dev server runs in the mode `development`, and `vite build` runs in the mode `production`. So `vite build` loads the variables in `.env.production`, if the file exists, and the option `--mode` selects another mode, such as `staging` with `.env.staging`.[^8] Only variables with the prefix `VITE_` go into the code for the client, such as `import.meta.env.VITE_SOME_KEY`, and a variable such as `DB_PASSWORD` stays out of the code for the client.[^8]

## Common tasks

| Task | Command |
|---|---|
| Make a project from a template | `npm create vite@latest`[^1] |
| Make a Vue project with a given name | `npm create vite@latest my-vue-app -- --template vue`[^1] |
| Start the dev server | `npx vite`[^1] |
| Build for production | `npx vite build`[^9] |
| Build with the variables of another mode | `vite build --mode staging`[^8] |
| Serve the production build on the local computer | `npx vite preview`[^9] |

`vite preview` starts a server in the build directory, `dist` by default. The documentation says that it is not a production server.[^9]

## Configuration

When `vite` runs, it looks for a file `vite.config.js` in the root of the project; other JavaScript and TypeScript extensions also work. The option `--config` gives another file.[^10] The configuration exports an object. To use ES module syntax, the file must be an ES module for Node.js, such as an `.mjs` file, or a `.js` file in a package with `"type": "module"`.[^10] For this reason, the script above uses `vite.config.mjs`.

The options of the build are in `build`. Some of them are:[^4]

| Option | Default | What it does |
|---|---|---|
| `build.outDir` | `dist` | the output directory, relative to the root of the project |
| `build.sourcemap` | `false` | `true` writes separate source map files, `'inline'` puts the map into the output file |
| `build.modulePreload` | `{ polyfill: true }` | adds the module preload polyfill; `{ polyfill: false }` turns it off |

## Alternatives

- [[webpack]]: a bundler with loaders and plugins that bundles the application in development too.

The page [[javascript-bundlers]] compares the work of bundlers in general.

## History and versions

The npm registry gives these release dates:[^11]

| Version | Release date |
|---|---|
| 0.1.0 | 21 April 2020 |
| 1.0.0-rc.1 | 17 July 2020 |
| 2.0.0 | 16 February 2021 |
| 3.0.0 | 13 July 2022 |
| 4.0.0 | 9 December 2022 |
| 5.0.0 | 16 November 2023 |
| 6.0.0 | 26 November 2024 |
| 7.0.0 | 24 June 2025 |
| 8.0.0 | 12 March 2026 |

At first, Vite used two tools: esbuild for fast compilation during development, and Rollup for the optimization of production builds. The documentation says that the two pipelines gave "different transformation behaviors, separate plugin systems, and growing glue code". Rolldown, a bundler in Rust that is compatible with the plugin API of Rollup, takes the place of both, and it uses Oxc to parse, transform and minify the code.[^2] The plugin API of Vite is based on the conventions of Rollup, and frameworks such as Nuxt, SvelteKit and Astro build on Vite.[^2]

When this page was written, the Releases page said that regular patches were released for `vite@8.3`, that important fixes and security patches went to `vite@7.3` and `vite@8.2`, and that security patches also went to `vite@6.4` and `vite@8.1`.[^12] This page uses Vite 8.3.8.

## References
