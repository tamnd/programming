```go run id=parser
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type token struct {
	kind, text string
	col        int
}

type result struct {
	text  string
	value int
}

type parseError struct{ msg string }

func (e parseError) Error() string { return e.msg }

func tokenize(src string) ([]token, error) {
	var tokens []token
	for i := 0; i < len(src); {
		c := src[i]
		switch {
		case c == ' ':
			i++
		case c >= '0' && c <= '9':
			j := i
			for j < len(src) && src[j] >= '0' && src[j] <= '9' {
				j++
			}
			tokens = append(tokens, token{"num", src[i:j], i})
			i = j
		case strings.IndexByte("+-*/^()", c) >= 0:
			tokens = append(tokens, token{string(c), string(c), i})
			i++
		default:
			return nil, parseError{fmt.Sprintf("unexpected character '%c' at column %d", c, i+1)}
		}
	}
	return append(tokens, token{"end", "", len(src)}), nil
}

func combine(op string, l, r result) result {
	a, b := l.value, r.value
	var v int
	switch op {
	case "+":
		v = a + b
	case "-":
		v = a - b
	case "*":
		v = a * b
	case "/":
		v = a / b
	case "^":
		v = 1
		for k := 0; k < b; k++ {
			v *= a
		}
	}
	return result{fmt.Sprintf("(%s %s %s)", l.text, op, r.text), v}
}

type parser struct {
	tokens []token
	i      int
}

func (p *parser) peek() string { return p.tokens[p.i].kind }

func (p *parser) take() token {
	p.i++
	return p.tokens[p.i-1]
}

func (p *parser) fail(expected string) {
	panic(parseError{fmt.Sprintf("expected %s at column %d", expected, p.tokens[p.i].col+1)})
}

func (p *parser) expr() result {
	left := p.term()
	for p.peek() == "+" || p.peek() == "-" {
		left = combine(p.take().kind, left, p.term())
	}
	return left
}

func (p *parser) term() result {
	left := p.unary()
	for p.peek() == "*" || p.peek() == "/" {
		left = combine(p.take().kind, left, p.unary())
	}
	return left
}

func (p *parser) unary() result {
	if p.peek() != "-" {
		return p.power()
	}
	p.take()
	r := p.unary()
	return result{"(-" + r.text + ")", -r.value}
}

func (p *parser) power() result {
	base := p.atom()
	if p.peek() != "^" {
		return base
	}
	p.take()
	return combine("^", base, p.unary())
}

func (p *parser) atom() result {
	tok := p.tokens[p.i]
	switch tok.kind {
	case "num":
		p.take()
		n, _ := strconv.Atoi(tok.text)
		return result{tok.text, n}
	case "(":
		p.take()
		inner := p.expr()
		if p.peek() != ")" {
			p.fail("')'")
		}
		p.take()
		return inner
	}
	p.fail("a number or '('")
	return result{}
}

// parse turns a panic with a parseError back into an ordinary error value.
// Any other panic is a bug in the parser, so it panics again.
func parse(src string) (res result, err error) {
	tokens, err := tokenize(src)
	if err != nil {
		return result{}, err
	}
	defer func() {
		if r := recover(); r != nil {
			pe, ok := r.(parseError)
			if !ok {
				panic(r)
			}
			err = pe
		}
	}()
	p := &parser{tokens: tokens}
	res = p.expr()
	if p.peek() != "end" {
		p.fail("an operator")
	}
	return res, nil
}

func main() {
	for _, src := range []string{"1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"} {
		r, err := parse(src)
		if err != nil {
			fmt.Printf("%s -> error: %v\n", src, err)
			continue
		}
		fmt.Printf("%s -> %s = %d\n", src, r.text, r.value)
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

- Verdict: idiom. One method per rule on a `parser` struct.
- Go functions return errors as values. Inside a deep recursion that means an `if err != nil` check after every call. This parser panics with a `parseError` instead, and the exported entry point `parse` recovers it and returns an ordinary `error`. The panic never leaves the package.
- The deferred function panics again for any value that is not a `parseError`. An index out of range then still shows as a crash.
- `atom` ends with `return result{}` after `p.fail`. The compiler does not know that `fail` never returns, so the function needs a final return.
