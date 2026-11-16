```cpp run id=copy-constructor-and-shared-ptr
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Team {
    std::string name;
    std::vector<std::string> members;
};

struct SharedTeam {
    std::shared_ptr<std::vector<std::string>> members;
};

void print(const std::vector<std::string>& v) {
    for (const auto& s : v) std::cout << s << ' ';
    std::cout << '\n';
}

int main() {
    Team a{"red", {"Ann", "Bo"}};
    Team b = a;  // the implicit copy constructor copies each member
    b.members.push_back("Cy");
    print(a.members);
    print(b.members);

    SharedTeam s{std::make_shared<std::vector<std::string>>(
        std::vector<std::string>{"Ann"})};
    SharedTeam t = s;  // copies the pointer, not the vector
    t.members->push_back("Cy");
    print(*s.members);
    std::cout << s.members.use_count() << '\n';
}
```

```output for=copy-constructor-and-shared-ptr
Ann Bo 
Ann Bo Cy 
Ann Cy 
2
```

- Verdict: native. `Team b = a;` calls the copy constructor, which the compiler writes when the class does not declare one. It copies each member with that member's own copy constructor.
- `std::vector` and `std::string` copy their elements, so the copy of `Team` is deep.
- A `std::shared_ptr` member copies the pointer and raises the use count; a raw pointer member copies only the address. Such a class needs its own copy constructor if the copy must own its data.
