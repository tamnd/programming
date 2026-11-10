```cpp run id=tree
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Entry {
public:
    explicit Entry(std::string name) : name_(std::move(name)) {}
    virtual ~Entry() = default;
    virtual long size() const = 0;
    virtual void print(std::ostream& out, int depth) const = 0;

protected:
    std::string name_;
};

class File : public Entry {
public:
    File(std::string name, long size) : Entry(std::move(name)), size_(size) {}
    long size() const override { return size_; }
    void print(std::ostream& out, int depth) const override {
        out << std::string(2 * depth, ' ') << name_ << ' ' << size_ << '\n';
    }

private:
    long size_;
};

class Directory : public Entry {
public:
    explicit Directory(std::string name) : Entry(std::move(name)) {}

    Directory& add(std::unique_ptr<Entry> child) {
        children_.push_back(std::move(child));
        return *this;
    }

    long size() const override {
        long total = 0;
        for (const auto& child : children_) {
            total += child->size();
        }
        return total;
    }

    void print(std::ostream& out, int depth) const override {
        out << std::string(2 * depth, ' ') << name_ << "/ " << size() << '\n';
        for (const auto& child : children_) {
            child->print(out, depth + 1);
        }
    }

private:
    std::vector<std::unique_ptr<Entry>> children_;
};

int main() {
    auto src = std::make_unique<Directory>("src");
    src->add(std::make_unique<File>("main.go", 1200)).add(std::make_unique<File>("util.go", 300));
    Directory project("project");
    project.add(std::make_unique<File>("readme.md", 200)).add(std::move(src));
    project.print(std::cout, 0);
}
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. An abstract base class with pure virtual functions, and a composite that owns its children through `std::unique_ptr`.
- Each child has one owner, so the destructor of the root frees the whole tree. `add` takes the `unique_ptr` by value, so the caller gives up ownership and cannot add the same child twice.
- A tree with links to the parent keeps them as raw pointers or references that do not own. An owning link in both directions would leak or free twice.
