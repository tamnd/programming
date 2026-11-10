```javascript run id=depth
let deepest = 0;

function descend(level) {
  deepest = level;
  descend(level + 1);
}

function makeUser(name) {
  const user = { name };
  return user;
}

try {
  descend(1);
} catch (error) {
  console.log(error instanceof RangeError, error.message);
  console.log("deeper than 1000 frames:", deepest > 1000);
}

const user = makeUser("alice");
console.log(user.name);
```

```output
true Maximum call stack size exceeded
deeper than 1000 frames: true
alice
```

- Verdict: native. The engine keeps objects on its heap and manages the frames. The program cannot choose where a value lives.
- Node.js throws a `RangeError` with the message "Maximum call stack size exceeded" when the stack is full. The program can catch it, and the frames above the `catch` are gone by then.
- `makeUser` returns an object that it made in its own call. The object outlives the frame because only the reference was in the frame.
