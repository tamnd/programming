```cpp run id=orders
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct Order {                                       // a value type
    std::string customer;
    int amount;
};

Order parse(const std::string& line) {               // a pure function
    std::istringstream in(line);
    Order o;
    std::getline(in, o.customer, ',');
    in >> o.amount;
    return o;
}

struct Report {                                      // an abstract class
    virtual ~Report() = default;
    virtual std::string line(const std::string& n, int t) const = 0;
};
struct Plain : Report {
    std::string line(const std::string& n, int t) const override { return n + " " + std::to_string(t); }
};
struct Csv : Report {
    std::string line(const std::string& n, int t) const override { return n + "," + std::to_string(t); }
};

int main() {
    std::istringstream text("ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5");
    std::map<std::string, int> totals;
    for (std::string line; std::getline(text, line);) {   // an imperative loop
        Order o = parse(line);
        totals[o.customer] += o.amount;
    }
    std::vector<std::pair<std::string, int>> big;
    std::ranges::copy_if(totals, std::back_inserter(big), [](const auto& p) { return p.second > 50; });
    std::ranges::sort(big, std::ranges::greater{}, &std::pair<std::string, int>::second);  // generic

    std::vector<std::unique_ptr<Report>> reports;
    reports.push_back(std::make_unique<Plain>());
    reports.push_back(std::make_unique<Csv>());
    for (const auto& r : reports)
        for (const auto& [name, total] : big) std::cout << r->line(name, total) << "\n";
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

- Verdict: native. Stroustrup designed C++ to support several styles: C-style procedural code, classes with virtual functions, generic programming with templates, and, with lambdas and ranges, a functional style. The block uses all four. The style that a part of a C++ program uses is a choice of the programmer, and a code base usually needs rules about which styles to use where.
