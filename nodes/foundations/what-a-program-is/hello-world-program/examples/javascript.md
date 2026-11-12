```javascript run id=console-and-stdout
console.log("Hello, world!");
console.log("%s has %d letters", "world", 5);
process.stdout.write("write adds no newline");
process.stdout.write("\n");
console.log("arguments:", process.argv.slice(2));
process.exitCode = 0;
```

```output for=console-and-stdout
Hello, world!
world has 5 letters
write adds no newline
arguments: []
```

- Verdict: native. A JavaScript file runs from top to bottom, so `console.log("Hello, world!")` alone is a complete program in Node.js and in a browser console. `console.log` adds a newline and formats `%s` and `%d` placeholders. In Node.js, `process.stdout.write` writes text as it is, `process.argv` holds the arguments after the paths of the runtime and the script, and `process.exitCode` sets the exit status.
