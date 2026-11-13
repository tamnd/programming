```cpp run id=ranges-and-views
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

struct Article {
    std::string title;
    int words;
    std::vector<std::string> tags;
};

int main() {
    const std::vector<Article> articles{
        {"Intro to NoSQL", 1200, {"nosql", "databases"}},
        {"Event sourcing", 800, {"architecture"}},
        {"Graph stores", 1500, {"nosql", "graphs"}},
        {"Refactoring loops", 600, {"refactoring"}},
        {"Key-value stores", 900, {"nosql"}},
    };

    // copy_if and sort are eager algorithms. take and transform are lazy views.
    std::vector<Article> nosql;
    std::ranges::copy_if(articles, std::back_inserter(nosql),
                         [](const Article& a) { return std::ranges::find(a.tags, "nosql") != a.tags.end(); });
    std::ranges::sort(nosql, std::ranges::greater{}, &Article::words);
    const char* sep = "";
    for (const auto& t : nosql | std::views::take(2) | std::views::transform(&Article::title)) {
        std::cout << sep << t;
        sep = ", ";
    }
    std::cout << '\n';

    // transform and accumulate are map and reduce.
    auto words = nosql | std::views::transform(&Article::words);
    std::cout << std::accumulate(words.begin(), words.end(), 0) << '\n';

    // transform and join are a flat map. A loop fills the map of counts.
    std::map<std::string, int> counts;
    for (const auto& t : articles | std::views::transform(&Article::tags) | std::views::join) {
        ++counts[t];
    }
    sep = "";
    for (const auto& [tag, c] : counts) {
        std::cout << sep << tag << '=' << c;
        sep = " ";
    }
    std::cout << '\n';

    int calls = 0;
    auto score = [&calls](const Article& a) {
        ++calls;
        return a.words;
    };
    auto longer = [](int s) { return s > 1000; };

    // A vector between the steps makes the pipeline eager: every score is computed.
    std::vector<int> all;
    std::ranges::transform(articles, std::back_inserter(all), score);
    for (int s : all | std::views::filter(longer) | std::views::take(1)) {
        std::cout << "eager " << s << ' ' << calls << '\n';
    }

    // Views are lazy, but the count is 4, not 1. filter calls the transform once to
    // test an item and again to read it. The loop also moves the filter to the next
    // match (800, then 1500) before take(1) ends the loop.
    calls = 0;
    for (int s : articles | std::views::transform(score) | std::views::filter(longer) | std::views::take(1)) {
        std::cout << "lazy " << s << ' ';
    }
    std::cout << calls << '\n';
}
```

```output
Graph stores, Intro to NoSQL
3600
architecture=1 databases=1 graphs=1 nosql=3 refactoring=1
eager 1200 5
lazy 1200 4
```

- Verdict: stdlib. The range algorithms copy_if, sort and transform are eager. The views take, transform, filter and join are lazy and join with |. The eager form computes all 5 scores into a vector. The lazy form gives 4 calls, not 1: filter calls the transform once to test an item and again to read it, and the loop moves the filter to the next match (800, then 1500) before take(1) ends the loop.
