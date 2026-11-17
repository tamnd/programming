```cpp run id=stack-adt
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Stack {
public:
    virtual ~Stack() = default;
    virtual void push(int v) = 0;
    virtual int pop() = 0;
    virtual bool empty() const = 0;
};

class ArrayStack final : public Stack {
public:
    void push(int v) override { items_.push_back(v); }
    int pop() override { int v = items_.back(); items_.pop_back(); return v; }
    bool empty() const override { return items_.empty(); }
private:
    std::vector<int> items_;
};

class LinkedStack final : public Stack {
public:
    void push(int v) override { top_ = std::make_unique<Cell>(Cell{v, std::move(top_)}); }
    int pop() override { int v = top_->v; top_ = std::move(top_->rest); return v; }
    bool empty() const override { return top_ == nullptr; }
private:
    struct Cell {
        int v;
        std::unique_ptr<Cell> rest;
    };
    std::unique_ptr<Cell> top_;
};

std::string drain(Stack& s) {
    for (int v = 1; v <= 3; ++v) s.push(v);
    std::string out;
    while (!s.empty()) {
        if (!out.empty()) out += ' ';
        out += std::to_string(s.pop());
    }
    return out;
}

int main() {
    ArrayStack a;
    LinkedStack l;
    std::cout << "ArrayStack: " << drain(a) << '\n';
    std::cout << "LinkedStack: " << drain(l) << '\n';
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. The abstract base class `Stack` has only pure virtual functions, so it names the operations. Each class keeps its representation in `private` members.
- A template that takes the stack type as a parameter would avoid the virtual calls. See the page on templates and generics.
