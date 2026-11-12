```go run id=speech
package main

import (
	"fmt"
	"strings"
)

type KnowledgeSource struct {
	Name, Src, Dst string
	Run            func([]string) []string
}

var lexicon = map[string]string{"hel lo": "hello", "wor ld": "world"}

func main() {
	sources := []KnowledgeSource{
		{"grammar", "words", "sentence", func(w []string) []string {
			s := strings.Join(w, " ")
			return []string{strings.ToUpper(s[:1]) + s[1:] + "."}
		}},
		{"lexicon", "syllables", "words", func(syl []string) []string {
			var words []string
			for i := 0; i+1 < len(syl); i += 2 {
				words = append(words, lexicon[syl[i]+" "+syl[i+1]])
			}
			return words
		}},
		{"segmenter", "signal", "syllables", func(sig []string) []string {
			return strings.Split(sig[0], "-")
		}},
	}

	board := map[string][]string{"signal": {"hel-lo-wor-ld"}}

	// The control component selects a source that can contribute now.
	for board["sentence"] == nil {
		for _, ks := range sources {
			_, haveSrc := board[ks.Src]
			_, haveDst := board[ks.Dst]
			if haveSrc && !haveDst {
				board[ks.Dst] = ks.Run(board[ks.Src])
				fmt.Printf("%s adds %s: %s\n", ks.Name, ks.Dst, strings.Join(board[ks.Dst], " "))
				break
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

- Verdict: idiom. A map is the blackboard, and function values are the knowledge sources.
