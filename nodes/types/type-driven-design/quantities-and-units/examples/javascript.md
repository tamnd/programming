```javascript run id=length-in-one-unit
// Store every length in metres, and convert only at the edges
const UNIT = { m: 1, km: 1000, ft: 0.3048, mi: 1609.344 };

class Length {
  #metres;
  constructor(metres) { this.#metres = metres; }
  static of(value, unit) {
    if (!(unit in UNIT)) throw new RangeError(`unknown length unit ${unit}`);
    return new Length(value * UNIT[unit]);
  }
  plus(other) {
    if (!(other instanceof Length)) throw new TypeError("can only add a Length");
    return new Length(this.#metres + other.#metres);
  }
  in(unit) { return this.#metres / UNIT[unit]; }
}

const run = Length.of(5, "km").plus(Length.of(1, "mi"));
console.log(`${run.in("km").toFixed(3)} km = ${run.in("mi").toFixed(3)} mi`);
for (const bad of [() => run.plus(300), () => Length.of(3, "furlong")]) {
  try {
    bad();
  } catch (err) {
    console.log(`${err.name}: ${err.message}`);
  }
}
const fmt = new Intl.NumberFormat("en", { style: "unit", unit: "kilometer-per-hour" });
console.log(fmt.format(run.in("km") / 0.5));
```

```output for=length-in-one-unit
6.609 km = 4.107 mi
TypeError: can only add a Length
RangeError: unknown length unit furlong
13.219 km/h
```

- Verdict: idiom. JavaScript numbers carry no unit. The usual idiom stores each quantity in one base unit, here metres in a private field, and converts only when a value enters or leaves the program. The class rejects a bare number and an unknown unit at run time. `Intl.NumberFormat` with `style: "unit"` formats a value with a unit, but it does not check units.
