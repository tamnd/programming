```javascript run id=push-length-and-resizable-buffers
// An Array grows when a program pushes an item. push returns the new length.
const a = [];
console.log(a.push(10), a.push(20, 30), a);

// length is writable. A smaller length removes items, and a larger length adds holes.
a.length = 2;
console.log(a);
a.length = 4;
console.log(a, 3 in a);

// new Array(n) makes n holes, not n items. Array.from fills each slot.
console.log(new Array(3), Array.from({ length: 3 }, (_, i) => i * i));

// unshift adds at the front, so every other item moves.
const b = [2, 3];
b.unshift(1);
console.log(b);

// A resizable ArrayBuffer has a byte length and a maximum byte length.
const buf = new ArrayBuffer(8, { maxByteLength: 64 });
const ints = new Int32Array(buf); // no length: it tracks the buffer
console.log(buf.byteLength, buf.maxByteLength, ints.length);
buf.resize(16);
ints[3] = 7;
console.log(buf.byteLength, ints.length, Array.from(ints));
```

```output
1 3 [ 10, 20, 30 ]
[ 10, 20 ]
[ 10, 20, <2 empty items> ] false
[ <3 empty items> ] [ 0, 1, 4 ]
[ 1, 2, 3 ]
8 64 2
16 4 [ 0, 0, 0, 7 ]
```

- Verdict: native. push returns the new length. A smaller length removes items, and a larger length adds empty slots, so 3 in a is false. new Array(3) makes holes, while Array.from fills the slots. A resizable ArrayBuffer grows to 16 bytes, and the Int32Array without a length tracks it.
