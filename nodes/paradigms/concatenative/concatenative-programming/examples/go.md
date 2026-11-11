```go run id=compile-words
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type Stack []int
type Word func(Stack) Stack

func Push(n int) Word {
	return func(s Stack) Stack { return append(s[:len(s):len(s)], n) }
}

func Lift2(f func(a, b int) int) Word {
	return func(s Stack) Stack {
		n := len(s)
		out := append(Stack{}, s[:n-2]...)
		return append(out, f(s[n-2], s[n-1]))
	}
}

var dict = map[string]Word{
	"+":   Lift2(func(a, b int) int { return a + b }),
	"*":   Lift2(func(a, b int) int { return a * b }),
	"dup": func(s Stack) Stack { return append(s[:len(s):len(s)], s[len(s)-1]) },
}

// Compile turns text into one word: the composition of its words.
func Compile(text string) Word {
	var words []Word
	for _, t := range strings.Fields(text) {
		if w, ok := dict[t]; ok {
			words = append(words, w)
		} else if n, err := strconv.Atoi(t); err == nil {
			words = append(words, Push(n))
		} else {
			panic("unknown word " + t)
		}
	}
	return func(s Stack) Stack {
		for _, w := range words {
			s = w(s)
		}
		return s
	}
}

func main() {
	dict["square"] = Compile("dup *")
	fmt.Println(Compile("2 3 + square")(nil))
	fmt.Println(Compile("4 square square")(Stack{1}))
}
```

```output for=compile-words
[25]
[1 256]
```

- Verdict: absent. Go applies functions to arguments. A dictionary from names to functions and a loop give a small concatenative language, and a new word is the compiled text of older words, as `square` is. The full slice expression `s[:len(s):len(s)]` makes `append` copy, so a word never changes the stack that it received.
