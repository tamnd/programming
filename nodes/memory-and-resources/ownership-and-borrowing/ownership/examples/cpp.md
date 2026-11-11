```cpp run id=unique
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Track {
    std::string name;
};

class Playlist {
public:
    void add(std::unique_ptr<Track> track) { tracks_.push_back(std::move(track)); }
    const Track* first() const { return tracks_.front().get(); }

private:
    std::vector<std::unique_ptr<Track>> tracks_;
};

int main() {
    auto track = std::make_unique<Track>(Track{"intro"});
    Playlist playlist;
    playlist.add(std::move(track));
    std::cout << (track == nullptr ? "moved, now null" : "still owned") << "\n";

    const Track* view = playlist.first();
    std::cout << view->name << "\n";
}
```

```output for=unique
moved, now null
intro
```

- Verdict: stdlib. `std::unique_ptr` owns one object and deletes it when it goes out of scope. It can be moved and cannot be copied, so the type system makes sure that there is one owner.
- A function that takes `std::unique_ptr<T>` by value takes ownership. A raw pointer or a reference is a non-owning view by convention, as the C++ Core Guidelines recommend.
- The compiler does not check that a view lives shorter than its owner. Moved-from objects can also still be used, and for `std::unique_ptr` the moved-from value is null.

```cpp norun reason=ub id=two-owners
#include <memory>

int main() {
    int* raw = new int(7);
    std::unique_ptr<int> a(raw);
    std::unique_ptr<int> b(raw);
}
```

Two `unique_ptr` objects take ownership of the same pointer, and both delete it. The double delete is undefined behavior. Make the owner at the point of allocation with `std::make_unique`, and never give a raw pointer to a second owner.
