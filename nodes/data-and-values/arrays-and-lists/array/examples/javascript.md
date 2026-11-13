```javascript run id=array-and-typed-arrays
// A JavaScript Array can grow, and an item can have any type.
const a = [10, 20, 30];
a.push("forty");
console.log(a.length, a);

// A read past the end gives undefined. A write past the end makes the array longer.
console.log(a[10]);
a[6] = 70;
console.log(a.length, a);

// Array.from with a length makes an array with a default value in each slot.
const zeros = Array.from({ length: 4 }, () => 0);
console.log(zeros);

// A typed array has a fixed length and one numeric type.
const t = new Int32Array(4);
t[1] = 5;
t[9] = 99; // ignored: index 9 is out of range
console.log(t.length, t.byteLength, Array.from(t));

// An Int8Array keeps the low 8 bits, so 200 becomes -56.
const b = new Int8Array([200, 127, -129]);
console.log(Array.from(b));

// Assignment shares the array. Spread syntax makes a shallow copy.
const shared = a;
const copy = [...a];
shared[0] = 0;
console.log(a[0], copy[0]);
```

```output
4 [ 10, 20, 30, 'forty' ]
undefined
7 [ 10, 20, 30, 'forty', <2 empty items>, 70 ]
[ 0, 0, 0, 0 ]
4 16 [ 0, 5, 0, 0 ]
[ -56, 127, 127 ]
0 10
```

- Verdict: native. Array can grow and can hold any type. A read past the end gives undefined, and a write past the end makes the array longer and leaves holes. A typed array has a fixed length and one numeric type: it ignores the write to index 9, and an Int8Array keeps the low 8 bits of each value.
