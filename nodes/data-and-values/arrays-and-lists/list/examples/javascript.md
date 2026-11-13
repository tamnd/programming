```javascript run id=array-as-list
// JavaScript has one list type: Array. Its items keep their order.
const xs = ["b", "a", "b"];
xs.push("c");
xs.splice(1, 0, "z"); // insert "z" at index 1
console.log(xs, xs.length, xs.indexOf("b"), xs.lastIndexOf("b"));

// The ES2023 copying methods return a new array and leave the old one as it was.
const sorted = xs.toSorted();
const changed = xs.with(0, "B");
console.log(sorted, changed, xs);

// Object.freeze makes a read-only array. An assignment to an item fails without an error
// outside strict mode, but push always throws.
const fixed = Object.freeze([1, 2, 3]);
fixed[0] = 9;
console.log(fixed[0]);
try {
  fixed.push(4);
} catch (e) {
  console.log(e.constructor.name);
}

// Two arrays with the same items are different objects.
console.log([1, 2] === [1, 2], JSON.stringify([1, 2]) === JSON.stringify([1, 2]));
```

```output
[ 'b', 'z', 'a', 'b', 'c' ] 5 0 3
[ 'a', 'b', 'b', 'c', 'z' ] [ 'B', 'z', 'a', 'b', 'c' ] [ 'b', 'z', 'a', 'b', 'c' ]
1
TypeError
false true
```

- Verdict: native. Array is the only list type. splice inserts at an index, and toSorted and with return a new array and leave the old one as it was. A frozen array ignores an item assignment outside strict mode, but push throws TypeError. === compares references, so two arrays with the same items are not equal.
