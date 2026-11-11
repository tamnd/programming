```javascript run id=transfer
const buffer = new ArrayBuffer(4);
new Uint8Array(buffer)[0] = 42;

const moved = structuredClone(buffer, { transfer: [buffer] });
console.log(buffer.byteLength, buffer.detached);
console.log(new Uint8Array(moved)[0]);

const again = moved.transfer();
console.log(moved.detached, again.byteLength);

try {
  new Uint8Array(moved);
} catch (e) {
  console.log(e.constructor.name);
}
```

```output for=transfer
0 true
42
true 4
TypeError
```

- Verdict: absent. Objects are shared through references, and the garbage collector frees them. The language has no owner for an ordinary object.
- Transferable objects, such as `ArrayBuffer`, `MessagePort` and streams, are the exception. A transfer moves the resource to a new object or to another worker, and the old object becomes detached and unusable.
