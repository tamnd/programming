```javascript run id=copy
class File {
  constructor(name) {
    this.name = name;
    console.log("open", name);
  }
  [Symbol.dispose]() {
    console.log("close", this.name);
  }
}

function copy(src, dst, fail) {
  using a = new File(src);
  using b = new File(dst);
  if (fail) throw new Error(`cannot read ${a.name}`);
  console.log("copy", a.name, "to", b.name);
}

copy("a.txt", "b.txt", false);
try {
  copy("a.txt", "c.txt", true);
} catch (e) {
  console.log("caught:", e.message);
}

function label() {
  let text = "try";
  try {
    return text;
  } finally {
    text = "finally";
    console.log("finally ran; text is now", text);
  }
}
console.log("label() returned", label());
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
finally ran; text is now finally
label() returned try
```

- Verdict: native. `try` with `finally` runs on every exit. A `using` declaration calls `[Symbol.dispose]()` when the block ends, in reverse order of declaration.
- `using` comes from the proposal Explicit Resource Management, which reached stage 4 at TC39. Its expected year of publication in the standard is 2027. Node.js 26 runs it.
- The `return` in `label` evaluates `text` before `finally` runs, so the change in `finally` does not change the result.

A `return` in `finally` overrides the `return` or the `throw` of the `try` block. The error in the second call disappears.

```javascript wrong id=finally-return
function save(record) {
  try {
    if (!record) throw new Error("empty record");
    return "saved";
  } finally {
    return "done";
  }
}

console.log(save({ id: 1 }));
console.log(save(null));
```

```output
done
done
```
