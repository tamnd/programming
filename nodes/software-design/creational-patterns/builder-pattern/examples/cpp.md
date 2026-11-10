```cpp run id=request
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

struct RequestOptions {
    std::string method = "GET";
    std::vector<std::string> headers = {};
    int timeout = 30;
    int retries = 0;
};

class Request {
public:
    Request(std::string url, RequestOptions options = {}) : url_(std::move(url)), options_(std::move(options)) {
        if (options_.timeout <= 0) {
            throw std::invalid_argument("timeout must be positive");
        }
    }

    void show() const {
        std::string headers;
        for (const auto& header : options_.headers) {
            headers += (headers.empty() ? "" : ", ") + header;
        }
        std::printf("%s %s timeout=%d retries=%d headers=[%s]\n", options_.method.c_str(), url_.c_str(),
                    options_.timeout, options_.retries, headers.c_str());
    }

private:
    std::string url_;
    RequestOptions options_;
};

int main() {
    Request("https://example.com/orders").show();
    Request("https://example.com/orders",
            {.method = "POST", .headers = {"Accept: application/json"}, .timeout = 5, .retries = 2})
        .show();
    try {
        Request("https://example.com/orders", {.timeout = 0});
    } catch (const std::invalid_argument& error) {
        std::printf("error: %s\n", error.what());
    }
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: native. Designated initializers on an aggregate with default member initializers give named, optional parts. Each member that the initializer leaves out takes its default member initializer.
- The designators must follow the order of the members in the declaration. `{.timeout = 5, .method = "POST"}` is ill-formed, so the call sites read in one fixed order.
- The options struct holds the defaults, and the constructor of `Request` checks the whole set, as `build` does.
- A designated initializer works only for an aggregate: a class with no user-declared constructors and no private members. So the checked product, `Request`, takes the aggregate as a parameter.
