```javascript run id=freeze
"use strict";
const point = Object.freeze({ x: 1, y: 2 });
try {
  point.x = 9;
} catch (error) {
  console.log(error.constructor.name);
}
console.log(point.x, Object.isFrozen(point));

const moved = { ...point, y: 5 };
console.log(moved);

const numbers = [3, 1, 2];
console.log(numbers.toSorted(), numbers);

const word = "abc";
console.log(word.toUpperCase(), word);
```

```output for=freeze
TypeError
1 true
{ x: 1, y: 5 }
[ 1, 2, 3 ] [ 3, 1, 2 ]
ABC abc
```

- Verdict: stdlib. Strings and the other primitive values cannot change. `Object.freeze` makes the own properties of an object read-only and stops the addition of properties.
- In strict code, an assignment to a frozen property throws `TypeError`. In sloppy code, the assignment does nothing and gives no error.
- `Object.freeze` is shallow. An object inside a frozen object can still change.
- `const` makes the binding immutable, not the value. A `const` array accepts `push`.
- Spread syntax and methods such as `toSorted` make a changed copy and leave the original as it was.
