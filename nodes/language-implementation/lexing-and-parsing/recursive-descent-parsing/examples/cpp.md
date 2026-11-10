```cpp run id=parser
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Token {
    std::string kind, text;
    std::size_t col;
};

using Result = std::pair<std::string, long long>;

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::vector<Token> tokenize(const std::string& src) {
    std::vector<Token> tokens;
    std::size_t i = 0;
    while (i < src.size()) {
        char c = src[i];
        if (c == ' ') {
            ++i;
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            std::size_t j = i;
            while (j < src.size() && std::isdigit(static_cast<unsigned char>(src[j]))) ++j;
            tokens.push_back({"num", src.substr(i, j - i), i});
            i = j;
        } else if (std::string("+-*/^()").find(c) != std::string::npos) {
            tokens.push_back({std::string(1, c), std::string(1, c), i});
            ++i;
        } else {
            throw ParseError("unexpected character '" + std::string(1, c) + "' at column " + std::to_string(i + 1));
        }
    }
    tokens.push_back({"end", "", src.size()});
    return tokens;
}

Result combine(const std::string& op, const Result& l, const Result& r) {
    long long a = l.second, b = r.second, v = 1;
    if (op == "+") v = a + b;
    else if (op == "-") v = a - b;
    else if (op == "*") v = a * b;
    else if (op == "/") v = a / b;
    else for (long long k = 0; k < b; ++k) v *= a;
    return {"(" + l.first + " " + op + " " + r.first + ")", v};
}

class Parser {
public:
    explicit Parser(const std::string& src) : tokens_(tokenize(src)) {}

    Result parse() {
        Result result = expr();
        if (peek() != "end") fail("an operator");
        return result;
    }

private:
    std::vector<Token> tokens_;
    std::size_t i_ = 0;

    const std::string& peek() const { return tokens_[i_].kind; }
    const Token& take() { return tokens_[i_++]; }

    [[noreturn]] void fail(const std::string& expected) const {
        throw ParseError("expected " + expected + " at column " + std::to_string(tokens_[i_].col + 1));
    }

    Result expr() {
        Result left = term();
        while (peek() == "+" || peek() == "-") {
            std::string op = take().kind;
            left = combine(op, left, term());
        }
        return left;
    }

    Result term() {
        Result left = unary();
        while (peek() == "*" || peek() == "/") {
            std::string op = take().kind;
            left = combine(op, left, unary());
        }
        return left;
    }

    Result unary() {
        if (peek() != "-") return power();
        take();
        auto [text, value] = unary();
        return {"(-" + text + ")", -value};
    }

    Result power() {
        Result base = atom();
        if (peek() != "^") return base;
        take();
        return combine("^", base, unary());
    }

    Result atom() {
        const Token& tok = tokens_[i_];
        if (tok.kind == "num") {
            take();
            return {tok.text, std::stoll(tok.text)};
        }
        if (tok.kind == "(") {
            take();
            Result inner = expr();
            if (peek() != ")") fail("')'");
            take();
            return inner;
        }
        fail("a number or '('");
    }
};

int main() {
    for (const std::string src : {"1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"}) {
        try {
            auto [text, value] = Parser(src).parse();
            std::cout << src << " -> " << text << " = " << value << '\n';
        } catch (const ParseError& e) {
            std::cout << src << " -> error: " << e.what() << '\n';
        }
    }
}
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
```

- Verdict: idiom. One member function per rule, and an exception class derived from `std::runtime_error` for errors in the input.
- `fail` has the attribute `[[noreturn]]`. The compiler then accepts `atom`, which ends with a call to `fail` and no `return`.
- The operator is copied into `op` before the right operand is parsed. In a call such as `combine(take().kind, left, term())`, C++ does not fix the order in which the arguments are evaluated, so `term()` could run before `take()`.
