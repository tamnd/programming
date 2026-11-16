```cpp run id=ucontext-fringe
#include <ucontext.h>

#include <iostream>
#include <vector>

struct Tree {
    int value = 0;
    std::vector<Tree> children;  // no children: a leaf
};

Tree L(int v) { return Tree{v, {}}; }
Tree N(std::vector<Tree> children) { return Tree{0, std::move(children)}; }

// A fiber with its own stack. resume() switches to it; suspend() switches back.
class Fiber {
public:
    explicit Fiber(const Tree& tree) : tree_(tree), stack_(64 * 1024) {
        getcontext(&self_);
        self_.uc_stack.ss_sp = stack_.data();
        self_.uc_stack.ss_size = stack_.size();
        self_.uc_link = &caller_;  // when entry() returns, go back to the caller
        makecontext(&self_, &Fiber::entry, 0);
    }
    Fiber(const Fiber&) = delete;
    Fiber& operator=(const Fiber&) = delete;

    // Run the fiber until it suspends. Returns false when it has finished.
    bool resume(int& out) {
        if (done_) return false;
        Fiber* previous = current_;
        current_ = this;
        swapcontext(&caller_, &self_);
        current_ = previous;
        if (done_) return false;
        out = value_;
        return true;
    }

    // Called on the fiber's own stack: give a value to the caller and pause.
    static void suspend(int v) {
        Fiber* f = current_;
        f->value_ = v;
        swapcontext(&f->self_, &f->caller_);
    }

private:
    static void walk(const Tree& t) {
        if (t.children.empty()) {
            suspend(t.value);  // pauses here, deep in the recursion
        } else {
            for (const Tree& child : t.children) walk(child);
        }
    }

    static void entry() {
        Fiber* f = current_;
        walk(f->tree_);
        f->done_ = true;
    }

    static inline Fiber* current_ = nullptr;
    const Tree& tree_;
    std::vector<char> stack_;
    ucontext_t self_{};
    ucontext_t caller_{};
    int value_ = 0;
    bool done_ = false;
};

bool same_fringe(const Tree& x, const Tree& y) {
    Fiber fx(x), fy(y);
    while (true) {
        int a = 0, b = 0;
        bool hasA = fx.resume(a);
        bool hasB = fy.resume(b);
        if (hasA != hasB || a != b) return false;
        if (!hasA) return true;
    }
}

int main() {
    Tree a = N({N({L(1), L(2)}), N({L(3), N({L(4)})})});
    Tree b = N({L(1), N({L(2), N({L(3), L(4)})})});
    Tree c = N({N({L(1), L(2)}), N({L(3), L(5)})});

    Fiber leaves(a);
    int leaf = 0;
    while (leaves.resume(leaf)) std::cout << "got " << leaf << '\n';
    std::cout << std::boolalpha;
    std::cout << "same fringe a b: " << same_fringe(a, b) << '\n';
    std::cout << "same fringe a c: " << same_fringe(a, c) << '\n';
}
```

```output for=ucontext-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: library. C++ has no fibers in the standard library. This example builds a fiber with the `<ucontext.h>` functions of glibc; Boost.Fiber and the Win32 fiber API are other choices.
- `makecontext` makes a context that runs `entry` on a stack that the program allocates, here 64 KiB. `swapcontext` saves the current context and switches to another one.
- `walk` is an ordinary recursive function. It calls `suspend` at any depth, and the whole call stack of the fiber waits until the next `resume`.
- The POSIX standard removed these functions in 2008, but glibc still provides them. When `same_fringe` returns early, the destructors of objects on the unfinished fiber's stack never run, so in this example the fiber stack holds only references.
