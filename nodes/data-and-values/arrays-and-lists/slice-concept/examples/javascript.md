```javascript run id=slice-splice-subarray
// Array.prototype.slice returns a new array. The end index is not included.
const xs = [10, 20, 30, 40, 50];
console.log(xs.slice(1, 3), xs.slice(-2), xs.slice(3, 99));

// The copy is shallow: a change to the new array does not touch xs,
// but objects inside are shared.
const people = [{ name: "Ann" }, { name: "Bo" }];
const firstOne = people.slice(0, 1);
firstOne[0].name = "Anna";
firstOne.push({ name: "Cy" });
console.log(people.length, people[0].name);

// splice is different: it removes items from the array itself and returns them.
const ys = [1, 2, 3, 4, 5];
const removed = ys.splice(1, 2);
console.log(removed, ys);

// A typed array has both: slice copies, subarray is a view of the same buffer.
const bytes = new Uint8Array([1, 2, 3, 4, 5]);
const copy = bytes.slice(1, 3);
const view = bytes.subarray(1, 3);
copy[0] = 77;
view[1] = 88;
console.log(Array.from(bytes), Array.from(copy), Array.from(view), view.byteOffset);
```

```output
[ 20, 30 ] [ 40, 50 ] [ 40, 50 ]
2 Anna
[ 2, 3 ] [ 1, 4, 5 ]
[ 1, 2, 88, 4, 5 ] [ 77, 3 ] [ 2, 88 ] 1
```

- Verdict: native. slice copies a range and cuts bounds that are out of range. The copy is shallow, so a change to a shared object shows in both arrays, but a push to the copy does not. splice removes a range from the array itself. On a typed array, slice copies and subarray is a view, so only the write through the view reaches bytes.
