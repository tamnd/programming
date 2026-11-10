```javascript run id=using
class Resource {
  constructor(name) {
    this.name = name;
    console.log("open", name);
  }

  [Symbol.dispose]() {
    console.log("close", this.name);
  }
}

function copy(fail) {
  using source = new Resource("source");
  using target = new Resource("target");
  if (fail) {
    throw new Error("disk full");
  }
  console.log("copy", source.name, "->", target.name);
}

copy(false);
try {
  copy(true);
} catch (error) {
  console.log("caught:", error.message);
}

{
  using stack = new DisposableStack();
  stack.defer(() => console.log("cleanup registered first"));
  stack.defer(() => console.log("cleanup registered second"));
  console.log("end of block");
}
```

```output
open source
open target
copy source -> target
close target
close source
open source
open target
close target
close source
caught: disk full
end of block
cleanup registered second
cleanup registered first
```

- Verdict: native. A `using` declaration calls `[Symbol.dispose]()` on its value when the block exits, in the reverse order of declaration.
- The exception from `copy(true)` reaches the `catch` after both resources close.
- `DisposableStack` collects cleanup callbacks with `defer` and runs them in reverse order when the stack is disposed.
- If both the block and a dispose method throw, the error is a `SuppressedError` that holds both. The `await using` form calls `[Symbol.asyncDispose]()`.
