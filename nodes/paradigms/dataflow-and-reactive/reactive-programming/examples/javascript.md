```javascript run id=proxy-effects
let active = null;
const deps = new Map(); // property name -> set of effects that read it

function reactive(obj) {
  return new Proxy(obj, {
    get(target, key) {
      if (active) {
        if (!deps.has(key)) deps.set(key, new Set());
        deps.get(key).add(active);
      }
      return target[key];
    },
    set(target, key, value) {
      target[key] = value;
      for (const effect of deps.get(key) ?? []) effect();
      return true;
    },
  });
}

function effect(fn) {
  active = fn;
  fn();
  active = null;
}

const cart = reactive({ price: 10, qty: 3, note: "" });
effect(() => console.log(`total ${cart.price * cart.qty}`));
cart.qty = 4;
cart.price = 12;
cart.note = "gift"; // no effect read note, so nothing runs
```

```output for=proxy-effects
total 30
total 40
total 48
```

- Verdict: idiom. JavaScript has events and promises but no signals or observables in the language. The `Proxy` records which effect reads which property, and a write runs the effects that depend on it. Vue works this way. RxJS gives observables, and a proposal for signals in the language is under discussion at TC39.
- This sketch keys the dependencies by property name only, so two reactive objects with the same property names would share them. Real libraries key by object and property.
