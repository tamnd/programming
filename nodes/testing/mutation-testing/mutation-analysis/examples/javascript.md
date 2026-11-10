```javascript run id=source-text
function argmax(xs) {
  let best = 0;
  for (let i = 1; i < xs.length; i++) {
    if (xs[i] > xs[best]) best = i;
  }
  return best;
}

// Each mutant replaces one piece of the source text of argmax.
const MUTANTS = [
  ["> to >=", "xs[i] > xs[best]", "xs[i] >= xs[best]"],
  ["> to <", "xs[i] > xs[best]", "xs[i] < xs[best]"],
  ["xs[i] to xs[best]", "xs[i] > xs[best]", "xs[best] > xs[best]"],
  ["start at 0", "let i = 1", "let i = 0"],
];

const weak = [[[1, 2, 3], 2], [[1, 3, 2], 1], [[3, 1, 2], 0]];
const strong = [...weak, [[2, 2, 1], 0]];

const passes = (f, suite) => suite.every(([xs, want]) => f(xs) === want);
const load = (text) => new Function(`return (${text});`)();

const source = argmax.toString();
if (!passes(load(source), strong)) throw new Error("the original must pass first");

console.log("mutant".padEnd(20) + "3 tests".padEnd(8) + "4 tests");
const killed = { weak: 0, strong: 0 };
for (const [name, from, to] of MUTANTS) {
  const mutant = load(source.replace(from, to));
  const cells = [];
  for (const [label, suite] of [["weak", weak], ["strong", strong]]) {
    const dead = !passes(mutant, suite);
    if (dead) killed[label]++;
    cells.push(dead ? "killed" : "lived");
  }
  console.log(name.padEnd(20) + cells[0].padEnd(8) + cells[1]);
}
console.log("killed".padEnd(20) + `${killed.weak} of 4  ${killed.strong} of 4`);
```

```output
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. JavaScript has no mutation tool in the language or in Node.js. StrykerJS mutates JavaScript and TypeScript.
- `toString` on a function defined in source returns the source text of the function. `new Function` compiles a changed copy at run time. So a JavaScript program can make its own mutants from text, as a mutation tool for a dynamic language does.
- The output matches the Python table: the tie `[2, 2, 1]` kills `> to >=`, and `start at 0` lives because it is an equivalent mutant.

A function made by `new Function` sees only the global scope, not the module scope. Here `argmax` reads the module constant `START`, so the mutant code throws `ReferenceError`. The harness counts the exception as a failed test.

```javascript wrong id=lost-scope
const START = 1;

function argmax(xs) {
  let best = 0;
  for (let i = START; i < xs.length; i++) {
    if (xs[i] > xs[best]) best = i;
  }
  return best;
}

const MUTANTS = [
  ["> to >=", "xs[i] > xs[best]", "xs[i] >= xs[best]"],
  ["> to <", "xs[i] > xs[best]", "xs[i] < xs[best]"],
  ["start at 0", "let i = START", "let i = 0"],
];

const weak = [[[1, 2, 3], 2], [[1, 3, 2], 1], [[3, 1, 2], 0]];

// Any exception counts as a failed test, so any exception kills the mutant.
const passes = (f, suite) => {
  try {
    return suite.every(([xs, want]) => f(xs) === want);
  } catch {
    return false;
  }
};
const load = (text) => new Function(`return (${text});`)();

const source = argmax.toString();
for (const [name, from, to] of MUTANTS) {
  const dead = !passes(load(source.replace(from, to)), weak);
  console.log(`${name}: ${dead ? "killed" : "lived"}`);
}
```

```output for=lost-scope
> to >=: killed
> to <: killed
start at 0: lived
```

- `> to >=` now shows as killed, though the weak suite has no tie. The `ReferenceError` killed it, not a test. `start at 0` lives only because the mutation removed the reference to `START`.
- The original code, loaded the same way, throws the same error. A harness that first runs the suite on the unchanged program finds this at once. Stryker puts a mutant that ends in a runtime error outside the mutation score, and does not count it as killed.
