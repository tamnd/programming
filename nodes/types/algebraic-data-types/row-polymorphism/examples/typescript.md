```typescript run id=keep
type XY = { x: number; y: number };

function shift<P extends XY>(p: P): P {
  return { ...p, x: p.x + 1 };
}

function shiftFixed(p: XY): XY {
  return { ...p, x: p.x + 1 };
}

const home = { x: 1, y: 2, label: "home" };
const a = shift({ x: 1, y: 2 });
const b = shift(home);
console.log(`point: x=${a.x} y=${a.y}`);
console.log(`labelled: x=${b.x} y=${b.y} label=${b.label}`);

const c = shiftFixed(home);
// c.label does not compile: the type of c is XY.
console.log("label still in the object:", "label" in c ? "yes" : "no");
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
label still in the object: yes
```

- Verdict: idiom. The type parameter `P` with the constraint `extends XY` plays the part of a row variable: `shift` accepts any object with `x` and `y`, and its result has the full type of the argument, so `b.label` compiles.
- `shiftFixed` uses only structural subtyping. It accepts `home`, but its result type is `XY`, so the type checker forgets `label`, although the object still has it.
