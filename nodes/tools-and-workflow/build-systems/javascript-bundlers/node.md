---
tag: NXS8ED
kind: concept
slug: javascript-bundlers
name: JavaScript bundlers
aliases: [bundler, module bundler, JavaScript bundling, bundle, tree shaking, code splitting, source map, chunk]
area: tools-and-workflow/build-systems
level: core
summary: >-
  A JavaScript bundler follows the imports of an application from its entry points and joins the modules into a few files. It can also remove unused code, split code into chunks and write source maps.
viz: optional
formal: none
see_also: [webpack, vite, javascript, typescript, module-pattern, build-graph, build-system-concept, incremental-builds]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Modules
    title: MDN Web Docs, JavaScript modules
    locator: A background on modules
  - url: https://webpack.js.org/concepts/why-webpack/
    title: webpack documentation, Why webpack
    locator: the whole page
  - url: https://vite.dev/guide/why
    title: Vite documentation, Why Vite
    locator: The Origins; A Unified Toolchain
  - url: https://webpack.js.org/concepts/
    title: webpack documentation, Concepts
    locator: introduction; Entry; Output; Loaders
  - url: https://webpack.js.org/concepts/dependency-graph/
    title: webpack documentation, Dependency Graph
    locator: the whole page
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Tree_shaking
    title: MDN Web Docs, Glossary, Tree shaking
    locator: the whole entry
  - url: https://esbuild.github.io/faq/
    title: esbuild documentation, FAQ
    locator: Why is esbuild fast?
  - url: https://rollupjs.org/introduction/
    title: Rollup documentation, Introduction
    locator: Overview; Quick Start; Tree-Shaking; Compatibility
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Code_splitting
    title: MDN Web Docs, Glossary, Code splitting
    locator: the whole entry
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Source_map
    title: MDN Web Docs, Glossary, Source map
    locator: the whole entry; Example
  - url: https://vite.dev/guide/dep-pre-bundling
    title: Vite documentation, Dependency Pre-Bundling
    locator: The Why
  - url: https://webpack.js.org/guides/tree-shaking/
    title: webpack documentation, Tree Shaking
    locator: Mark the file as side-effect-free
  - url: https://webpack.js.org/guides/caching/
    title: webpack documentation, Caching
    locator: Output Filenames
  - url: https://registry.npmjs.org/webpack
    title: npm registry, package webpack
    locator: time (0.1.0)
---

## Idea

A large JavaScript program is split into modules: units of code that other units import when they need them.[^1] A browser can load a module through a `<script>` tag, but each module is then one more request over the network, and a module that imports other modules makes the browser wait for each level of imports.[^2][^3] A bundler solves this problem at build time. It starts from the entry points of the application, follows each `import` to find all the modules that the application needs, and joins these modules into one file or a few files, the bundles.[^4][^5]

Browsers now load ES modules natively, without a library. MDN says that this does not make bundlers obsolete, because bundlers still divide code into chunks of a good size and do other optimizations, such as minification, dead code elimination and tree shaking.[^1] [[webpack]], Rollup and esbuild are bundlers, and [[vite]] is a build tool that bundles the code for production.[^6][^7][^3]

## How it works

**The dependency graph.** Each time one file depends on another, the bundler records a dependency. It starts from the entry points, follows the imports recursively, and builds a graph of all the modules that the application needs.[^5] A module that no path from an entry point reaches is not in the graph, so it is not in the bundle. A bundler for the web can also treat images, fonts and style sheets as nodes of the graph.[^5] The page [[build-graph]] explains dependency graphs in build tools in general.

**Transformation.** Before a module goes into the graph, the bundler can transform it. For example, webpack uses loaders to convert files of other types, such as TypeScript, into valid modules.[^4]

**Tree shaking.** MDN says: "Tree shaking is a term commonly used within a JavaScript context to describe the removal of dead code". It uses the `import` and `export` statements to find which exports of a module other modules use.[^6] Rollup analyzes the imports statically and excludes the code that the application does not use. With CommonJS, `require` gives the whole object of a library, but an ES `import` names the one function that the module uses.[^8]

**Scope.** A module runs in its own scope, and its variables and functions are not visible outside it unless it exports them.[^1] The bundle is one script, so the bundler must keep the names of each module apart. Before bundlers, tools put each script file into an immediately invoked function expression (IIFE), so that the files could be joined without a clash of names.[^2] The page [[module-pattern]] shows this pattern. A bundler can give each module a function scope in the same way, and Rollup has an output format `iife`, a script that runs a function at once.[^8]

