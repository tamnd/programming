```cpp run id=glyphs
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// A flyweight holds only the intrinsic state: the character and its width.
class Glyph {
public:
    Glyph(char symbol, int width) : symbol_(symbol), width_(width) {}
    int width() const { return width_; }
    // the column is extrinsic state: the caller passes it in
    std::string draw(int column) const { return std::string(1, symbol_) + "@" + std::to_string(column); }

private:
    const char symbol_;
    const int width_;
};

// The flyweight factory: one Glyph for each character. It owns the glyphs.
class GlyphFactory {
public:
    const Glyph& get(char symbol) {
        auto& slot = glyphs_[symbol];
        if (!slot) {
            int width = (symbol == 'i' || symbol == 'l' || symbol == 'r') ? 1 : (symbol == 'm' || symbol == 'w') ? 3 : 2;
            slot = std::make_unique<const Glyph>(symbol, width);
        }
        return *slot;
    }
    std::size_t size() const { return glyphs_.size(); }

private:
    std::map<char, std::unique_ptr<const Glyph>> glyphs_;
};

int main() {
    GlyphFactory factory;
    std::vector<const Glyph*> line;
    for (char symbol : std::string("abracadabra")) {
        line.push_back(&factory.get(symbol));
    }

    int column = 0;
    std::string drawn;
    for (const Glyph* glyph : line) {
        drawn += (drawn.empty() ? "" : " ") + glyph->draw(column);
        column += glyph->width();
    }
    std::cout << drawn << '\n';
    std::cout << "characters: " << line.size() << ", glyph objects: " << factory.size() << '\n';
    std::cout << std::boolalpha << "first and last a are one object: " << (line.front() == line.back()) << '\n';
}
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
```

- Verdict: idiom. The factory owns each glyph through a `std::unique_ptr<const Glyph>`, and clients hold plain `const Glyph*` pointers that do not own.
- The pointers stay valid while the factory lives. A `std::map` does not move its nodes when it grows, and the `unique_ptr` keeps each glyph at one address in any case.
- `const` members and a `const Glyph` type stop a change to a shared glyph at compile time.
- The two pointers are compared with `==`. The program prints the result of the test and not the addresses, which change from run to run.
