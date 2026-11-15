```javascript run id=separators-and-a-stop-word
// Process, test, prepare: a list with separators between the items.
const items = ["x", "y", "z"];
let out = "";
let i = 0;
while (true) {
  out += items[i];
  i++;
  if (i === items.length) break;
  out += " | ";
}
console.log(out);

// Read commands until "stop".
const queue = ["ls", "cd", "stop", "rm"];
const done = [];
for (;;) {
  const cmd = queue.shift();
  if (cmd === "stop") break;
  done.push(cmd);
}
console.log(done.join(","), "left:", queue.join(","));
```

```output for=separators-and-a-stop-word
x | y | z
ls,cd left: rm
```

- Verdict: idiom. JavaScript has no middle-test loop, so both loops are endless loops with a `break` in the middle. The first adds an item 3 times and a separator 2 times. It assumes at least one item: with an empty list it would add `undefined`. `items.join(" | ")` does the same job in one call.
