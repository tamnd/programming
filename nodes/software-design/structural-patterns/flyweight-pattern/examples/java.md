```java run id=glyphs
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    /** A flyweight holds only the intrinsic state: the character and its width. */
    record Glyph(char symbol, int width) {
        String draw(int column) {
            return symbol + "@" + column; // the column is extrinsic: the caller passes it in
        }
    }

    /** The flyweight factory: one Glyph for each character. */
    static final class GlyphFactory {
        private static final Map<Character, Integer> WIDTHS = Map.of('i', 1, 'l', 1, 'r', 1, 'm', 3, 'w', 3);
        private final Map<Character, Glyph> glyphs = new HashMap<>();

        Glyph get(char symbol) {
            return glyphs.computeIfAbsent(symbol, s -> new Glyph(s, WIDTHS.getOrDefault(s, 2)));
        }

        int size() {
            return glyphs.size();
        }
    }

    public static void main(String[] args) {
        GlyphFactory factory = new GlyphFactory();
        List<Glyph> line = new ArrayList<>();
        for (char symbol : "abracadabra".toCharArray()) {
            line.add(factory.get(symbol));
        }

        int column = 0;
        List<String> drawn = new ArrayList<>();
        for (Glyph glyph : line) {
            drawn.add(glyph.draw(column));
            column += glyph.width();
        }
        System.out.println(String.join(" ", drawn));
        System.out.println("characters: " + line.size() + ", glyph objects: " + factory.size());
        System.out.println("first and last a are one object: " + (line.getFirst() == line.getLast()));
    }
}
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
```

- Verdict: idiom. A `record` for the shared state and `Map.computeIfAbsent` for the factory. The fields of a record are `final`, so a shared glyph cannot change.
- The JDK uses the pattern for boxed values. `Integer.valueOf` returns cached objects for the values from -128 to 127, and string literals are interned. The node page shows the bug that this causes with `==`.
- `Map<Character, Glyph>` boxes each key. `Character.valueOf` caches the characters from `'\u0000'` to `'\u007F'`, so these keys are flyweights as well.
- The factory is not thread-safe. A `ConcurrentHashMap` with the same `computeIfAbsent` call makes it safe to share between threads.
