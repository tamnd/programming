```cpp run id=folded
#include <map>
#include <print>
#include <string>
#include <vector>

// Manual instrumentation: a Scope object at the top of a function records the
// path of calls that led to it, one line per path, as a flame graph expects.
class Scope {
  public:
    explicit Scope(const char *name) {
        stack().push_back(name);
        std::string path;
        for (const auto &frame : stack()) {
            path += (path.empty() ? "" : ";") + frame;
        }
        folded()[path]++;
    }
    ~Scope() { stack().pop_back(); }
    Scope(const Scope &) = delete;
    Scope &operator=(const Scope &) = delete;

    static std::map<std::string, int> &folded() {
        static std::map<std::string, int> counts;
        return counts;
    }

  private:
    static std::vector<std::string> &stack() {
        thread_local std::vector<std::string> frames;
        return frames;
    }
};

std::vector<std::string> split(const std::string &line) {
    Scope scope("split");
    std::vector<std::string> fields{""};
    for (char c : line) {
        if (c == ',') {
            fields.emplace_back();
        } else {
            fields.back().push_back(c);
        }
    }
    return fields;
}

bool check(const std::vector<std::string> &fields) {
    Scope scope("check");
    for (const auto &field : fields) {
        if (field.empty()) {
            return false;
        }
    }
    return true;
}

int load(const std::vector<std::string> &lines) {
    Scope scope("load");
    int good = 0;
    for (const auto &line : lines) {
        good += check(split(line));
    }
    return good;
}

int main() {
    {
        Scope scope("main");
        std::println("good lines: {}", load({"a,b", "c,d", "e,"}));
        check({"x"});
    }
    for (const auto &[path, count] : Scope::folded()) {
        std::println("{} {}", path, count);
    }
}
```

```output for=folded
good lines: 2
main 1
main;check 1
main;load 1
main;load;check 3
main;load;split 3
```

- Verdict: library. C++ has no profiler in the standard library. Tools outside the language, such as `perf` and `gprof`, sample or count the compiled program, as for C.
- The `Scope` object uses RAII: the constructor pushes a frame and the destructor pops it, also when the function returns early.
- The output is the folded format of flame graph tools: a path of names and a count on each line.
