---
tag: NMZZNR
kind: tool
slug: esbuild-and-other-bundlers
name: esbuild and other bundlers
aliases: [esbuild, Rollup, Rolldown, Parcel, Rspack, Turbopack, JavaScript bundler in Go, JavaScript bundler in Rust, rollup.config.js]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  esbuild is a bundler for JavaScript, CSS and TypeScript, written in Go. The page compares it with Rollup, Rolldown, Parcel, Rspack and Turbopack.
role: [build]
licence: mit
repo: https://github.com/evanw/esbuild
homepage: https://esbuild.github.io/
see_also: [javascript-bundlers, webpack, vite, javascript, typescript, build-system-concept, incremental-builds, build-graph]
sources:
  - url: https://esbuild.github.io/
    title: esbuild documentation, home page
    locator: Major features
  - url: https://esbuild.github.io/faq/
    title: esbuild documentation, FAQ
    locator: Why is esbuild fast?; Benchmark details; Production readiness
  - url: https://github.com/evanw/esbuild/blob/main/LICENSE.md
    title: esbuild, LICENSE.md (MIT)
    locator: the first lines
  - url: https://esbuild.github.io/getting-started/
    title: esbuild documentation, Getting Started
    locator: Install esbuild; Your first bundle; Build scripts; Additional npm flags
  - url: https://esbuild.github.io/api/
    title: esbuild documentation, API
    locator: Overview; Build; Transform; Bundle; Platform; Splitting; Target; Minify; Sourcemap; Tree shaking; Watch; Serve
  - url: https://esbuild.github.io/content-types/
    title: esbuild documentation, Content Types
    locator: the introduction (loaders); TypeScript caveats
  - url: https://rollupjs.org/introduction/
    title: Rollup documentation, Introduction
    locator: Overview; Tree-Shaking; Publishing ES Modules
  - url: https://rollupjs.org/command-line-interface/
    title: Rollup documentation, Command Line Interface
    locator: Command line flags (-f, --format; --silent)
  - url: https://github.com/rollup/rollup/blob/master/LICENSE.md
    title: Rollup, LICENSE.md (MIT)
    locator: the first lines
  - url: https://rolldown.rs/guide/introduction
    title: Rolldown documentation, Introduction
    locator: What is a Bundler; Why Rolldown
  - url: https://github.com/rolldown/rolldown
    title: Rolldown, README.md
    locator: Licenses
  - url: https://parceljs.org/
    title: Parcel, home page
    locator: the introduction; Zero config
  - url: https://parceljs.org/getting-started/webapp/
    title: Parcel documentation, Building a web app with Parcel
    locator: Installation; Project setup
  - url: https://rspack.rs/guide/start/introduction
    title: Rspack documentation, Introduction
    locator: Why Rspack?
  - url: https://nextjs.org/docs/app/api-reference/turbopack
    title: Next.js documentation, Turbopack
    locator: the introduction; Why Turbopack?; Supported platforms
  - url: https://registry.npmjs.org/esbuild
    title: npm registry, package esbuild
    locator: the time field (the first releases in April 2020; 0.28.2)
  - url: https://registry.npmjs.org/rollup
    title: npm registry, package rollup
    locator: the time field (0.1.0, 1.0.0, 4.0.0 and 4.68.3)
  - url: https://registry.npmjs.org/parcel
    title: npm registry, package parcel
    locator: the time field (2.0.0)
  - url: https://registry.npmjs.org/@rspack/core
    title: npm registry, package @rspack/core
    locator: the time field (1.0.0 and 2.0.0)
  - url: https://registry.npmjs.org/rolldown
    title: npm registry, package rolldown
    locator: the time field (1.0.0)
---

## What it is

esbuild is a bundler for the web. It reads JavaScript, CSS, TypeScript and JSX, follows the imports, and writes bundles. Its documentation lists these major features: speed without the need of a cache, built-in support for JavaScript, CSS, TypeScript and JSX, an API for the CLI, JavaScript and Go, bundles of ESM and CommonJS modules, tree shaking, minification and source maps, and a local server, watch mode and plugins.[^1] esbuild is written in Go and compiles to native code.[^2] It is under the MIT licence.[^3]

This page also compares other bundlers that are not covered on their own pages: Rollup, Rolldown, Parcel, Rspack and Turbopack. For the ideas that all bundlers share, such as the module graph, tree shaking and code splitting, see [[javascript-bundlers]]. [[webpack]] and [[vite]] have their own pages.

## Install and first use

The documentation installs esbuild from npm in the project, as a development dependency with an exact version: `npm install --save-exact --save-dev esbuild`.[^4] The package installs a prebuilt native executable, and `./node_modules/.bin/esbuild --version` checks it.[^4] To bundle means to put the imported dependencies in the file itself, and to do it again for their dependencies. esbuild does not bundle by default: the option `--bundle` turns bundling on.[^5]

