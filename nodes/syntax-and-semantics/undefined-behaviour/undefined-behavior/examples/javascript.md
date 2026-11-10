```javascript run id=defined
const x = 2 ** 31 - 1;
console.log(x + 1);
console.log((x + 1) | 0);
console.log([1, 2, 3][3]);
console.log([10, 9, 1].sort());
console.log([10, 9, 1].sort((a, b) => a - b));
```

```output for=defined
2147483648
-2147483648
undefined
[ 1, 10, 9 ]
[ 1, 9, 10 ]
```

```javascript wrong id=bad-comparator
console.log([3, 1, 2].sort((a, b) => a > b));
```

```output for=bad-comparator
[ 3, 1, 2 ]
```

The comparator returns a boolean, never a negative number. It is not a consistent comparator, so the sort order is implementation-defined. A different engine can give a different order. The result above is the order that Node.js gave.

- Verdict: absent. ECMAScript gives each operation a defined result. A number is a 64-bit float, so `x + 1` does not wrap, and `| 0` converts to a 32-bit integer with a defined wrap.
- A read past the end of an array gives `undefined`. It does not read other memory.
- The specification still has implementation-defined parts. The order of `sort` with an inconsistent comparator is one of them.
- `sort()` without a comparator compares the values as strings, so `10` comes before `9`.
