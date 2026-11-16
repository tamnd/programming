```javascript run id=deque
const d = ["b", "c"];
d.unshift("a");
d.push("d");
console.log(d.join(","), "front", d[0], "back", d.at(-1));

console.log(d.shift(), d.pop(), d.join(","));

// Both ends of an empty array give undefined.
const empty = [];
console.log(empty.shift(), empty.pop());
```

```output for=deque
a,b,c,d front a back d
a d b,c
undefined undefined
```

- Verdict: idiom. An array works as a deque: `push` and `pop` at the back, `unshift` and `shift` at the front.
- `shift` and `unshift` move every other item, so a large deque needs a ring buffer.
