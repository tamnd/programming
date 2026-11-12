```typescript run id=callbacks
function forEach<T>(items: T[], callback: (item: T) => void): void {
  for (const item of items) callback(item); // a blocking callback
}

const shout = (word: string): void => console.log(word.toUpperCase());
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

- Verdict: native. A function type such as `(item: T) => void` gives the type of the callback.
