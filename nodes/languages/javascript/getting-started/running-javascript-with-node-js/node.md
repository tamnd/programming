---
tag: 87KT3Q
kind: feature
owner: javascript
slug: running-javascript-with-node-js
name: Running JavaScript with Node.js
aliases: [node command, node REPL, node -e, node -p, .mjs, .cjs, package.json type]
area: languages/javascript/getting-started
level: intro
summary: >-
  The node command runs a file, a string (-e, -p) or standard input, and starts the REPL with no program. The file extension and the type field of package.json pick CommonJS or ES module.
grammar: "node [options] [V8 options] [<program-entry-point> | -e \"script\" | -] [--] [arguments] (Node.js, Command-line API, Synopsis)"
spec: {standard: null, url: "https://nodejs.org/api/cli.html#synopsis", locator: "Command-line API, Synopsis"}
see_also: [javascript/running-javascript-in-the-browser, javascript/a-first-program, javascript/the-browser-console, javascript/javascript-engines, python/running-a-python-program, python/interactive-mode, parse-command-line-arguments]
sources:
  - url: https://nodejs.org/en/learn/getting-started/introduction-to-nodejs
    title: Introduction to Node.js, Node.js Learn
    locator: introduction; An Example Node.js Application
    accessed: 2026-11-18
  - url: https://nodejs.org/api/cli.html#synopsis
    title: Command-line API, Node.js documentation
    locator: Synopsis; Program entry point; -e; -p; -c; -i; -; --watch
    accessed: 2026-11-18
  - url: https://nodejs.org/en/about/previous-releases
    title: Node.js Releases
    locator: introduction; Release Schedule
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/command-line/run-nodejs-scripts-from-the-command-line
    title: Run Node.js scripts from the command line, Node.js Learn
    locator: shebang; Restart the application automatically; Using the --run flag
    accessed: 2026-11-18
  - url: https://nodejs.org/api/process.html#processargv
    title: Process, Node.js documentation
    locator: process.argv
    accessed: 2026-11-18
  - url: https://nodejs.org/api/packages.html#determining-module-system
    title: Modules, Packages, Node.js documentation
    locator: Determining module system; Syntax detection
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/command-line/how-to-use-the-nodejs-repl
    title: How to use the Node.js REPL, Node.js Learn
    locator: How to use the Node.js REPL; Dot commands
    accessed: 2026-11-18
  - url: https://nodejs.org/api/repl.html
    title: REPL, Node.js documentation
    locator: JavaScript expressions; Accessing core Node.js modules; Assignment of the _ (underscore) variable; await keyword
    accessed: 2026-11-18
---

## Synopsis

```text
node app.js [args]          run a file; args go to process.argv
node app.mjs                run a file as an ES module
node -e "code" [args]       run the code in the string
node -p "expression"        run the string and print its result
node - < app.js             read the program from standard input
node --check app.js         check the syntax, run nothing
node --watch app.js         run, and restart when the file changes
node --run test             run the "test" script of package.json
node                        start the REPL
```

Node.js is "an open-source and cross-platform JavaScript runtime environment" that "runs the V8 JavaScript engine, the core of Google Chrome, outside of the browser".[^1] Its command is `node`. Options come before the program, and the arguments after it go to the program.[^2]

## Semantics

**r1. Installing.** The Node.js project ships builds for each release line. A line is first Current, and then it moves to LTS, "long-term support", which "typically guarantees that critical bugs will be fixed for a total of 30 months". The project says: "Production applications should only use Active LTS or Maintenance LTS releases."[^3] The examples of this guide run with Node.js 24, an LTS line.[^3]

