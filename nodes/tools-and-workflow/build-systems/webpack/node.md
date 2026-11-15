---
tag: 3H4QP3
kind: tool
slug: webpack
name: webpack
aliases: [webpack, webpack-cli, webpack.config.js, webpack loader, webpack plugin, SplitChunksPlugin, webpack-dev-server]
area: tools-and-workflow/build-systems
level: core
summary: >-
  webpack is a module bundler for JavaScript. It follows the imports from the entry modules to build a dependency graph and writes bundles. Loaders transform other files, and plugins change the build.
role: [build]
licence: mit
repo: https://github.com/webpack/webpack
homepage: https://webpack.js.org/
see_also: [javascript-bundlers, vite, javascript, typescript, build-system-concept, build-graph, incremental-builds, make]
sources:
  - url: https://webpack.js.org/concepts/
    title: webpack documentation, Concepts
    locator: the introduction; Entry; Output; Loaders; Plugins; Mode; Browser Compatibility; Environment
  - url: https://webpack.js.org/concepts/dependency-graph/
    title: webpack documentation, Dependency Graph
    locator: the whole page
  - url: https://github.com/webpack/webpack/blob/main/LICENSE
    title: webpack, LICENSE (MIT)
    locator: the whole file
  - url: https://webpack.js.org/license/
    title: webpack documentation, License
    locator: webpack; webpack documentation; webpack code samples
  - url: https://webpack.js.org/guides/getting-started/
    title: webpack documentation, Getting Started
    locator: Basic Setup; Creating a Bundle; Using a Configuration
  - url: https://webpack.js.org/guides/tree-shaking/
    title: webpack documentation, Tree Shaking
    locator: the introduction; Add a Utility; Mark the file as side-effect-free; Minify the Output
  - url: https://webpack.js.org/configuration/mode/
    title: webpack documentation, Mode
    locator: Usage; the table of modes; Defaults per mode
  - url: https://webpack.js.org/concepts/loaders/
    title: webpack documentation, Loaders
    locator: the introduction; Example; Using Loaders
  - url: https://webpack.js.org/concepts/plugins/
    title: webpack documentation, Plugins
    locator: the introduction; Anatomy
  - url: https://webpack.js.org/guides/code-splitting/
    title: webpack documentation, Code Splitting
    locator: the introduction (three approaches)
  - url: https://webpack.js.org/configuration/devtool/
    title: webpack documentation, Devtool
    locator: devtool
  - url: https://webpack.js.org/guides/caching/
    title: webpack documentation, Caching
    locator: Output Filenames
  - url: https://webpack.js.org/concepts/hot-module-replacement/
    title: webpack documentation, Hot Module Replacement
    locator: the introduction
  - url: https://webpack.js.org/concepts/why-webpack/
    title: webpack documentation, Why webpack
    locator: IIFEs; Birth of JavaScript Modules; ESM; Automatic Dependency Collection
  - url: https://registry.npmjs.org/webpack
    title: npm registry, package webpack
    locator: the time field (release dates of 0.1.0, 1.0.0, 2.2.0, 3.0.0, 4.0.0 and 5.0.0)
---

## What it is

The webpack documentation says: "At its core, webpack is a static module bundler for modern JavaScript applications". When webpack processes an application, it builds a dependency graph from one or more entry points, and then it combines the modules that the project needs into one or more bundles. The bundles are static files that a server sends to the browser.[^1] The page [[javascript-bundlers]] explains why bundlers exist and what they do in general. This page is about webpack: its configuration, its loaders and its plugins.

Each time one file depends on another, webpack treats this as a dependency. So a project can also give webpack images, fonts and other files that are not code, as dependencies of its modules.[^2] Out of the box, webpack understands JavaScript, JSON and WebAssembly files, and CSS and HTML through built-in support. Loaders let it process other types of files.[^1]

webpack runs on Node.js. Webpack 5 needs Node.js 10.13.0 or later, and the bundles that it makes support all browsers that are ES5-compliant.[^1] webpack is free software under the MIT License, "Copyright JS Foundation and other contributors".[^3] Its documentation is under the Creative Commons BY 4.0 licence, and the code samples of the documentation are under CC0 1.0.[^4]

## Install and first use

A project installs webpack and its command-line interface from npm, as development dependencies:[^5]

```text
npm install webpack webpack-cli --save-dev
```

Then `npx webpack` runs the local copy of webpack. From version 4.0.0, webpack does not need a configuration file: the default entry is `./src/index.js`, and the default output is `./dist/main.js`.[^1][^5] A file `webpack.config.js` in the root of the project changes these defaults.[^5]

