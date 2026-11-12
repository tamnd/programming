```javascript run id=byte-offsets-in-a-buffer
const buf = new ArrayBuffer(8); // 8 bytes; JavaScript gives offsets, not addresses
const words = new Uint32Array(buf); // two 4-byte views on the same memory
const bytes = new Uint8Array(buf);
words[0] = 0x11223344;
console.log("bytes:", Array.from(bytes, (b) => b.toString(16).padStart(2, "0")).join(" "));

const view = new DataView(buf);
console.log("little-endian read:", view.getUint32(0, true).toString(16));
console.log("big-endian read:   ", view.getUint32(0, false).toString(16));

const tail = new Uint8Array(buf, 4, 4); // byteOffset 4: the second word
tail[0] = 0xff;
console.log("word 1 after a write at offset 4:", words[1]);
```

```output for=byte-offsets-in-a-buffer
bytes: 44 33 22 11 00 00 00 00
little-endian read: 11223344
big-endian read:    44332211
word 1 after a write at offset 4: 255
```

- Verdict: absent. JavaScript has no addresses, so the closest idea is a byte offset in an `ArrayBuffer`. A `Uint32Array` and a `Uint8Array` on the same buffer show the bytes of 0x11223344 in little-endian order. A `DataView` reads at a byte offset in the byte order that the call names, and a write through the byte view at offset 4 changes word 1 of the word view.
