```javascript run id=change
function rebind(items) {
  items = [0, 0, 0];
}

function append(items) {
  items.push(4);
}

const numbers = [1, 2, 3];
rebind(numbers);
console.log("after rebind:", numbers.join(", "));
append(numbers);
console.log("after append:", numbers.join(", "));

const alias = numbers;
alias[0] = 100;
console.log("numbers:", numbers.join(", "));
console.log("alias === numbers:", alias === numbers);

const copy = [...numbers];
console.log("copy === numbers:", copy === numbers);
console.log("same elements:", copy.every((x, i) => x === numbers[i]));
```

```output for=change
after rebind: 1, 2, 3
after append: 1, 2, 3, 4
numbers: 100, 2, 3, 4
alias === numbers: true
copy === numbers: false
same elements: true
```

- Verdict: native. A variable holds a primitive value or a reference to an object. Arrays and functions are objects.
- A call copies the value of each argument into the parameter. For an object, the copied value is the reference. An assignment to the parameter does not reach the caller, and a change to the object does.
- `===` compares two objects by identity. Two distinct arrays with the same elements are not equal.
- The spread `[...numbers]` makes a shallow copy. The copy holds the same element values, and nested objects stay shared (see Pitfalls).
- JavaScript has no pointer type and no call by reference.
