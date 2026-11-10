```cpp run id=editor
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// A command is a pair of functions that share state through captures.
struct Command {
    std::function<void()> execute;
    std::function<void()> undo;
};

Command append(std::string& text, std::string suffix) {
    return {[&text, suffix] { text += suffix; },
            [&text, suffix] { text.resize(text.size() - suffix.size()); }};
}

Command delete_last(std::string& text, std::size_t count) {
    auto removed = std::make_shared<std::string>();  // shared by the two lambdas
    return {[&text, count, removed] {
                *removed = text.substr(text.size() - count);
                text.resize(text.size() - count);
            },
            [&text, removed] { text += *removed; }};
}

class History {
public:
    void run(Command command) {
        command.execute();
        undo_.push_back(std::move(command));
        redo_.clear();
    }

    bool undo() { return move_top(undo_, redo_, &Command::undo); }
    bool redo() { return move_top(redo_, undo_, &Command::execute); }

private:
    static bool move_top(std::vector<Command>& from, std::vector<Command>& to, std::function<void()> Command::*step) {
        if (from.empty()) return false;
        Command command = std::move(from.back());
        from.pop_back();
        (command.*step)();
        to.push_back(std::move(command));
        return true;
    }

    std::vector<Command> undo_, redo_;
};

int main() {
    std::string text;
    History history;
    auto show = [&text](const char* label) { std::printf("%s: \"%s\"\n", label, text.c_str()); };
    history.run(append(text, "hello")); show("append");
    history.run(append(text, " world")); show("append");
    history.run(delete_last(text, 5)); show("delete");
    history.undo(); show("undo");
    history.undo(); show("undo");
    history.redo(); show("redo");
    history.run(append(text, "!")); show("append");
    if (!history.redo()) std::printf("redo: nothing to redo\n");
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. A command is a struct of two `std::function` values. Each lambda captures the receiver by reference and its arguments by value.
- The two lambdas of `delete_last` must share the removed text. A `std::shared_ptr` holds it, so both lambdas see one string, and the string lives as long as the command.
- The lambdas capture `text` by reference, so the commands must not outlive the string. Here both live in `main`. A history that outlives its document would hold dangling references.
- A function named `erase` would lose to `std::erase` here. Argument-dependent lookup finds `std::erase` for a `std::string` argument, and its template parameter matches the `int` argument exactly. So the function is `delete_last`.
- A pointer to member, `&Command::undo`, lets one function move a command between the stacks for both undo and redo.