**r2. A file.** `node app.js` runs the file. A relative name is resolved from the current directory, and an error is thrown when no file matches.[^2] A first line `#!/usr/bin/env node` and the executable bit let a Unix shell run the file as `./app.js`.[^4] Example: [`run-forms`](#run-forms).

**r3. Arguments.** `process.argv` is an array. Its first element is the path of the `node` executable, its second is the absolute path of the program, and then: "The remaining elements are additional command-line arguments."[^5] So a program reads its own arguments with `process.argv.slice(2)` ([[parse-command-line-arguments]]). Example: [`run-forms`](#run-forms).

**r4. A string or standard input.** `-e` evaluates its argument as JavaScript, and `-p` is described as "Identical to -e but prints the result." A lone `-` reads the program from standard input.[^2] `--check` does a syntax check "without executing".[^2]

**r5. Watch mode and tasks.** With `--watch`, "changes in the watched files cause the Node.js process to restart".[^2] `node --run name` runs a command from the `scripts` field of `package.json`, without the pre and post scripts of `npm run`.[^4]

**r6. CommonJS or ES module.** Node.js has two module systems. CommonJS uses `require()`, and ES modules use `import`.[^1] Node.js picks one per file:[^6]

- `.mjs` is an ES module and `.cjs` is CommonJS, always.
- `.js` follows the `type` field of the nearest `package.json`: `module` or `commonjs`.
- A `.js` file with no `type` is ambiguous. Node.js runs it as CommonJS, and if the parser finds ES module syntax such as `import`, `export` or `import.meta`, it runs it again as an ES module.

Syntax detection "is enabled by default" from Node.js 22.7.0, and the documentation asks package authors to always set the `type` field, because ES module syntax in an ambiguous file "incurs a performance cost".[^6] Example: [`module-kind`](#module-kind).

**r7. The REPL.** `node` with no program starts the REPL: it reads a line, evaluates it, prints the result and loops.[^7] It prints the value of each expression, and `undefined` for a statement such as a declaration.[^7] [^8] The variable `_` holds the result of the last expression, `await` works at the top level, and the core modules such as `fs` load on first use.[^8] Dot commands control the session: `.help`, `.editor`, `.break`, `.load`, `.save` and `.exit`.[^7] An unfinished line, such as `function twice(x) {`, continues on the next line.[^7] Example: [`repl`](#repl).

## Examples

The forms of rules r2 to r4, each in a child process. The example writes `greet.js` to a temporary folder and runs it with arguments, then runs strings and standard input.

```javascript run id=run-forms
const { spawnSync } = require("node:child_process");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

function node(args, options = {}) {
  const result = spawnSync(process.execPath, args, { encoding: "utf8", ...options });
  return (result.stdout + result.stderr).trim();
}

const dir = fs.mkdtempSync(path.join(os.tmpdir(), "run-"));
fs.writeFileSync(path.join(dir, "greet.js"), "console.log('hello,', process.argv.slice(2).join(' and '));\n");

console.log("file: ", node(["greet.js", "Ada", "Grace"], { cwd: dir }));
console.log("-e:   ", node(["-e", "console.log(6 * 7)"]));
console.log("-p:   ", node(["-p", "6 * 7"]));
console.log("stdin:", node([], { input: "console.log('from stdin')" }));
console.log("check:", node(["--check", "greet.js"], { cwd: dir }) || "(no syntax errors)");
fs.rmSync(dir, { recursive: true });
```

```output for=run-forms
file:  hello, Ada and Grace
-e:    42
-p:    42
stdin: from stdin
check: (no syntax errors)
```

The module system of each file (rule r6). Every file runs the same probe; only CommonJS has `require`. The file `plain/d.js` has no marker, but its `import` statement makes Node.js run it as an ES module.

```javascript run id=module-kind
const { spawnSync } = require("node:child_process");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const probe = "console.log(typeof require === 'function' ? 'CommonJS' : 'ES module');\n";
const dir = fs.mkdtempSync(path.join(os.tmpdir(), "kind-"));
const files = {
  "plain/a.js": probe,
  "plain/b.cjs": probe,
  "plain/c.mjs": probe,
  "plain/d.js": "import os from 'node:os';\n" + probe,
  "typed/package.json": '{ "type": "module" }\n',
  "typed/e.js": probe,
  "typed/f.cjs": probe,
};
for (const [name, text] of Object.entries(files)) {
  fs.mkdirSync(path.dirname(path.join(dir, name)), { recursive: true });
  fs.writeFileSync(path.join(dir, name), text);
}
for (const name of Object.keys(files).filter((name) => !name.endsWith(".json"))) {
  const result = spawnSync(process.execPath, [path.join(dir, name)], { encoding: "utf8" });
  console.log(name.padEnd(11), result.stdout.trim());
}
fs.rmSync(dir, { recursive: true });
```

```output for=module-kind
plain/a.js  CommonJS
plain/b.cjs CommonJS
plain/c.mjs ES module
plain/d.js  ES module
typed/e.js  ES module
typed/f.cjs CommonJS
```

A REPL session with its input piped in (rule r7). `-i` opens the REPL even when standard input is not a terminal.[^2] The example drops the two welcome lines, which name the version, and the bare prompts.

```javascript run id=repl
const { spawnSync } = require("node:child_process");

const input = [
  "1 + 2",
  "_ * 10",
  "const n = 5",
  "n ** 2",
  "function twice(x) {",
  "return 2 * x }",
  "twice(n)",
  "await Promise.resolve('done')",
  ".exit",
].join("\n");
const result = spawnSync(process.execPath, ["-i"], { input: input + "\n", encoding: "utf8" });
const lines = result.stdout.split("\n").slice(2);
console.log(lines.filter((line) => line.trim() !== ">").join("\n"));
```

```output for=repl
> 3
> 30
> undefined
> 25
> | undefined
> 10
> 'done'
```

The `|` prompt is the continuation line of the function. A file with `import` and no marker is an ES module, and an ES module has no `require`:

```javascript fails id=no-require file=main.js expect=exception:ReferenceError
import { platform } from "node:os";

console.log(typeof platform);
const path = require("node:path");
```

Node.js says `ReferenceError: require is not defined in ES module scope, you can use import instead`.

## Pitfalls

### `require` in an ES module, `import` in CommonJS

The two systems are fixed per file by rule r6. A file that mixes them fails, as [`no-require`](#no-require) shows. Rename it to `.mjs` or `.cjs`, or set `type` in `package.json`.[^6]

### Detection depends on the code

Without a `type` field, a `.js` file that loses its last `import` or `export` runs as CommonJS from then on. Set the field, or use `.mjs` and `.cjs`, to fix the choice.[^6]

### Quotes on Windows

In `cmd.exe` a single quote does not quote an `-e` string; only the double quote does. PowerShell and Git Bash accept both.[^2]

## Comparison

| Task | Node.js | Python |
|---|---|---|
| run a file | `node app.js` | `python3 app.py` |
| run a string | `node -e "..."`, `node -p "..."` | `python3 -c "..."` |
| read standard input | `node -` | `python3 -` |
| interactive session | `node` | `python3` |
| last result | `_` | `_` |
| arguments | `process.argv.slice(2)` | `sys.argv[1:]` |

For the Python side, see [[python/running-a-python-program]] and [[python/interactive-mode]]. A browser runs the same language through script elements instead: [[javascript/running-javascript-in-the-browser]].

## References
