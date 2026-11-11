```javascript run id=constants-and-switch
const START = 0, SIGN = 1, INT = 2, DOT = 3, FRAC = 4, DEAD = 5;

function step(s, ch) {
  const digit = ch >= "0" && ch <= "9";
  switch (s) {
    case START: return ch === "+" || ch === "-" ? SIGN : digit ? INT : DEAD;
    case SIGN: return digit ? INT : DEAD;
    case INT: return digit ? INT : ch === "." ? DOT : DEAD;
    case DOT:
    case FRAC: return digit ? FRAC : DEAD;
    default: return DEAD;
  }
}

function accepts(text) {
  let s = START;
  for (const ch of text) s = step(s, ch);
  return s === INT || s === FRAC;
}

const inputs = ["42", "-3.5", "1.", "+", "007", ".5", "4.2.1"];
for (const text of inputs) console.log(text, accepts(text));
const pattern = /^[+-]?[0-9]+(\.[0-9]+)?$/;
console.log("regex agrees:", inputs.every((t) => pattern.test(t) === accepts(t)));
```

```output for=constants-and-switch
42 true
-3.5 true
1. false
+ false
007 true
.5 false
4.2.1 false
regex agrees: true
```

- Verdict: idiom. JavaScript has no enum type, so the states are integer constants, and a `switch` on the current state gives the transitions. Nothing checks that every state has a case. The regular expression literal recognizes the same language.
