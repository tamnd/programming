```javascript run id=compare-fn-and-collator
// A compare function returns a negative number, zero or a positive number.
console.log([10, 9, 1].toSorted((a, b) => a - b));

// Without one, sort compares the items as strings.
console.log([10, 9, 1].toSorted());

// A compare function that returns a boolean never says "less".
console.log([3, 1, 2, 5, 4].toSorted((a, b) => a > b));

// Chain two keys with ||: a zero from the first key falls through to the second.
const people = [
  { name: "cy", age: 31 },
  { name: "ann", age: 31 },
  { name: "bob", age: 25 },
];
const byAgeThenName = (a, b) => a.age - b.age || a.name.localeCompare(b.name);
console.log(people.toSorted(byAgeThenName).map((p) => p.name));

// Swap the arguments to reverse the order.
console.log(people.toSorted((a, b) => byAgeThenName(b, a)).map((p) => p.name));

// Intl.Collator gives a compare function for text.
const collator = new Intl.Collator("en", { numeric: true });
console.log(["item10", "item9", "item1"].toSorted(collator.compare));
```

```output
[ 1, 9, 10 ]
[ 1, 10, 9 ]
[ 3, 1, 2, 5, 4 ]
[ 'bob', 'ann', 'cy' ]
[ 'cy', 'ann', 'bob' ]
[ 'item1', 'item9', 'item10' ]
```

- Verdict: native. `(a, b) => a - b` sorts numbers. Without a compare function, `toSorted` compares the items as strings, so `10` comes before `9`. A compare function that returns a boolean gives a wrong order and no error. `||` passes a zero from the age to the name, and swapped arguments reverse the order. `Intl.Collator` with `numeric: true` puts `item9` before `item10`.