The build machine of this site has no esbuild, so the scripts below do not run when the site is built. They ran once on the build machine, in a temporary directory, with esbuild 0.28.2 and Rollup 4.68.3 installed from npm, on Node.js 24.10.0. The install used the npm option `--ignore-scripts`. With this option, npm still installs the esbuild executable for the platform, but the command in `node_modules/.bin` stays a JavaScript shim, which starts one more Node.js process.[^4] The output after each script is the recorded output of that run.

The first script writes two modules. `math.js` exports `square` and `cube`, and `index.js` imports `cube` and prints a line. Then it makes two bundles. The first is for a browser, in the `iife` format. The second is minified, with a source map, and the script runs it with Node.js.

```text
# Bundle two modules with esbuild, in two output formats.
# esbuild must be installed in node_modules.
mkdir -p src
cat > src/math.js <<'JS'
export function square(x) {
  return x * x;
}

export function cube(x) {
  return x * x * x;
}
JS
cat > src/index.js <<'JS'
import { cube } from './math.js';

console.log('5 cubed is', cube(5));
JS
npx esbuild --version
echo "--- an IIFE for a browser"
npx esbuild src/index.js --bundle --format=iife --outfile=out/app.js --log-level=warning
cat out/app.js
echo "--- the same, minified, with a source map"
npx esbuild src/index.js --bundle --minify --sourcemap --outfile=out/app.min.js --log-level=warning
cat out/app.min.js
echo "--- run the bundle"
node out/app.min.js
```

```text
0.28.2
--- an IIFE for a browser
(() => {
  // src/math.js
  function cube(x) {
    return x * x * x;
  }

  // src/index.js
  console.log("5 cubed is", cube(5));
})();
--- the same, minified, with a source map
(()=>{function o(e){return e*e*e}console.log("5 cubed is",o(5));})();
//# sourceMappingURL=app.min.js.map
--- run the bundle
5 cubed is 125
```

The bundle has the code of both modules, with a comment that names each source file. The function `square` is not used, so tree shaking removed it. The `iife` format wraps the code in an immediately-invoked function expression, so that its variables do not leak into the global scope.[^5] The minified bundle renames `cube` to `o` and ends with a comment that points to its source map.

## Concepts of the tool

**Build and transform.** The two APIs that are used most are build and transform.[^5] Build takes one or more entry point files and options, and writes the results to the file system.[^5] Transform works on one string of code, with no access to other files; common uses are to minify code and to turn TypeScript into JavaScript.[^5]

**Loaders.** Each content type has a loader, which tells esbuild how to read the contents of a file. Some file extensions have a loader by default.[^6] esbuild removes the types of TypeScript, but it does not check them, so the documentation tells you to run `tsc -noEmit` in parallel to check the types.[^6]

**Platform and format.** By default, esbuild makes code for the browser, and `--platform=node` makes code for Node.js.[^5] For the browser platform, the default output format of a bundle is `iife`.[^5]

**Target.** The option `--target` names the environments that the code must run in, such as a version of Chrome, and esbuild changes the JavaScript syntax that those environments do not support into older syntax.[^5] For example, it changes the `??` operator into a conditional expression for Chrome 79 or earlier.[^5] It changes only syntax: it does not add polyfills for APIs.[^5]

**Tree shaking.** In esbuild, tree shaking means removing dead code at the level of declarations.[^5] Its test for side effects is conservative: it removes code only when it can be sure that the code has no hidden side effects.[^5]

**Why it is fast.** The FAQ gives several reasons.[^2] esbuild is native code, so it does not wait for a JavaScript engine to parse and optimize the code of the bundler at each run. Go has threads that share memory, and esbuild uses all CPU cores where it can: of its three phases, parsing, linking and code generation, the first and the last are the most work and fully parallel. And all of esbuild is written from scratch, with performance in mind and with consistent data structures. In the benchmark of the FAQ, which bundles 10 copies of the three.js library with no cache, esbuild took 0.39 s, Parcel 2 took 14.91 s, Rollup 4 with Terser took 34.10 s, and webpack 5 took 41.21 s.[^2]

## Common tasks

- **Bundle for a browser.** `esbuild app.js --bundle --outfile=out.js`.[^4]
- **Bundle for Node.js.** `esbuild app.js --bundle --platform=node`.[^5]
- **Minify and write a source map.** `--minify` and `--sourcemap`, as in the run above.[^5] Minified code is smaller, so it downloads faster, but it is harder to debug.[^5]
- **Split code.** `--splitting` puts the code that many entry points share in a shared file, and puts code that `import()` loads in a separate file.[^5] It works only with the `esm` format, and the documentation calls it a work in progress.[^5]
- **Watch and serve.** `--watch` rebuilds when a file changes.[^5] Serve mode starts a web server for the code.[^5]
- **Run from npm.** Put the command in the `scripts` of `package.json`, such as `"build": "esbuild app.jsx --bundle --outfile=out.js"`, and run `npm run build`.[^4]

