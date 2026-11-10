```typescript run id=change
function rebind(items: number[]): void {
  items = [0, 0, 0];
}

function append(items: number[]): void {
  items.push(4);
}

const numbers: number[] = [1, 2, 3];
rebind(numbers);
console.log("after rebind:", numbers.join(", "));
append(numbers);
console.log("after append:", numbers.join(", "));

const alias: number[] = numbers;
alias[0] = 100;
console.log("numbers:", numbers.join(", "));
console.log("alias === numbers:", alias === numbers);

const copy: number[] = [...numbers];
console.log("copy === numbers:", copy === numbers);
```

```output for=change
after rebind: 1, 2, 3
after append: 1, 2, 3, 4
numbers: 100, 2, 3, 4
alias === numbers: true
copy === numbers: false
```

- Verdict: native. TypeScript has the run-time model of JavaScript. A variable of an object type holds a reference.
- The types are removed before the program runs. They do not change how arguments are passed or how `===` compares objects.
- `const` stops a second assignment to the variable `numbers`. It does not stop changes to the array, so `alias[0] = 100` is valid.
- TypeScript has no pointer type and no call by reference.
