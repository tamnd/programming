```python run id=glyphs
from dataclasses import dataclass
from functools import cache

WIDTHS = {"i": 1, "l": 1, "r": 1, "m": 3, "w": 3}  # all other letters are 2 units wide


@dataclass(frozen=True, slots=True)
class Glyph:
    """A flyweight. It holds only the intrinsic state: the character and its width."""

    char: str
    width: int

    def draw(self, column: int) -> str:
        # the column is extrinsic state: the caller passes it in
        return f"{self.char}@{column}"


@cache
def glyph(char: str) -> Glyph:
    """The flyweight factory: one Glyph for each character."""
    return Glyph(char, WIDTHS.get(char, 2))


text = "abracadabra"
line = [glyph(char) for char in text]

column = 0
drawn = []
for g in line:
    drawn.append(g.draw(column))
    column += g.width
print(" ".join(drawn))
print(f"characters: {len(line)}, glyph objects: {glyph.cache_info().currsize}")
print("first and last a are one object:", line[0] is line[-1])
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: True
```

- Verdict: stdlib. `functools.cache` turns a constructor call into a flyweight factory. It keeps one result for each argument.
- The glyph is a frozen dataclass, so no caller can change a shared glyph. `slots=True` removes the per-object `__dict__`, which matters when there are many small objects.
- The column is extrinsic. The loop computes it from the widths and passes it to `draw`. A glyph that stored its own column could not be shared.
- `sys.intern` does the same job for strings. CPython also keeps one object for each small integer from -5 to 256. The node page shows why `is` must not be used to compare such values.