**Output.** The bundler writes the bundles to an output directory. An HTML file then loads the bundle instead of each module.[^4]

## In languages

The program below is a model of the four steps of a bundler, in JavaScript on Node.js. It is not a real bundler: a real bundler parses JavaScript, and this model reads only two forms of lines, `import { a } from './m.js';` and `export function a`. The program has four modules in memory. First, it follows the imports from `./index.js` and builds the graph. Then it removes each exported function that no module imports, and it writes a bundle in which each module is an IIFE that returns its exports. Last, it runs the bundle with the function `vm.runInNewContext` of Node.js.

```javascript run id=mini-bundler
// A model of a bundler. It is not a real bundler: it reads only
// "import { a } from './m.js'" and one-line "export function" forms.
const vm = require("node:vm");

const files = {
  "./index.js": [
    "import { cube } from './math.js';",
    "import { greet } from './text.js';",
    "console.log(greet('bundle'), cube(3));",
  ],
  "./math.js": [
    "export function square(x) { return x * x; }",
    "export function cube(x) { return x * x * x; }",
  ],
  "./text.js": [
    "export function greet(name) { return 'hello, ' + name; }",
    "export function shout(s) { return s.toUpperCase() + '!'; }",
  ],
  "./old.js": ["export function unusedHelper() { return 0; }"],
};

const importRe = /^import \{ (.+) \} from '(.+)';$/;
const exportRe = /^export function (\w+)/;

// 1. The dependency graph: start at the entry and follow the imports.
const order = []; // dependencies come before the modules that import them
const used = {}; // module -> the names that other modules import from it
function visit(id) {
  if (order.includes(id)) return;
  for (const line of files[id]) {
    const m = line.match(importRe);
    if (!m) continue;
    const names = m[1].split(", ");
    console.log(`${id} imports ${names.join(", ")} from ${m[2]}`);
    used[m[2]] = [...(used[m[2]] || []), ...names];
    visit(m[2]);
  }
  order.push(id);
}
visit("./index.js");
console.log("not in the graph:", Object.keys(files).filter((f) => !order.includes(f)).join(", "));

// 2. Tree shaking: keep only the exports that some module imports.
const kept = {};
for (const id of order) {
  kept[id] = files[id].filter((line) => {
    const m = line.match(exportRe);
    if (m && !(used[id] || []).includes(m[1])) {
      console.log(`removed: ${m[1]} in ${id}`);
      return false;
    }
    return true;
  });
}

// 3. Join the modules into one script. Each module gets its own scope.
const out = ["(() => {", "  const modules = {};"];
for (const id of order) {
  const exportsOf = [];
  const body = [];
  for (const line of kept[id]) {
    const im = line.match(importRe);
    const ex = line.match(exportRe);
    if (im) body.push(`const { ${im[1]} } = modules['${im[2]}'];`);
    else if (ex) {
      exportsOf.push(ex[1]);
      body.push(line.replace(/^export /, ""));
    } else body.push(line);
  }
  out.push(`  // ${id}`);
  out.push(`  modules['${id}'] = (() => {`);
  for (const line of body) out.push(`    ${line}`);
  out.push(exportsOf.length ? `    return { ${exportsOf.join(", ")} };` : "    return {};");
  out.push("  })();");
}
out.push("})();");
const bundle = out.join("\n");
console.log("--- the bundle");
console.log(bundle);

