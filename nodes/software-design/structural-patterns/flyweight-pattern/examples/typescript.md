```typescript run id=glyphs
const WIDTHS: Readonly<Record<string, number>> = { i: 1, l: 1, r: 1, m: 3, w: 3 }; // others are 2

// A flyweight holds only the intrinsic state. Only Glyph.of can make one.
class Glyph {
  static readonly #pool = new Map<string, Glyph>();

  static of(char: string): Glyph {
    let glyph = Glyph.#pool.get(char);
    if (glyph === undefined) {
      glyph = new Glyph(char, WIDTHS[char] ?? 2);
      Glyph.#pool.set(char, glyph);
    }
    return glyph;
  }

  static get count(): number {
    return Glyph.#pool.size;
  }

  readonly char: string;
  readonly width: number;

  private constructor(char: string, width: number) {
    this.char = char;
    this.width = width;
  }

  draw(column: number): string {
    return `${this.char}@${column}`; // the column is extrinsic: the caller passes it in
  }
}

const line: readonly Glyph[] = [..."abracadabra"].map(Glyph.of);

let column = 0;
const drawn: string[] = [];
for (const glyph of line) {
  drawn.push(glyph.draw(column));
  column += glyph.width;
}
console.log(drawn.join(" "));
console.log(`characters: ${line.length}, glyph objects: ${Glyph.count}`);
console.log("first and last a are one object:", line[0] === line.at(-1));
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
```

- Verdict: idiom. A static factory method and a `private` constructor, so that code outside the class cannot make a second glyph for the same character.
- `readonly` fields stop a change to a shared glyph at compile time. They do not freeze the object at run time.
- `private constructor` is a compile-time check only. The `#pool` field is private at run time as well.
