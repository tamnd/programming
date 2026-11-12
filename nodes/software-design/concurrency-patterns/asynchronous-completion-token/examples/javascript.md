```javascript run id=act
function service(requests) {
  // replies in the reverse order; the token comes back unchanged
  return [...requests].reverse().map(([token, name]) => [token, `${name} loaded`]);
}

const pending = new Map(); // token -> state that the completion needs
const requests = [];
["image-7", "image-9"].forEach((name, i) => {
  const token = i + 1;
  pending.set(token, name);
  requests.push([token, name]);
  console.log(`send ${token}: load ${name}`);
});

for (const [token, result] of service(requests)) {
  pending.delete(token); // the token finds the state at once
  console.log(`done ${token}: ${result}`);
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. The token is a number key in a `Map`. With promises, each call returns its own promise, which plays the part of the token.
