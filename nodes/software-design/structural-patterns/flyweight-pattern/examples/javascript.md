```javascript run id=glyphs
const WIDTHS = { i: 1, l: 1, r: 1, m: 3, w: 3 }; // all other letters are 2 units wide

// A flyweight holds only the intrinsic state: the character and its width.
class Glyph {
  constructor(char) {
    this.char = char;
    this.width = WIDTHS[char] ?? 2;
    Object.freeze(this); // no caller can change a shared glyph
  }
  draw(column) {
    return `${this.char}@${column}`; // the column is extrinsic: the caller passes it in
  }
}

// The flyweight factory: one Glyph for each character.
class GlyphFactory {
  #glyphs = new Map();
  get(char) {
    let glyph = this.#glyphs.get(char);
    if (glyph === undefined) {
      glyph = new Glyph(char);
      this.#glyphs.set(char, glyph);
    }
    return glyph;
  }
  get size() {
    return this.#glyphs.size;
  }
}

const factory = new GlyphFactory();
const line = [..."abracadabra"].map((char) => factory.get(char));

let column = 0;
const drawn = [];
for (const glyph of line) {
  drawn.push(glyph.draw(column));
  column += glyph.width;
}
console.log(drawn.join(" "));
console.log(`characters: ${line.length}, glyph objects: ${factory.size}`);
console.log("first and last a are one object:", line[0] === line.at(-1));
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
```

- Verdict: idiom. A `Map` from the key to the shared object, and `Object.freeze` on each object that it hands out.
- `Object.freeze` is shallow. A glyph with an array field would need a frozen array too.
- JavaScript strings are primitive values, and `===` compares them by value. A program never needs to intern a string to compare it fast in its own code.
- Methods on a class live once on its prototype object, not on each instance. This is sharing of the same kind, built into the language.
