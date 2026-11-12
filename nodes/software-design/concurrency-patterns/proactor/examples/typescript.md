```typescript run id=proactor
type Handler = (name: string, size: number) => void;

const files: Record<string, string> = { "a.txt": "hello", "b.txt": "abc" };

// the Asynchronous Operation Processor: completes each read later
function startRead(name: string, handler: Handler): Promise<void> {
  console.log("start read", name);
  return new Promise<number>((resolve) => {
    setTimeout(() => resolve(files[name].length), 0);
  }).then((size) => handler(name, size));
}

const onRead: Handler = (name, size) => {
  console.log(`${name} done: ${size} bytes`);
};

async function main(): Promise<void> {
  const a = startRead("a.txt", onRead);
  const b = startRead("b.txt", onRead);
  await Promise.all([a, b]);
}

main();
```

```output
start read a.txt
start read b.txt
a.txt done: 5 bytes
b.txt done: 3 bytes
```

- Verdict: native. `setTimeout` stands in for the operation, and the event loop calls the handler with the result. The type `Handler` fixes the shape of each completion handler.
