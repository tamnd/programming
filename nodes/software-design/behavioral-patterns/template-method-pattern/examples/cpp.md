```cpp run id=loader
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Item = std::pair<std::string, int>;

class Loader {
public:
    virtual ~Loader() = default;

    // The public interface is non-virtual. It fixes the steps.
    std::vector<Item> load(const std::vector<std::string>& lines) const {
        std::vector<Item> records;
        for (const auto& line : lines) {
            if (line.empty() || line.front() == '#') continue;
            Item record = parse(line);
            if (accept(record)) records.push_back(std::move(record));
        }
        return records;
    }

private:
    // The steps are private and virtual. Derived classes override them but cannot call them.
    virtual Item parse(const std::string& line) const = 0;
    virtual bool accept(const Item&) const { return true; }
};

Item split(const std::string& line, char sep) {
    auto at = line.find(sep);
    return {line.substr(0, at), std::stoi(line.substr(at + 1))};
}

class CsvLoader final : public Loader {
    Item parse(const std::string& line) const override { return split(line, ','); }
};

class KeyValueLoader final : public Loader {
    Item parse(const std::string& line) const override { return split(line, '='); }
    bool accept(const Item& record) const override { return record.second > 0; }
};

void show(const char* label, const std::vector<Item>& records) {
    std::cout << label << ":";
    for (const auto& [name, count] : records) std::cout << ' ' << name << ':' << count;
    std::cout << '\n';
}

int main() {
    show("csv", CsvLoader{}.load({"# stock", "pen,3", "", "book,1"}));
    show("kv", KeyValueLoader{}.load({"pen=3", "# sold out", "cup=0"}));
}
```

```output for=loader
csv: pen:3 book:1
kv: pen:3
```

- Verdict: idiom. The non-virtual interface idiom. `load` is public and non-virtual, and the steps are private and virtual. Herb Sutter recommends this form for C++ class design, and calls it NVI.
- A derived class can override a private virtual function, but it cannot call the base version. The base class keeps control of when each step runs.
- The destructor is public and virtual, because client code may delete a derived loader through a `Loader` pointer.
- The lines here have no surrounding spaces, so `load` does not trim them. The other examples trim each line.
