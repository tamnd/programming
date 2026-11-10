```cpp run id=undo
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Editor;

// The memento. The caretaker can copy, move and destroy it, but only Editor
// can make one or read its state.
class Memento {
private:
    friend class Editor;
    Memento(std::string text, std::size_t cursor) : text_(std::move(text)), cursor_(cursor) {}
    std::string text_;
    std::size_t cursor_;
};

// The originator.
class Editor {
public:
    void type(const std::string& chars) {
        text_.insert(cursor_, chars);
        cursor_ += chars.size();
    }
    void move_to(std::size_t position) { cursor_ = std::min(position, text_.size()); }
    Memento save() const { return Memento(text_, cursor_); }
    void restore(Memento memento) {
        text_ = std::move(memento.text_);
        cursor_ = memento.cursor_;
    }
    std::string show() const { return text_.substr(0, cursor_) + "|" + text_.substr(cursor_); }

private:
    std::string text_;
    std::size_t cursor_ = 0;
};

// The caretaker. It keeps mementos and cannot look inside them.
class History {
public:
    explicit History(Editor& editor) : editor_(editor) {}
    void save() { undo_.push_back(editor_.save()); }
    bool undo() {
        if (undo_.empty()) return false;
        Memento memento = std::move(undo_.back());
        undo_.pop_back();
        editor_.restore(std::move(memento));
        return true;
    }

private:
    Editor& editor_;
    std::vector<Memento> undo_;
};

int main() {
    Editor editor;
    History history(editor);

    history.save();
    editor.type("Hello");
    std::cout << editor.show() << '\n';
    history.save();
    editor.type(" world");
    std::cout << editor.show() << '\n';
    history.save();
    editor.move_to(5);
    editor.type(",");
    std::cout << editor.show() << '\n';

    for (int i = 0; i < 4; ++i) {
        std::cout << (history.undo() ? "undo: " + editor.show() : "nothing to undo") << '\n';
    }
}
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. This is the form that Design Patterns describes for C++: the memento's state and constructor are `private`, and the originator is a `friend`. The caretaker sees only the copy and move operations, which the compiler declares as public.
- `save` copies the `std::string`, because the copy constructor of `std::string` copies its characters. The memento owns its own text.
- `restore` takes the memento by value, and `undo` moves it out of the vector, so the text is moved back into the editor with no second copy.
