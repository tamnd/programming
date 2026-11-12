```cpp run id=extensions
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>

struct Extension {
    virtual ~Extension() = default;
};

class Document {
public:
    explicit Document(std::string text) : text_(std::move(text)) {}
    const std::string& text() const { return text_; }

    template <typename T>
    void add_extension() {
        extensions_[std::type_index(typeid(T))] = std::make_unique<T>(*this);
    }

    template <typename T>
    T* get_extension() const {  // nullptr if there is no such extension
        auto it = extensions_.find(std::type_index(typeid(T)));
        return it == extensions_.end() ? nullptr : static_cast<T*>(it->second.get());
    }

private:
    std::string text_;
    std::unordered_map<std::type_index, std::unique_ptr<Extension>> extensions_;
};

struct WordCount : Extension {
    explicit WordCount(const Document& doc) : doc(doc) {}
    int count() const {
        std::istringstream in(doc.text());
        std::string word;
        int n = 0;
        while (in >> word) ++n;
        return n;
    }
    const Document& doc;
};

struct Upper : Extension {
    explicit Upper(const Document& doc) : doc(doc) {}
    std::string text() const {
        std::string s = doc.text();
        for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    }
    const Document& doc;
};

void show_upper(const Document& doc) {
    if (auto* upper = doc.get_extension<Upper>()) {
        std::cout << "upper: " << upper->text() << "\n";
    } else {
        std::cout << "upper: not supported\n";
    }
}

int main() {
    Document doc("the quick brown fox");
    doc.add_extension<WordCount>();
    std::cout << "words: " << doc.get_extension<WordCount>()->count() << "\n";
    show_upper(doc);
    doc.add_extension<Upper>();
    show_upper(doc);
}
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. `std::type_index` makes a type usable as a map key. The `static_cast` is safe because each entry is stored under the type of its own object.
