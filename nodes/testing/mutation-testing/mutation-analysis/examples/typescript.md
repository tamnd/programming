```typescript run id=schema
type Mutant = "none" | "> to >=" | "> to <" | "xs[i] to xs[best]" | "start at 0";

const MUTANTS: Exclude<Mutant, "none">[] = ["> to >=", "> to <", "xs[i] to xs[best]", "start at 0"];

function bigger(m: Mutant, a: number, b: number): boolean {
  switch (m) {
    case "> to >=":
      return a >= b;
    case "> to <":
      return a < b;
    case "none":
    case "xs[i] to xs[best]":
    case "start at 0":
      return a > b;
    default: {
      const unreachable: never = m; // a new mutant name must be handled above
      return unreachable;
    }
  }
}

// argmax with every mutant built in; m picks which one is active.
function argmax(xs: readonly number[], m: Mutant = "none"): number {
  let best = 0;
  for (let i = m === "start at 0" ? 0 : 1; i < xs.length; i++) {
    const left = m === "xs[i] to xs[best]" ? xs[best] : xs[i];
    if (bigger(m, left, xs[best])) best = i;
  }
  return best;
}

type Case = [xs: number[], want: number];
const weak: Case[] = [[[1, 2, 3], 2], [[1, 3, 2], 1], [[3, 1, 2], 0]];
const strong: Case[] = [...weak, [[2, 2, 1], 0]];

const passes = (m: Mutant, suite: Case[]): boolean => suite.every(([xs, want]) => argmax(xs, m) === want);

if (!passes("none", strong)) throw new Error("the original must pass first");

console.log("mutant".padEnd(20) + "3 tests".padEnd(8) + "4 tests");
const killed = { weak: 0, strong: 0 };
for (const m of MUTANTS) {
  const weakDead = !passes(m, weak);
  const strongDead = !passes(m, strong);
  killed.weak += Number(weakDead);
  killed.strong += Number(strongDead);
  console.log(m.padEnd(20) + (weakDead ? "killed" : "lived").padEnd(8) + (strongDead ? "killed" : "lived"));
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

- Verdict: library. TypeScript has no mutation tool of its own. StrykerJS mutates TypeScript as well as JavaScript.
- The mutant names form a union of string literal types. The `default` branch assigns `m` to a variable of type `never`, so the type checker rejects the program if a name is added to `Mutant` and not handled in `bigger`.
- `argmax` takes the mutant as a parameter, not as a global. Each call states which mutant it runs, so the tests of two mutants cannot interfere through shared state.
- A type checker removes some mutants before any test runs. A mutant that returns a string where `number` is declared does not compile. A tool for a typed language must decide whether such a mutant counts at all; Stryker leaves compile errors out of the score.
