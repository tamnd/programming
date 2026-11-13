// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

package markdowntohtml

import (
	"bufio"
	"encoding/json"
	"os"
	"path/filepath"
	"testing"
)

type testCase struct {
	Name string `json:"name"`
	Args struct {
		Text string `json:"text"`
	} `json:"args"`
	Expect string `json:"expect"`
}

func load(t *testing.T, name string) []testCase {
	t.Helper()
	f, err := os.Open(filepath.Join("..", "..", "cases", name))
	if err != nil {
		t.Fatal(err)
	}
	defer f.Close()
	var cases []testCase
	s := bufio.NewScanner(f)
	for s.Scan() {
		if len(s.Bytes()) == 0 {
			continue
		}
		var c testCase
		if err := json.Unmarshal(s.Bytes(), &c); err != nil {
			t.Fatal(err)
		}
		cases = append(cases, c)
	}
	if err := s.Err(); err != nil {
		t.Fatal(err)
	}
	return cases
}

func TestCases(t *testing.T) {
	for _, c := range append(load(t, "basic.jsonl"), load(t, "edge.jsonl")...) {
		t.Run(c.Name, func(t *testing.T) {
			if got := MarkdownToHTML(c.Args.Text); got != c.Expect {
				t.Fatalf("got %q, want %q", got, c.Expect)
			}
		})
	}
}
