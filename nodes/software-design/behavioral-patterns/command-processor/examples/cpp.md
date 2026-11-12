```cpp run id=processor
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Command {
    virtual ~Command() = default;
    virtual std::string name() const = 0;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Insert : public Command {
public:
    Insert(std::vector<std::string>& words, std::string word) : words_(words), word_(std::move(word)) {}
    std::string name() const override { return "insert " + word_; }
    void execute() override { words_.push_back(word_); }
    void undo() override { words_.pop_back(); }

private:
    std::vector<std::string>& words_;
    std::string word_;
};

class Processor {
public:
    void doIt(std::unique_ptr<Command> command) {
        std::cout << "do: " << command->name() << "\n";
        command->execute();
        done_.push_back(std::move(command));
    }
    void undoIt() {
        std::unique_ptr<Command> command = std::move(done_.back());
        done_.pop_back();
        std::cout << "undo: " << command->name() << "\n";
        command->undo();
    }

private:
    std::vector<std::unique_ptr<Command>> done_;
};

std::string join(const std::vector<std::string>& words) {
    std::string out;
    for (const auto& w : words) out += (out.empty() ? "" : " ") + w;
    return out;
}

int main() {
    std::vector<std::string> words;
    Processor processor;
    processor.doIt(std::make_unique<Insert>(words, "hello"));
    processor.doIt(std::make_unique<Insert>(words, "world"));
    std::cout << "text: " << join(words) << "\n";
    processor.undoIt();
    std::cout << "text: " << join(words) << "\n";
}
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. `Command` is an abstract class, and the processor owns the executed commands through `std::unique_ptr`.
