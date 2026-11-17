---
tag: SGHKQ5
kind: feature
owner: javascript
slug: hashbang-comments
name: Hashbang comments
aliases: [hashbang, shebang, "#!", "#!/usr/bin/env node", HashbangComment]
area: languages/javascript/syntax
level: intro
summary: >-
  A #! line at the very start of a script or module is a comment for the engine. A Unix shell reads it to pick the interpreter, so a file that starts with #!/usr/bin/env node runs as a command.
grammar: "HashbangComment :: #! SingleLineCommentChars opt (ECMA-262, 12.5 Hashbang Comments)"
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-hashbang", locator: "12.5 Hashbang Comments"}
see_also: [javascript/comments, javascript/lexical-grammar, javascript/running-javascript-with-node-js, javascript/white-space-and-line-terminators, python/running-a-python-program]
sources:
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#hashbang_comments
    title: Lexical grammar, MDN
    locator: Hashbang comments
    accessed: 2026-11-18
  - url: https://tc39.es/ecma262/2026/multipage/ecmascript-language-lexical-grammar.html#sec-hashbang
    title: ECMAScript 2026 Language Specification
    locator: "12 ECMAScript Language: Lexical Grammar; 12.5 Hashbang Comments"
    accessed: 2026-11-18
  - url: https://tc39.es/proposal-hashbang/
    title: Hashbang Grammar, TC39 proposal
    locator: introduction; Why only at the start of a Source Text; Does the hashbang syntax work within eval / Function?; Interaction with byte-order mark
    accessed: 2026-11-18
  - url: https://nodejs.org/en/learn/command-line/run-nodejs-scripts-from-the-command-line
    title: Run Node.js scripts from the command line, Node.js Learn
    locator: introduction, shebang
    accessed: 2026-11-18
  - url: https://docs.python.org/3/reference/lexical_analysis.html#comments
    title: Lexical analysis, The Python Language Reference
    locator: 2.1.3 Comments; 2.1.4 Encoding declarations
    accessed: 2026-11-18
---

## Synopsis

```javascript run id=synopsis
#!/usr/bin/env node
console.log("run me with ./hello");
```

```output for=synopsis
run me with ./hello
```

The first line names the program that runs the file. The shell runs `env`, which finds `node` on the `PATH`. For the engine the line is a comment. MDN: "A hashbang comment behaves exactly like a single line-only (//) comment, except that it begins with #! and is only valid at the absolute start of a script or module."[^1]

## Semantics

