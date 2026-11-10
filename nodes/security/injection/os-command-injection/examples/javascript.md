```javascript wrong id=exec
const { execSync } = require("node:child_process");

function lookUp(name) {
  process.stdout.write(execSync(`echo looking up ${name}`));
}

lookUp("report.txt");
lookUp("report.txt; echo INJECTED");
```

```output for=exec
looking up report.txt
looking up report.txt
INJECTED
```

```javascript run id=exec-file
const { execFileSync } = require("node:child_process");

function lookUp(name) {
  process.stdout.write(execFileSync("echo", ["looking up", name]));
}

lookUp("report.txt");
lookUp("report.txt; echo INJECTED");
```

```output for=exec-file
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. `exec` and `execSync` start a shell and pass it the command as one string. The Node.js documentation says never to pass unsanitized user input to them.
- `execFile`, `execFileSync` and `spawn` take the program and an array of arguments, and do not start a shell by default. The option `shell: true` turns the shell back on.
- A template literal builds the same string as `+`. It is the most common form of the weakness in JavaScript.
- CodeQL queries `js/command-line-injection` and `js/shell-command-constructed-from-input` follow input to these calls.
