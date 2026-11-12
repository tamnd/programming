```cpp run id=raii-and-init-statements
#include <iostream>
#include <map>
#include <mutex>
#include <string>

struct Guard {
    std::string name;
    explicit Guard(std::string n) : name(std::move(n)) { std::cout << "open " << name << "\n"; }
    ~Guard() { std::cout << "close " << name << "\n"; }
};

int main() {
    {   // a destructor runs at the end of the block (RAII)
        Guard a("a");
        Guard b("b");
        std::cout << "in the block\n";
    }   // b is destroyed first, then a

    // An if with an init-statement: it is local to the if and its else.
    std::map<std::string, int> ages{{"ada", 36}};
    if (auto it = ages.find("ada"); it != ages.end()) {
        std::cout << "ada is " << it->second << "\n";
    }

    // A lock is held until the end of the block.
    std::mutex m;
    int shared = 0;
    {
        std::scoped_lock lock(m);
        shared += 1;
    }
    std::cout << "shared = " << shared << "\n";

    // A lambda body is a block, and an immediately called lambda gives a value.
    const int limit = [] {
        int v = 2;
        v *= 21;
        return v;
    }();
    std::cout << "limit = " << limit << "\n";
}
```

```output for=raii-and-init-statements
open a
open b
in the block
close b
close a
ada is 36
shared = 1
limit = 42
```

- Verdict: native. Destructors run at the end of a block in reverse order of construction, so `b` closes before `a`. An `if` with an init-statement keeps `it` local to the `if`. A `scoped_lock` holds the mutex until the end of its block. A lambda that is called at once gives a value from a block.