## Configuration

The documentation gives the options of esbuild on the command line, or as the arguments of the API in JavaScript or Go.[^5] When the command line must pass many options, the documentation moves the build to a script that calls the JavaScript API. The script must be in a file with the `.mjs` extension, because it uses `import`:[^4][^5]

```text
import * as esbuild from 'esbuild'

await esbuild.build({
  entryPoints: ['app.ts'],
  bundle: true,
  outdir: 'dist',
})
```

## Alternatives

**Rollup.** Rollup is a module bundler for JavaScript that uses the standard ES module format.[^7] It analyzes the imported code statically and leaves out the parts that are not used.[^7] Rollup can also write UMD or CommonJS output, so that a library in ES modules works with tools such as Node.js and webpack.[^7] The option `-f` or `--format` chooses the output type: `amd`, `cjs`, `es`, `iife`, `umd` or `system`.[^8] Rollup is under the MIT licence.[^9] The second script bundles the same two modules with Rollup, in the `es` and `iife` formats. It runs after the first script, in the same directory. Like esbuild, Rollup leaves out `square`. The `es` output is a module with no wrapper, and the `iife` output adds `'use strict'`.

```text
# Bundle the same two modules with Rollup, as an ES module and as an IIFE.
# rollup must be installed in node_modules, and src/ must exist from esbuild.sh.
npx rollup --version
echo "--- format es"
npx rollup src/index.js --format es --silent
echo "--- format iife"
npx rollup src/index.js --format iife --silent
```

```text
rollup v4.68.3
--- format es
function cube(x) {
  return x * x * x;
}

console.log('5 cubed is', cube(5));
--- format iife
(function () {
  'use strict';

  function cube(x) {
    return x * x * x;
  }

  console.log('5 cubed is', cube(5));

})();
```

**Rolldown.** Rolldown is a bundler written in Rust. It is designed to be the bundler under Vite, and to replace both esbuild and Rollup there with one tool.[^10] It supports the plugin API of Rollup and Vite, and it can also be used alone, as a replacement for Rollup in most cases.[^10] Its documentation says that it is on the same performance level as esbuild, and 10 to 30 times faster than Rollup.[^10] It is under the MIT licence.[^11]

**Parcel.** Parcel calls itself a build tool for the web with zero configuration.[^12] It accepts any type of file as an entry point, and its guide starts from an HTML file: Parcel follows all the dependencies from there.[^13] It has a development server that rebuilds the application on a change.[^13] When a project uses a file type that Parcel does not support by default, Parcel installs the plugins that it needs.[^12]

**Rspack.** Rspack is a web bundler written in Rust. It is compatible with most of the configuration options and APIs of webpack, so a project can keep its loaders and plugins.[^14] It uses a parallel architecture, incremental compilation, lazy compilation and a persistent cache, and its documentation says that it is more than 10 times faster than webpack.[^14]

**Turbopack.** Turbopack is an incremental bundler for JavaScript and TypeScript, written in Rust and built into Next.js.[^15] It caches results down to the level of functions, keeps them on disk between runs, and bundles only what the dev server requests.[^15] It needs native bindings for the platform. On a platform without them, such as FreeBSD, Next.js uses WebAssembly bindings, which do not support Turbopack, and you build with the option `--webpack`.[^15]

| Tool | Main use on this page |
| --- | --- |
| esbuild | bundles and transforms from a CLI or an API, in Go |
| Rollup | ES modules, and output in many formats for libraries |
| Rolldown | the bundler of Vite, in Rust, with the plugin API of Rollup |
| Parcel | applications with no configuration |
| Rspack | a bundler in Rust for projects that use webpack |
| Turbopack | the bundler of Next.js, in Rust |

## History and versions

The npm registry lists the first releases of esbuild from April 2020, and the release used here, 0.28.2, from August 2026.[^16] esbuild has not reached version 1.0. Its author calls it a late-stage beta: it is stable, but some features, mainly code splitting, are still primitive.[^2] Effort is still made to keep the API stable.[^2] Vite used esbuild for a long time to transform TypeScript into JavaScript.[^2]

Rollup published version 0.1.0 on npm in May 2015, 1.0.0 in December 2018, and 4.0.0 in October 2023.[^17] Parcel 2.0.0 is from October 2021.[^18] The package `@rspack/core` published 1.0.0 in August 2024 and 2.0.0 in April 2026.[^19] Rolldown published 1.0.0 in May 2026.[^20]

## References
