```cpp run id=act
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using Request = std::pair<int, std::string>;

std::vector<Request> service(const std::vector<Request>& requests) {
    std::vector<Request> replies;
    for (auto it = requests.rbegin(); it != requests.rend(); ++it) {  // reverse order
        replies.push_back({it->first, it->second + " loaded"});
    }
    return replies;
}

int main() {
    std::map<int, std::string> pending;
    std::vector<Request> requests;
    const std::vector<std::string> names{"image-7", "image-9"};
    for (std::size_t i = 0; i < names.size(); ++i) {
        int token = static_cast<int>(i) + 1;
        pending[token] = names[i];
        requests.push_back({token, names[i]});
        std::cout << "send " << token << ": load " << names[i] << "\n";
    }
    for (const auto& [token, result] : service(requests)) {
        pending.erase(token);  // the token finds the state at once
        std::cout << "done " << token << ": " << result << "\n";
    }
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. The token here is an integer key. A C or C++ program can also use a pointer to the state as the token.
