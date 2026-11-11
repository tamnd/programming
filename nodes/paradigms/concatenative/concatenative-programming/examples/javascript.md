```javascript run id=word-arrays
// A word maps a stack to a stack. A program is an array of words,
// so joining two programs is array concatenation.
const push = (n) => (s) => [...s, n];
const lift2 = (f) => (s) => [...s.slice(0, -2), f(s.at(-2), s.at(-1))];
const words = {
  "+": lift2((a, b) => a + b),
  "*": lift2((a, b) => a * b),
  dup: (s) => [...s, s.at(-1)],
  swap: (s) => [...s.slice(0, -2), s.at(-1), s.at(-2)],
};

const compile = (text) =>
  text.split(/\s+/).map((t) => (t in words ? words[t] : push(Number(t))));
const run = (prog, stack = []) => prog.reduce((s, w) => w(s), stack);

const square = compile("dup *");
const sumOfSquares = [...square, ...compile("swap"), ...square, ...compile("+")];

console.log(run(compile("3 4"), []), "->", run(sumOfSquares, [3, 4]));
console.log(run([...compile("3 4"), ...sumOfSquares]));
```

```output for=word-arrays
[ 3, 4 ] -> [ 25 ]
[ 25 ]
```

- Verdict: absent. JavaScript applies functions to arguments. Because functions are values, a program can be an array of words, and two programs join with the spread operator, as `sumOfSquares` does. Method chains such as `array.filter(f).map(g)` read left to right in the same way, but each step names its function, and the value passes along one at a time instead of on a stack.
