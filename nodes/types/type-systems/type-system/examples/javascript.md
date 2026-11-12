```javascript run id=weak-dynamic-typing
// JavaScript checks types at run time and converts values when the types do not match.
console.log(1 + "1", "6" / "2", true + 1, [1] == 1, null == undefined);

// === does not convert, == does.
console.log(0 == "", 0 === "", "1" == 1, "1" === 1);

// A wrong operation either converts, gives NaN or throws a TypeError.
console.log("abc" * 2, undefined + 1);
try {
  const n = 5;
  n.toUpperCase();
} catch (e) {
  console.log(e.constructor.name, e.message);
}

// A function does not check the number or the types of its arguments.
function add(a, b) {
  return a + b;
}
console.log(add(1, 2), add(1), add("1", 2), add(1, 2, 3));

// Duck typing: an object with a next() method works as an iterator.
const counter = { i: 0, next() { return this.i < 3 ? { value: this.i++, done: false } : { done: true }; } };
console.log([...{ [Symbol.iterator]: () => counter }]);
```

```output for=weak-dynamic-typing
11 3 2 true true
true false true false
NaN NaN
TypeError n.toUpperCase is not a function
3 NaN 12 3
[ 0, 1, 2 ]
```

- Verdict: native. JavaScript checks types at run time and converts mismatched values, so `1 + "1"` is `"11"` and `"6" / "2"` is `3`. `==` converts before it compares, and `===` does not. A wrong arithmetic operation gives `NaN`, and a call of a value that is not a function throws a `TypeError`. A function accepts any number of arguments of any type.
