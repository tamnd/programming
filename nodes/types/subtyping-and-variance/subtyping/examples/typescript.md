```typescript run id=structural
interface Shape {
  area(): number;
}

class Square {
  side: number;
  constructor(side: number) {
    this.side = side;
  }
  area(): number {
    return this.side * this.side;
  }
}

function total(shapes: readonly Shape[]): number {
  let sum = 0;
  for (const shape of shapes) {
    sum += shape.area();
  }
  return sum;
}

const circle = { radius: 1, area: () => Math.PI };
console.log(total([new Square(2), circle]).toFixed(2));

const squares: readonly Square[] = [new Square(1), new Square(3)];
const shapes: readonly Shape[] = squares;
console.log(total(shapes));

type Printer = (value: string) => void;
const printAny = (value: unknown): void => console.log("got", value);
const printName: Printer = printAny;
printName("Ada");
```

```output for=structural
7.14
10
got Ada
```

- Verdict: native. Subtyping is structural. `Square` does not declare `implements Shape`, and the object literal has no class, but both have `area`, so both are `Shape` values.
- A `readonly Square[]` is a `readonly Shape[]`.
- A function that accepts `unknown` can stand in for a function that accepts `string`, because parameters are contravariant.
- The parameters of methods are checked bivariantly. The concept page shows the run time error that this allows.
- The runner strips the types and does not run the type checker.
