```cpp run id=pipeline
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

struct Request {
    std::optional<std::string> user;
    std::string path;
};

struct Response {
    int status;
    std::string body;
};

using Next = std::function<Response(const Request&)>;
using Handler = std::function<Response(const Request&, const Next&)>;

// run calls handler i, and gives it a Next that calls handler i + 1.
Response run(const std::vector<Handler>& handlers, const Next& last, std::size_t i, const Request& request) {
    if (i == handlers.size()) {
        return last(request);
    }
    Next next = [&](const Request& r) { return run(handlers, last, i + 1, r); };
    return handlers[i](request, next);
}

int main() {
    Handler log = [](const Request& request, const Next& next) {
        std::cout << "log: " << request.path << " from " << request.user.value_or("nobody") << "\n";
        return next(request);
    };
    Handler auth = [](const Request& request, const Next& next) {
        if (!request.user) {
            return Response{401, "sign in first"};
        }
        return next(request);
    };
    std::map<std::pair<std::string, std::string>, Response> saved;
    Handler cache = [&saved](const Request& request, const Next& next) {
        auto key = std::pair{request.user.value_or(""), request.path};
        if (auto hit = saved.find(key); hit != saved.end()) {
            return Response{hit->second.status, hit->second.body + " (cached)"};
        }
        Response response = next(request);
        saved.emplace(key, response);
        return response;
    };
    Next app = [](const Request& request) { return Response{200, "report for " + request.user.value_or("")}; };

    std::vector<Handler> handlers{log, auth, cache};
    for (auto user : {std::optional<std::string>{"ada"}, std::optional<std::string>{}, std::optional<std::string>{"ada"}}) {
        Response response = run(handlers, app, 0, Request{user, "/report"});
        std::cout << response.status << " " << response.body << "\n";
    }
}
```

```output
log: /report from ada
200 report for ada
log: /report from nobody
401 sign in first
log: /report from ada
200 report for ada (cached)
```

- Verdict: idiom. Each handler is a `std::function` that takes the request and `next`. `run` makes `next` for handler `i` as a lambda that calls `run` for handler `i + 1`.
- The lambda for `next` captures by reference. This is safe because `next` does not outlive the call of `run` that made it.
- The cache lambda captures the map `saved` by reference, so the map must outlive the chain. Here both live in `main`.
