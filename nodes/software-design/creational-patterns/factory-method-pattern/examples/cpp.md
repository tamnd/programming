```cpp run id=export
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Writer {
    virtual ~Writer() = default;
    virtual std::string header() const = 0;
    virtual std::string row(const std::string& name, int count) const = 0;
};

struct CsvWriter : Writer {
    std::string header() const override { return "name,count"; }
    std::string row(const std::string& name, int count) const override {
        return name + "," + std::to_string(count);
    }
};

struct TextWriter : Writer {
    std::string header() const override { return "Inventory"; }
    std::string row(const std::string& name, int count) const override {
        return name + ": " + std::to_string(count);
    }
};

using Rows = std::vector<std::pair<std::string, int>>;

class Exporter {
public:
    virtual ~Exporter() = default;

    std::string export_rows(const Rows& rows) const {
        const std::unique_ptr<Writer> writer = create_writer();  // the factory method
        std::string out = writer->header();
        for (const auto& [name, count] : rows) {
            out += "\n" + writer->row(name, count);
        }
        return out;
    }

protected:
    virtual std::unique_ptr<Writer> create_writer() const = 0;
};

class CsvExporter : public Exporter {
protected:
    std::unique_ptr<Writer> create_writer() const override {
        return std::make_unique<CsvWriter>();
    }
};

class TextExporter : public Exporter {
protected:
    std::unique_ptr<Writer> create_writer() const override {
        return std::make_unique<TextWriter>();
    }
};

int main() {
    const Rows rows{{"pen", 3}, {"book", 1}};
    const CsvExporter csv;
    const TextExporter text;
    for (const Exporter* exporter : {static_cast<const Exporter*>(&csv), static_cast<const Exporter*>(&text)}) {
        std::cout << exporter->export_rows(rows) << "\n";
    }
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. A pure virtual function is the factory method. It returns `std::unique_ptr<Writer>`, so the caller owns the product and the type says so.
- The method is named `export_rows` because `export` is a keyword in C++.
- `export_rows` is not virtual, so it is a template method with one hook. The factory method is called there, not in the constructor of `Exporter` (see the node page).
- A template parameter, `Exporter<CsvWriter>`, is the compile-time form with no creator subclass, as Design Patterns describes for C++.