// 4. Run the bundle as one script, as a browser runs one <script> file.
console.log("--- run the bundle");
vm.runInNewContext(bundle, { console });
```

```output for=mini-bundler
./index.js imports cube from ./math.js
./index.js imports greet from ./text.js
not in the graph: ./old.js
removed: square in ./math.js
removed: shout in ./text.js
--- the bundle
(() => {
  const modules = {};
  // ./math.js
  modules['./math.js'] = (() => {
    function cube(x) { return x * x * x; }
    return { cube };
  })();
  // ./text.js
  modules['./text.js'] = (() => {
    function greet(name) { return 'hello, ' + name; }
    return { greet };
  })();
  // ./index.js
  modules['./index.js'] = (() => {
    const { cube } = modules['./math.js'];
    const { greet } = modules['./text.js'];
    console.log(greet('bundle'), cube(3));
    return {};
  })();
})();
--- run the bundle
hello, bundle 27
```

The module `./old.js` is not in the bundle, because no module imports it. The functions `square` and `shout` are exported, but no module imports them, so tree shaking removed them. The modules are in the order of the graph: `./math.js` and `./text.js` come before `./index.js`, because `./index.js` uses their exports. The page [[webpack]] shows the same steps in a real bundler: the bundle of webpack has no `square`, and the call `cube(5)` became `5*5*5`.

## Variations and related ideas

**Code splitting.** Code splitting puts the code of an application, and the code of its dependencies, into separate bundles that the browser can load independently. The application loads only the code that it needs at the start, and it loads other bundles on demand. This makes the first load of the application faster.[^9] Bundlers such as webpack and Browserify support code splitting: they can make more than one bundle, and the application loads them at run time.[^9]

**Source maps.** A bundle that is joined and minified is hard to debug. A source map is "a JSON file format that maps between minified or transformed code received by the browser and its original unmodified form".[^10] A browser finds the source map through the HTTP header `SourceMap` or through a comment `sourceMappingURL` at the end of the generated code.[^10] The run on the page [[vite]] shows such a comment and the source files that its map lists.

**Unbundled development.** Vite does not bundle the code of the application during development. It serves each source module over native ES modules when the browser asks for it, and so its dev server does not need to process the whole application before it starts.[^3] It still pre-bundles the dependencies, because some of them are CommonJS or UMD, and some, such as `lodash-es` with over 600 internal modules, would make the browser send over 600 HTTP requests at the same time.[^11] For production, Vite bundles the application, because unbundled modules cause more network round trips.[^3]

**Bundlers in native languages.** Most bundlers are written in JavaScript. esbuild is written in Go and compiles to native code, and its FAQ gives this and the heavy use of parallelism as reasons for its speed.[^7] Rolldown, the bundler of Vite, is written in Rust.[^3]

**Libraries.** A bundler can also build a library. Rollup can compile ES modules to the CommonJS or UMD format, so that tools that use CommonJS can use the library.[^8]

## Pitfalls

**Side effects.** Some modules do something when another module imports them, for example a polyfill that changes the global scope. webpack calls this a side effect, The property `"sideEffects"` in `package.json` tells webpack which files are "pure" and so safe to prune when they are not used.[^12] The guide warns that a CSS file that a module imports only for its side effect must be in the list of side effects, so that the production build does not drop it.[^12]

**CommonJS.** Tree shaking relies on the static `import` and `export` statements.[^6] With CommonJS, `require` returns the whole object of a library, so the bundler cannot remove its unused parts in the same way.[^8]

**Cached bundles.** A browser keeps files in its cache. If a bundle changes but keeps its name, the browser can use the old file. webpack adds a hash of the content to the file name, with `[contenthash]`, so that each change gives a different name.[^13]

**Debugging.** The code in a bundle is not the code in the source files. Without a source map, the debugger of the browser shows the minified code.[^10]

## Cost

A bundler adds a build step between the source code and the browser. Before the bundler can write a bundle, it reads, parses and transforms every module of the graph. The Vite documentation says that, in a bundle-based dev server, the start takes longer as the application grows.[^3] The esbuild FAQ says that parsing and code generation are most of the work of a bundler, and that they can run in parallel, but linking is mostly serial.[^7]

In the browser, the bundle saves requests. The webpack documentation says that bundling helps HTTP/1.1 clients because the browser makes fewer requests, and that with HTTP/2, code splitting can also help.[^5] Code splitting lets an application load only the code that it needs at a given time, and MDN says that this improves the performance of the application, especially at the first load.[^9]

## History

Before bundlers, a page loaded one script for each function, or one big script. Too many scripts make a bottleneck in the network, and one big script has problems with scope, size and maintenance.[^2] IIFEs let task runners, such as Make, Gulp and Grunt, join the script files safely, but a change to one file made the runner rebuild everything, and the runner could not find unused code.[^2] Node.js and CommonJS brought `require`, but browsers did not support CommonJS, so Browserify, RequireJS and SystemJS let CommonJS modules run in a browser.[^2] Then the ECMAScript standard got its own modules.[^2] A bundler such as webpack builds the dependency graph from the imports and exports by itself, and the webpack documentation calls this "Automatic Dependency Collection".[^2] The first version of webpack on npm, 0.1.0, is from 11 March 2012.[^14]

Vite first used esbuild in development and Rollup for production builds. Its documentation says that the two pipelines caused "different transformation behaviors, separate plugin systems, and growing glue code", and Rolldown takes the place of both.[^3]

## References
