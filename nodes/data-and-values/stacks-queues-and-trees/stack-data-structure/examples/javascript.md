```javascript run id=stack
const stack = [];
for (const item of ["a", "b", "c"]) {
  stack.push(item);
}
console.log(stack.join(","), "top", stack.at(-1));

console.log(stack.pop(), stack.pop(), stack.join(","));
stack.pop();
console.log("empty:", stack.length === 0 ? "yes" : "no");
console.log("pop on empty:", stack.pop());
```

```output for=stack
a,b,c top c
c b a
empty: yes
pop on empty: undefined
```

- Verdict: idiom. An array is the stack. `push` and `pop` work at its end, and `at(-1)` peeks.
- `pop` on an empty array returns `undefined` and does not throw.
