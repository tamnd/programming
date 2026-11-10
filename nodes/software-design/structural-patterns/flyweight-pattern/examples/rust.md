```rust run id=glyphs
use std::collections::HashMap;
use std::rc::Rc;

/// A flyweight holds only the intrinsic state: the character and its width.
struct Glyph {
    symbol: char,
    width: usize,
}

impl Glyph {
    /// The column is extrinsic state: the caller passes it in.
    fn draw(&self, column: usize) -> String {
        format!("{}@{}", self.symbol, column)
    }
}

/// The flyweight factory: one shared Glyph for each character.
#[derive(Default)]
struct GlyphFactory {
    glyphs: HashMap<char, Rc<Glyph>>,
}

impl GlyphFactory {
    fn get(&mut self, symbol: char) -> Rc<Glyph> {
        let glyph = self.glyphs.entry(symbol).or_insert_with(|| {
            let width = match symbol {
                'i' | 'l' | 'r' => 1,
                'm' | 'w' => 3,
                _ => 2,
            };
            Rc::new(Glyph { symbol, width })
        });
        Rc::clone(glyph)
    }
}

fn main() {
    let mut factory = GlyphFactory::default();
    let line: Vec<Rc<Glyph>> = "abracadabra".chars().map(|c| factory.get(c)).collect();

    let mut column = 0;
    let mut drawn = Vec::new();
    for glyph in &line {
        drawn.push(glyph.draw(column));
        column += glyph.width;
    }
    println!("{}", drawn.join(" "));
    println!("characters: {}, glyph objects: {}", line.len(), factory.glyphs.len());
    println!(
        "first and last a are one object: {}",
        Rc::ptr_eq(&line[0], &line[line.len() - 1])
    );
    println!("references to the a glyph: {}", Rc::strong_count(&line[0]));
}
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
references to the a glyph: 6
```

- Verdict: idiom. A `HashMap` of `Rc<Glyph>`, and `Rc::clone` to hand out one more reference to the same glyph.
- `Rc` gives shared ownership of an immutable value. The type system stops a change to a shared glyph: `Rc<T>` gives out only `&T`, so a call that needs `&mut Glyph` does not compile.
- `Rc::ptr_eq` compares the two pointers, not the values. `==` on `Rc<T>` compares the values and needs `T: PartialEq`.
- The count of 6 references to the `a` glyph is 5 from the line and 1 from the factory. `Arc` is the thread-safe form.