The build machine of this site has no webpack, so the scripts on this page do not run when the site is built. They ran once on the build machine, in a temporary directory, with webpack 5.112.2 and webpack-cli 7.3.1 installed from npm, on Node.js 24.10.0. The output after each script is the recorded output of that run. The script below writes two modules, the same as in the tree-shaking guide of webpack, and a configuration file. Then it makes a production build, shows the bundle and runs it with Node.js. The `grep` removes the line with the time of the build.[^6]

```text
# Bundle two modules with webpack in production mode.
# webpack and webpack-cli must be installed in node_modules.
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
cat > webpack.config.js <<'JS'
const path = require('path');

module.exports = {
  entry: './src/index.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist'),
  },
};
JS
npx webpack --mode production --no-color 2>&1 | grep -v -E 'compiled successfully in'
echo "--- dist/main.js"
cat dist/main.js; echo
echo "--- run the bundle"
node dist/main.js
```

The output of that run:

```text
asset main.js 55 bytes [emitted] [minimized] (name: main)
orphan modules 95 bytes [orphan] 1 module
./src/index.js + 1 modules 166 bytes [built] [code generated]
--- dist/main.js
(()=>{"use strict";console.log("5 cubed is",5*5*5)})();
--- run the bundle
5 cubed is 125
```

The bundle has one line. The module `index.js` imports only `cube`, so the function `square` is not in the bundle: this is tree shaking.[^6] The lines `orphan modules` and `./src/index.js + 1 modules` show that webpack built `math.js` as a part of `index.js`, and not as a module of its own. In the bundle, the call `cube(5)` became `5*5*5`. The mode `production` turns on the plugins for this work, such as `FlagDependencyUsagePlugin`, `ModuleConcatenationPlugin` and `MinimizerPlugin`.[^7]

In the mode `development`, webpack keeps all the code readable, and the option `optimization.usedExports` makes it mark each export that no module uses.[^6] The script below builds the same two modules in this way, with a second configuration file, and shows the lines of the bundle that are about the exports.

```text
# Build the same modules in development mode, with usedExports on,
# to see how webpack marks the unused export. Run after bundle.sh.
cat > webpack.dev.js <<'JS'
const path = require('path');

module.exports = {
  mode: 'development',
  devtool: false,
  entry: './src/index.js',
  output: {
    filename: 'dev.js',
    path: path.resolve(__dirname, 'dist'),
  },
  optimization: {
    usedExports: true,
  },
};
JS
npx webpack --config webpack.dev.js --no-color 2>&1 | grep -v -E 'compiled successfully in'
echo "--- the lines about exports in dist/dev.js"
grep -E 'harmony export|^function' dist/dev.js
echo "--- run the bundle"
node dist/dev.js
```

The output of that run:

```text
asset dev.js 2.94 KiB [emitted] (name: main)
runtime modules 403 bytes 2 modules
cacheable modules 166 bytes
  ./src/index.js 71 bytes [built] [code generated]
  ./src/math.js 95 bytes [built] [code generated]
--- the lines about exports in dist/dev.js
/* harmony export */ __webpack_require__.d(__webpack_exports__, {
/* harmony export */   cube: () => (/* binding */ cube)
/* harmony export */ });
/* unused harmony export square */
function square(x) {
function cube(x) {
/******/ 	// define getter/value functions for harmony exports
--- run the bundle
5 cubed is 125
```

The comment `unused harmony export square` is the mark, and "harmony modules" is another name for ES2015 modules.[^6] The function `square` is still in this bundle. The guide then sets the mode to `production`, and the production build drops the unused code, as the first run shows.[^6]

## Concepts of the tool

**The dependency graph.** webpack starts from the entry points that the command line or the configuration gives, follows the imports recursively, and builds a graph of all the modules that the application needs. Then it puts all these modules into a small number of bundles, often only one.[^2] The documentation says that bundling helps HTTP/1.1 clients, because the browser makes fewer requests, and that with HTTP/2, code splitting can also help.[^2] The page [[build-graph]] explains dependency graphs in build tools in general.

**Entry and output.** An entry point is the module where webpack starts the graph. The property `entry` gives one or more entry points.[^1] The property `output` gives where webpack writes the bundles and how it names them.[^1]

**Loaders.** A loader is a transformation that webpack applies to the source code of a module. Loaders can compile another language, such as TypeScript, to JavaScript, or load an image as a data URL.[^8] In the configuration, a rule has two properties: `test` selects the files, and `use` names the loader for them.[^1] This configuration from the documentation uses `sass-loader` for each `.scss` file and `ts-loader` for each `.ts` file:[^8]

```text
export default {
  module: {
    rules: [
      { test: /\.scss$/, use: "sass-loader" },
      { test: /\.ts$/, use: "ts-loader" },
    ],
  },
};
```

