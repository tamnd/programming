```cpp run id=frames
#include <iostream>
#include <vector>

static std::vector<const int*> addrs; // the address of here in each call

struct Trace {
    int n;
    const int& here;
    ~Trace() { std::cout << "leave n=" << n << " here=" << here << "\n"; }
};

void count(int n) {
    const int here = n * 10; // a local: it lives in the frame of this call
    addrs.push_back(&here);
    Trace trace{n, here};    // its destructor runs when the call ends
    std::cout << "enter n=" << n << " here=" << here << "\n";
    if (n > 1) {
        count(n - 1);
    } else {
        bool distinct = addrs[0] != addrs[1] && addrs[1] != addrs[2] && addrs[0] != addrs[2];
        std::cout << addrs.size() << " frames, " << addrs.size() << " "
                  << (distinct ? "different" : "shared") << " addresses\n";
    }
}

int main() {
    count(3);
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
3 frames, 3 different addresses
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has a frame with its own `n` and `here`. When the call ends, the destructors of its locals run in the reverse order of their construction, and here `~Trace` prints the leave line.
