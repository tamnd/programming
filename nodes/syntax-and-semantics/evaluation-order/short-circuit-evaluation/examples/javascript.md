```javascript run id=nullish-and-optional-chaining
const log = [];
const f = (name, v) => (log.push(name), v);

// && and || return an operand, and the right one runs only when needed.
console.log(f("a", 0) && f("b", 1), f("a", "x") || f("b", "y"), log.join(","));

// ?? replaces only null and undefined; || also replaces 0 and "".
const cfg = { retries: 0, name: "" };
console.log(cfg.retries || 5, cfg.retries ?? 5, JSON.stringify(cfg.name || "anon"), JSON.stringify(cfg.name ?? "anon"));

// ?. stops the whole chain when the left side is null or undefined.
const user = null;
log.length = 0;
console.log(user?.profile.getName(f("arg", 1)), log.length);

// Logical assignment assigns only when the operator would evaluate the right side.
const opts = { size: 10 };
opts.size ??= f("default size", 99);
opts.color ??= "red";
console.log(opts, log);
```

```output for=nullish-and-optional-chaining
0 x a,a
5 0 "anon" ""
undefined 0
{ size: 10, color: 'red' } []
```

- Verdict: native. `&&` and `||` return an operand and run the right one only when needed. `||` replaces `0` and `""`, and `??` replaces only `null` and `undefined`. `?.` stops the whole chain at a null value, so the argument of `getName` is not computed. `??=` assigns, and computes its right side, only when the property is null or undefined.
