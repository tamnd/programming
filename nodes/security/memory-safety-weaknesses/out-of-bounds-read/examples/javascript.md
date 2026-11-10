```javascript wrong id=subarray
function echo(payload, claimed) {
  return payload.subarray(0, claimed);
}

const payload = new TextEncoder().encode("hello");
const reply = echo(payload, 64);
console.log(`claimed 64, reply has ${reply.length} bytes: ${new TextDecoder().decode(reply)}`);
```

```output for=subarray
claimed 64, reply has 5 bytes: hello
```

`subarray` clamps the range to the array. The program does not notice that the claimed length was wrong.

```javascript run id=check
function echo(payload, claimed) {
  if (!Number.isInteger(claimed) || claimed < 0 || claimed > payload.length) {
    throw new RangeError(`claimed ${claimed} bytes, received ${payload.length}`);
  }
  return payload.subarray(0, claimed);
}

const payload = new TextEncoder().encode("hello");
for (const claimed of [5, 64]) {
  try {
    console.log(`claimed ${claimed}: ${new TextDecoder().decode(echo(payload, claimed))}`);
  } catch (error) {
    console.log(`discarded: ${error.message}`);
  }
}
```

```output for=check
claimed 5: hello
discarded: claimed 64 bytes, received 5
```

- Verdict: native. A read out of range of an array gives `undefined`, and typed arrays read only their `ArrayBuffer`.
- In Node.js, `Buffer.allocUnsafe` returns memory that is not initialized and may contain sensitive data. A reply built from such a buffer with a claimed length leaks old data, as Heartbleed did. Use `Buffer.alloc`.
- Out-of-bounds reads in the engine and in native add-ons are outside the reach of script.
