```cpp run id=speech
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using Data = std::vector<std::string>;

struct KnowledgeSource {
    std::string name, src, dst;
    std::function<Data(const Data&)> run;
};

std::string join(const Data& d) {
    std::string out;
    for (size_t i = 0; i < d.size(); ++i) out += (i ? " " : "") + d[i];
    return out;
}

int main() {
    const std::map<std::string, std::string> lexicon = {{"hel lo", "hello"}, {"wor ld", "world"}};
    const std::vector<KnowledgeSource> sources = {
        {"grammar", "words", "sentence", [](const Data& w) {
             std::string s = join(w);
             s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
             return Data{s + "."};
         }},
        {"lexicon", "syllables", "words", [&](const Data& syl) {
             Data words;
             for (size_t i = 0; i + 1 < syl.size(); i += 2) words.push_back(lexicon.at(syl[i] + " " + syl[i + 1]));
             return words;
         }},
        {"segmenter", "signal", "syllables", [](const Data& sig) {
             Data parts;
             std::stringstream ss(sig[0]);
             for (std::string p; std::getline(ss, p, '-');) parts.push_back(p);
             return parts;
         }},
    };

    std::map<std::string, Data> board = {{"signal", {"hel-lo-wor-ld"}}};

    // The control component selects a source that can contribute now.
    while (!board.count("sentence")) {
        for (const auto& ks : sources) {
            if (board.count(ks.src) && !board.count(ks.dst)) {
                board[ks.dst] = ks.run(board[ks.src]);
                std::cout << ks.name << " adds " << ks.dst << ": " << join(board[ks.dst]) << "\n";
                break;
            }
        }
    }
}
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A `std::map` is the blackboard, and `std::function` values are the knowledge sources.
