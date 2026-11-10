```cpp run id=storage
#include <memory>
#include <print>
#include <string>

struct Buffer {
    explicit Buffer(std::string name) : name(std::move(name)) {
        std::println("acquire {}", this->name);
    }
    ~Buffer() { std::println("release {}", name); }
    std::string name;
};

int main() {
    {
        Buffer local("local");
        auto owned = std::make_unique<Buffer>("owned");
        std::println("end of block");
    }
    Buffer* raw = new Buffer("raw");
    delete raw;
}
```

```output
acquire local
acquire owned
end of block
release owned
release local
acquire raw
release raw
```

- Verdict: native. `local` has automatic storage and its destructor runs at the end of the block. The object that `std::make_unique` makes has dynamic storage, and the `std::unique_ptr` deletes it at the end of the same block.
- Objects in a block are destroyed in the reverse order of their construction, so `owned` goes before `local`.
- `new` and `delete` manage dynamic storage by hand. Every `new` needs exactly one `delete`, which is why owning pointers such as `std::unique_ptr` exist. See [[resource-acquisition-is-initialization]].
