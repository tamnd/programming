// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

package idgen

import (
	"slices"
	"sync"
	"testing"
)

var cases = []struct {
	name  string
	calls int
	want  []int64
}{
	{"three-calls", 3, []int64{2, 1, 1}},
	{"ten-calls", 10, []int64{9, 1, 1}},
	{"single-call", 1, []int64{0, 1, 1}},
	{"pair", 2, []int64{1, 1, 1}},
	{"limit", 1000000, []int64{999999, 1, 1}},
}

func TestReport(t *testing.T) {
	for _, c := range cases {
		if got := Report(c.calls); !slices.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestGoroutinesShareOneInstance(t *testing.T) {
	var wg sync.WaitGroup
	got := make([]*IdGenerator, 8)
	for i := range got {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			got[i] = Instance()
		}(i)
	}
	wg.Wait()
	for _, g := range got {
		if g != Instance() {
			t.Fatal("two instances")
		}
	}
	if Constructions() != 1 {
		t.Errorf("constructions = %d", Constructions())
	}
}
