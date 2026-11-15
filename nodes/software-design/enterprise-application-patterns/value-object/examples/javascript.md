```javascript run id=date-range-value
// A calendar day as a small value: three numbers.
const day = (y, m, d) => Object.freeze({ y, m, d });
const dayKey = (t) => `${t.y}-${String(t.m).padStart(2, "0")}-${String(t.d).padStart(2, "0")}`;
const before = (s, t) => dayKey(s) < dayKey(t); // ISO strings sort as dates

class DateRange {
  #start;
  #end;

  constructor(start, end) {
    if (before(end, start)) throw new Error("the end is before the start");
    this.#start = start;
    this.#end = end;
    Object.freeze(this);
  }

  // JavaScript compares objects by identity, so the value object defines equals and a key.
  equals(other) {
    return this.key() === other.key();
  }

  key() {
    return `${dayKey(this.#start)}..${dayKey(this.#end)}`;
  }

  contains(t) {
    return !before(t, this.#start) && !before(this.#end, t);
  }

  extendTo(end) {
    return new DateRange(this.#start, end); // a new value; this does not change
  }

  toString() {
    return this.key();
  }
}

const yn = (b) => (b ? "yes" : "no");

const a = new DateRange(day(2026, 3, 1), day(2026, 3, 10));
const b = new DateRange(day(2026, 3, 1), day(2026, 3, 10));
console.log("a equals b:", yn(a.equals(b)));
console.log("a and b are the same object:", yn(a === b));

const c = a.extendTo(day(2026, 3, 15));
console.log(`a: ${a}`);
console.log(`c: ${c}`);

const rooms = new Map([[a.key(), "room 4"]]); // a Map key must be a primitive to match by value
console.log("the room for b:", rooms.get(b.key()));
console.log("a contains 2026-03-05:", yn(a.contains(day(2026, 3, 5))));
console.log("a contains 2026-03-12:", yn(a.contains(day(2026, 3, 12))));

try {
  new DateRange(day(2026, 3, 10), day(2026, 3, 1));
} catch (e) {
  console.log("error:", e.message);
}
```

```output
a equals b: yes
a and b are the same object: no
a: 2026-03-01..2026-03-10
c: 2026-03-01..2026-03-15
the room for b: room 4
a contains 2026-03-05: yes
a contains 2026-03-12: no
error: the end is before the start
```

- Verdict: idiom. JavaScript compares objects by identity, and a `Map` finds an object key by identity. The class defines `equals` and a string `key()`, and the map uses the key.
- `Object.freeze(this)` and private fields stop changes to the object after the constructor.
