```typescript run id=copy
class File implements Disposable {
  readonly name: string;
  constructor(name: string) {
    this.name = name;
    console.log("open", name);
  }
  [Symbol.dispose](): void {
    console.log("close", this.name);
  }
}

function copy(src: string, dst: string, fail: boolean): void {
  using a = new File(src);
  using b = new File(dst);
  if (fail) throw new Error(`cannot read ${a.name}`);
  console.log("copy", a.name, "to", b.name);
}

copy("a.txt", "b.txt", false);
try {
  copy("a.txt", "c.txt", true);
} catch (e) {
  console.log("caught:", (e as Error).message);
}

{
  using cleanup = new DisposableStack();
  for (const n of ["x", "y", "z"]) {
    cleanup.defer(() => console.log("release", n));
  }
  console.log("all acquired");
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
open a.txt
open c.txt
close c.txt
close a.txt
caught: cannot read a.txt
all acquired
release z
release y
release x
```

- Verdict: native. TypeScript 5.2 added `using` declarations and the types `Disposable` and `DisposableStack`.
- `DisposableStack` collects cleanup actions whose number is known only at run time. Its method `defer` takes a callback, and the stack calls the callbacks in reverse order when it is disposed. The TypeScript notes compare it with the keyword `defer` in Go.
- `try` and `finally` work as in JavaScript, including the pitfall of a `return` in `finally`.