**r1. Location.** "Hashbang Comments are location-sensitive and like other types of comments are discarded from the stream of input elements for the syntactic grammar."[^2] Only the goal symbol InputElementHashbangOrRegExp, which is used "at the start of a Script or Module", has the HashbangComment token.[^2] So the `#!` must be the first two code points of the text. MDN adds that "no whitespace of any kind is permitted before the #!", and that "only one such comment is permitted".[^1] Example: [`where`](#where).

**r2. Content.** The comment runs from `#!` to the end of the first line, like a `//` comment ([[javascript/comments]]).[^2] [^1] The engine does not read the path: the line "only has semantic meaning to the shell if the script is directly run in a shell".[^1]

**r3. Scripts, modules and `eval`.** A hashbang works in both goals: a script and a module.[^3] The proposal answers the question of `eval` and `Function`: "Since eval uses Script it does support Hashbang comments. Since Function uses FunctionBody, it does not."[^3] Example: [`where`](#where).

**r4. Running a file as a command.** The Node.js guide calls this first line the shebang, which "tells the OS which interpreter to use for running the script". The line `#!/usr/bin/env node` makes the shell run `env`, which starts `node`, and the file needs the executable bit (`chmod u+x app.js`).[^4] The arguments after the command name go to `process.argv` as usual ([[javascript/running-javascript-with-node-js]]). Example: [`as-command`](#as-command).

**r5. Origin.** Hosts for the command line, such as Node.js, accepted the line before the standard did, and stripped it "from the source text before being passed to the engine".[^1] The proposal moved this step into the engine: it "is to match de-facto usage in some CLI JS hosts that allow for Shebangs / Hashbang".[^3]

**r6. A byte order mark.** "A byte-order mark (BOM) character at the beginning of a source text will prevent a subsequent #! from being interpreted as a hashbang."[^3] MDN warns that a Unix shell "will not recognize the hashbang if it's preceded by a BOM character", and asks for UTF-8 without a BOM.[^1] Example: [`where`](#where).

## Examples

The examples run with Node.js 24.

The first line of this program is a hashbang, and Node.js reads it as a comment:

```javascript run id=hashbang-file
#!/usr/bin/env node
console.log("the hashbang line is a comment");
```

```output for=hashbang-file
the hashbang line is a comment
```

A file run as a command (rule r4). The example writes the file `hello` without an extension, sets the executable bit and runs the file itself, not `node`. The `PATH` gets the folder of the running `node`, so `env` finds it:

```javascript run id=as-command
const { spawnSync } = require("node:child_process");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");

const dir = fs.mkdtempSync(path.join(os.tmpdir(), "hashbang-"));
const file = path.join(dir, "hello");
fs.writeFileSync(file, "#!/usr/bin/env node\nconsole.log('hello, arguments:', process.argv.slice(2));\n");
fs.chmodSync(file, 0o755);

const env = { ...process.env, PATH: path.dirname(process.execPath) + path.delimiter + process.env.PATH };
const result = spawnSync(file, ["--verbose", "a.txt"], { encoding: "utf8", env });
console.log(result.stdout.trim());
fs.rmSync(dir, { recursive: true });
```

```output for=as-command
hello, arguments: [ '--verbose', 'a.txt' ]
```

Where a hashbang is allowed (rules r1, r3 and r6). Only the first case puts `#!` at the very start of a Script. The last line runs a module that starts with a hashbang:

```javascript run id=where
const cases = [
  ["eval, at the start", () => eval("#!/usr/bin/env node\n6 * 7")],
  ["eval, after a space", () => eval(" #!/usr/bin/env node\n6 * 7")],
  ["eval, after a BOM", () => eval("\uFEFF#!/usr/bin/env node\n6 * 7")],
  ["eval, on line 2", () => eval("6 * 7;\n#!/usr/bin/env node")],
  ["Function body", () => new Function("#!/usr/bin/env node\nreturn 6 * 7")()],
];
for (const [name, run] of cases) {
  try {
    console.log(name.padEnd(20), run());
  } catch (error) {
    console.log(name.padEnd(20), error.name);
  }
}

const { execFileSync } = require("node:child_process");
const source = "#!/usr/bin/env node\nexport {};\nconsole.log('module:', typeof require);";
console.log(execFileSync(process.execPath, ["--input-type=module", "-e", source]).toString().trim());
```

```output for=where
eval, at the start   42
eval, after a space  SyntaxError
eval, after a BOM    SyntaxError
eval, on line 2      SyntaxError
Function body        SyntaxError
module: undefined
```

A hashbang after a comment is not on the first line (rule r1):

```javascript fails id=second-line expect=exception:SyntaxError
// a comment comes first
#!/usr/bin/env node
console.log("never runs");
```

Node.js reports `SyntaxError: Invalid or unexpected token` at line 2.

## Pitfalls

### A license header above the hashbang

Tools that add a comment at the top of each file push the hashbang to line 2, and the file then fails to parse ([`second-line`](#second-line)). The hashbang must stay first.[^1]

### A byte order mark

An editor that saves UTF-8 with a BOM breaks the command: the shell does not see the `#!` (rule r6). Save the file as UTF-8 without a BOM.[^1]

### `#!` as a general comment

MDN: "You must only use the #! comment style to specify a JavaScript interpreter. In all other cases just use a // comment (or multiline comment)."[^1]

## Comparison

| Topic | JavaScript | Python |
|---|---|---|
| hashbang line | a HashbangComment, only at the start[^2] | an ordinary `#` comment[^5] |
| other comments on line 1 or 2 | not special | a `coding=` comment declares the encoding[^5] |

In Python, `#` starts every comment, so a shebang line needs no rule of its own.[^5] See [[python/running-a-python-program]].

## References
