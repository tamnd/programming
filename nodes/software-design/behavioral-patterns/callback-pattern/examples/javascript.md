```javascript run id=callbacks
function forEach(items, callback) {
  for (const item of items) callback(item); // a blocking callback
}

const shout = (word) => console.log(word.toUpperCase());
forEach(["hello", "world"], shout);

setTimeout(() => console.log("saved"), 0); // an asynchronous callback
console.log("request sent");
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: native. Functions are values. `setTimeout()` takes an asynchronous callback, so "saved" comes after "request sent".
