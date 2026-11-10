```typescript run id=using
class Resource implements Disposable {
  readonly name: string;

  constructor(name: string) {
    this.name = name;
    console.log("open", name);
  }

  [Symbol.dispose](): void {
    console.log("close", this.name);
  }
}

function copy(fail: boolean): string {
  using source = new Resource("source");
  using target = new Resource("target");
  if (fail) {
    return "skipped";
  }
  return `copied ${source.name} -> ${target.name}`;
}

console.log(copy(false));
console.log(copy(true));
```

```output
open source
open target
close target
close source
copied source -> target
open source
open target
close target
close source
skipped
```

- Verdict: native. TypeScript has the JavaScript `using` declaration and types it with the `Disposable` interface, which requires a `[Symbol.dispose]()` method.
- The early `return "skipped"` still closes both resources before the caller prints the result.
- The class assigns `name` in the constructor body. The type stripping of Node does not accept parameter properties such as `constructor(readonly name: string)`.
