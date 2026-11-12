```cpp run id=todo
#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Item {
    std::string title;
    bool done = false;
};

class TodoModel {
public:
    void subscribe(std::function<void()> f) { observers_.push_back(std::move(f)); }
    const std::vector<Item>& items() const { return items_; }

    void add(const std::string& title) {
        if (title.empty()) throw std::invalid_argument("a title is required");
        items_.push_back({title});
        changed();
    }

    void complete(int n) {
        if (n < 1 || n > static_cast<int>(items_.size()))
            throw std::invalid_argument("no item " + std::to_string(n));
        items_[n - 1].done = true;
        changed();
    }

private:
    void changed() const {
        for (const auto& f : observers_) f();
    }
    std::vector<Item> items_;
    std::vector<std::function<void()>> observers_;
};

class ListView {
public:
    explicit ListView(TodoModel& model) {
        model.subscribe([&model] {
            int n = 1;
            for (const auto& item : model.items())
                std::cout << "  [" << (item.done ? 'x' : ' ') << "] " << n++ << " " << item.title << "\n";
        });
    }
};

class SummaryView {
public:
    explicit SummaryView(TodoModel& model) {
        model.subscribe([&model] {
            const auto& items = model.items();
            auto done = std::count_if(items.begin(), items.end(), [](const Item& i) { return i.done; });
            std::cout << "  summary: " << items.size() - done << " open, " << done << " done\n";
        });
    }
};

class Controller {
public:
    explicit Controller(TodoModel& model) : model_(model) {}

    void handle(const std::string& line) {
        std::cout << "> " << line << "\n";
        auto space = line.find(' ');
        std::string command = line.substr(0, space);
        std::string argument = space == std::string::npos ? "" : line.substr(space + 1);
        bool number = !argument.empty() && std::all_of(argument.begin(), argument.end(), ::isdigit);
        try {
            if (command == "add") model_.add(argument);
            else if (command == "done" && number) model_.complete(std::stoi(argument));
            else std::cout << "  error: cannot read '" << line << "'\n";
        } catch (const std::invalid_argument& err) {
            std::cout << "  error: " << err.what() << "\n";
        }
    }

private:
    TodoModel& model_;
};

int main() {
    TodoModel model;
    ListView list(model);
    SummaryView summary(model);
    Controller controller(model);
    for (const char* line : {"add buy milk", "add write report", "done 1", "done 7", "rename 2"})
        controller.handle(line);
}
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. `std::function<void()>` holds each observer. The views subscribe lambdas that capture the model by reference.
- `items()` returns a `const` reference, so a view reads the vector without a copy and cannot change it.
- The lambdas hold references to the model, so the model must outlive the views' subscriptions. Here all objects live in `main`, and the model is created first.
