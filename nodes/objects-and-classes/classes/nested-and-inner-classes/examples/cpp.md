```cpp run id=nested-class-with-pointer
#include <iostream>
#include <string>
#include <vector>

class Playlist {
public:
    explicit Playlist(std::string name) : name_(std::move(name)) {}
    void add(std::string song) { songs_.push_back(std::move(song)); }

    class Cursor { // a nested class; it has no this of Playlist
    public:
        explicit Cursor(const Playlist &owner) : owner_(&owner) {}
        bool next() { return ++i_ < owner_->songs_.size(); } // private member: allowed
        std::string current() const { return owner_->name_ + ": " + owner_->songs_[i_]; }

    private:
        const Playlist *owner_;
        std::size_t i_ = static_cast<std::size_t>(-1);
    };

    Cursor cursor() const { return Cursor(*this); }

private:
    std::string name_;
    std::vector<std::string> songs_;
};

int main() {
    Playlist p("road");
    p.add("one");
    p.add("two");
    Playlist::Cursor c = p.cursor();
    while (c.next()) {
        std::cout << c.current() << "\n";
    }
}
```

```output for=nested-class-with-pointer
road: one
road: two
```

- Verdict: native. A C++ nested class is a member of the outer class, so it can use the private members of `Playlist`, but it has no `this` of an outer object. It keeps a pointer, which the code sets in the constructor.
- The pointer can outlive the `Playlist`. If `p` is destroyed while `c` is still in use, `c` reads freed memory; a Java inner object would keep its outer object alive instead.
