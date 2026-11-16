```javascript run id=switch-js
function kind(fruit) {
  switch (fruit) {
    case "apple":
    case "pear":
      return "pome";
    case "cherry":
    case "plum":
      return "stone fruit";
    default:
      return "other";
  }
}

for (const fruit of ["pear", "plum", "kiwi"]) {
  console.log(fruit, kind(fruit));
}

// switch (true) tests conditions, like an if chain.
function size(n) {
  switch (true) {
    case n < 10:
      return "small";
    case n < 100:
      return "medium";
    default:
      return "large";
  }
}

console.log(size(5), size(50), size(500));
```

```output for=switch-js
pear pome
plum stone fruit
kiwi other
small medium large
```

- Verdict: native. A JavaScript `switch` can test a string, and two labels in a row share one group. Each case ends with `return`. `switch (true)` compares `true` with each case expression in order, so it works like an `if` chain.