**Plugins.** The documentation calls plugins "the backbone of webpack": webpack itself uses the same plugin system, and plugins do the work that a loader cannot do.[^9] A plugin is a JavaScript object with an `apply` method. The compiler calls this method, and the plugin gets access to the whole lifecycle of the compilation through hooks.[^9]

**Modes.** The option `mode` is `development`, `production` or `none`, and the default is `production`.[^7] The mode does nothing by itself: it selects the default values of other options.[^7] `production` gives deterministic mangled names to modules and chunks and turns on the plugins of the first run above. `development` gives useful names to modules and chunks. `none` turns off all the default optimizations.[^7]

**Tree shaking and side effects.** Tree shaking needs the static structure of `import` and `export`. webpack 2 added support for ES2015 modules and the detection of unused exports.[^6] The guide defines a side effect as "code that performs a special behavior when imported, other than exposing one or more exports", such as a polyfill that changes the global scope.[^6] The property `"sideEffects"` in `package.json` tells webpack which files are pure, with no side effects, and so safe to prune when they are not used. `"sideEffects": false` marks all the files of the package as pure, and an array lists the files that have side effects.[^6]

**Code splitting.** webpack has three general ways to split code into more than one bundle: more than one entry point, the removal of duplicates with entry dependencies or `SplitChunksPlugin`, and dynamic imports, with inline function calls in modules.[^10] The page [[javascript-bundlers]] explains why an application splits its code.

**Source maps.** The option `devtool` controls how webpack makes source maps. The default is `false`, no source maps, and in the mode `development` the default is `'eval'` for JavaScript. The documentation says that these values "can affect build and rebuild speed dramatically".[^11]

**Caching.** A browser keeps files in its cache. When a bundle changes, its name must change, so that the browser loads it again. The substitution `[contenthash]` in a file name adds a hash of the content of the file, and the hash changes when the content changes.[^12]

**Hot module replacement.** Hot module replacement (HMR) "exchanges, adds, or removes modules while an application is running, without a full reload". So the application keeps its state, and a change to CSS or JavaScript shows in the browser at once.[^13]

## Common tasks

| Task | Command or setting |
|---|---|
| Install webpack in a project | `npm install webpack webpack-cli --save-dev`[^5] |
| Build with the defaults or with `webpack.config.js` | `npx webpack`[^5] |
| Build in development mode | `npx webpack --mode development`[^7] |
| Use another configuration file | `npx webpack --config webpack.dev.js`[^5] |
| Name the output files by their content | `filename: '[name].[contenthash].js'` in `output`[^12] |
| Write source maps to separate files | `devtool: 'source-map'`[^11] |
| Mark a package as free of side effects | `"sideEffects": false` in `package.json`[^6] |

## Configuration

The configuration is a JavaScript module that exports an object. When the file `webpack.config.js` is present, the command `webpack` uses it by default, and the option `--config` gives a file with another name.[^5] The main properties are the core concepts above: `entry`, `output`, `module.rules` for the loaders, `plugins`, `mode`, `devtool` and `optimization`.[^1][^7] Because the configuration is code, it can compute values, such as the absolute path of the output directory with `path.resolve`.[^1] A configuration file can be a CommonJS module, with `module.exports`, as in the scripts above, or an ECMAScript module, with `export default`, as in the documentation.[^5]

## Alternatives

- [[vite]]: a dev server on native ES modules and a build command that bundles with Rolldown.
- [[make]]: a general build tool. Before bundlers, task runners such as Make, Gulp and Grunt joined the script files of a project into one file.[^14]

The page [[javascript-bundlers]] compares the work of bundlers in general.

## History and versions

The documentation of webpack tells the history of the problem. Before bundlers, a page loaded one script for each function, or one big script. Immediately invoked function expressions (IIFEs) let task runners join the files without a clash of names, but a change to one file made the runner rebuild the whole thing. Node.js and CommonJS brought `require`, but browsers did not support CommonJS, so tools such as Browserify, RequireJS and SystemJS let CommonJS modules run in a browser. A bundler such as webpack finds the dependency graph from the imports and exports by itself, and the documentation names this "Automatic Dependency Collection".[^14]

The npm registry gives these release dates:[^15]

| Version | Release date |
|---|---|
| 0.1.0 | 11 March 2012 |
| 1.0.0 | 19 February 2014 |
| 2.2.0 | 17 January 2017 |
| 3.0.0 | 19 June 2017 |
| 4.0.0 | 25 February 2018 |
| 5.0.0 | 10 October 2020 |

Webpack 2 added support for ES2015 modules, and webpack 4 added the property `"sideEffects"`.[^6] From webpack 4.0.0, a project does not need a configuration file.[^1] This page uses webpack 5.112.2.

## References
