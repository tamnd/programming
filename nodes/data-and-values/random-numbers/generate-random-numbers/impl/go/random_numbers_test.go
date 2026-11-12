// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

package randomnumbers

import (
	"math"
	"regexp"
	"slices"
	"strconv"
	"testing"
)

var rangeCases = []struct {
	name   string
	words  []uint32
	lo, hi int64
	count  int
	want   []int64
}{
	{"coin", []uint32{5, 2, 7}, 0, 1, 3, []int64{1, 0, 1}},
	{"no-results", []uint32{}, 1, 6, 0, []int64{}},
	{"die", []uint32{7, 12, 1000000, 4294967295}, 1, 6, 4, []int64{2, 1, 5, 4}},
	{"negative-range", []uint32{100, 2000000000, 4294967294}, -10, 10, 3, []int64{6, -5, -8}},
	{"one-value", []uint32{0, 99}, 5, 5, 2, []int64{5, 5}},
	{"reject-low-words", []uint32{0, 3, 4}, 1, 6, 1, []int64{5}},
	{"accept-least", []uint32{3, 4, 9}, 0, 5, 2, []int64{4, 3}},
	{"power-of-two", []uint32{0, 4294967295}, 0, 7, 2, []int64{0, 7}},
	{"worst-case", []uint32{2147483646, 2147483647}, 0, 2147483648, 1, []int64{2147483647}},
	{"full-unsigned-32", []uint32{0, 4294967295, 123456789}, 0, 4294967295, 3, []int64{0, 4294967295, 123456789}},
	{"full-signed-32", []uint32{0, 2147483648, 4294967295}, -2147483648, 2147483647, 3, []int64{-2147483648, 0, 2147483647}},
	{"near-int64-max", []uint32{4294967295, 6}, 9223372036854775798, 9223372036854775807, 2, []int64{9223372036854775803, 9223372036854775804}},
	{"near-int64-min", []uint32{4294967295, 9}, math.MinInt64, -9223372036854775799, 2, []int64{-9223372036854775803, -9223372036854775799}},
	{"wide-int64", []uint32{4294967295, 0}, 9223372032559808512, 9223372036854775807, 2, []int64{9223372036854775807, 9223372032559808512}},
}

var shuffleCases = []struct {
	name  string
	words []uint32
	items []string
	want  []string
}{
	{"empty", []uint32{}, []string{}, []string{}},
	{"one-item", []uint32{}, []string{"a"}, []string{"a"}},
	{"two-items-swap", []uint32{0}, []string{"a", "b"}, []string{"b", "a"}},
	{"two-items-keep", []uint32{1}, []string{"a", "b"}, []string{"a", "b"}},
	{"five-items", []uint32{7, 3, 4, 1}, []string{"a", "b", "c", "d", "e"}, []string{"a", "e", "b", "d", "c"}},
	{"reject-in-shuffle", []uint32{0, 2, 0}, []string{"a", "b", "c"}, []string{"b", "a", "c"}},
	{"duplicates", []uint32{1, 1}, []string{"x", "x", "y"}, []string{"x", "y", "x"}},
	{"non-ascii", []uint32{4294967295, 4294967294, 0}, []string{"é", "日本", "🙂", "a"}, []string{"日本", "é", "🙂", "a"}},
}

// fakeWords returns a word source that gives words in order, and a check
// that all were used.
func fakeWords(t *testing.T, words []uint32) (WordSource, func() bool) {
	next := 0
	source := func() uint32 {
		if next >= len(words) {
			t.Fatal("no more words")
		}
		next++
		return words[next-1]
	}
	return source, func() bool { return next == len(words) }
}

func TestIntInRange(t *testing.T) {
	for _, c := range rangeCases {
		t.Run(c.name, func(t *testing.T) {
			source, usedAll := fakeWords(t, c.words)
			got := make([]int64, 0, c.count)
			for range c.count {
				got = append(got, IntInRange(source, c.lo, c.hi))
			}
			if !slices.Equal(got, c.want) || !usedAll() {
				t.Errorf("got %v, want %v, used all words: %v", got, c.want, usedAll())
			}
		})
	}
}

func TestShuffle(t *testing.T) {
	for _, c := range shuffleCases {
		t.Run(c.name, func(t *testing.T) {
			source, usedAll := fakeWords(t, c.words)
			got := slices.Clone(c.items)
			Shuffle(source, got)
			if !slices.Equal(got, c.want) || !usedAll() {
				t.Errorf("got %q, want %q, used all words: %v", got, c.want, usedAll())
			}
		})
	}
}

func TestBadRangePanics(t *testing.T) {
	for _, r := range [][2]int64{{1, 0}, {0, 1 << 32}, {math.MinInt64, math.MaxInt64}} {
		func() {
			defer func() {
				if recover() == nil {
					t.Errorf("IntInRange(%d, %d) did not panic", r[0], r[1])
				}
			}()
			IntInRange(func() uint32 { return 0 }, r[0], r[1])
		}()
	}
}

func TestSameSeedSameWords(t *testing.T) {
	a, b, c := SeededWords(42), SeededWords(42), SeededWords(43)
	var wa, wb, wc []uint32
	for range 10 {
		wa, wb, wc = append(wa, a()), append(wb, b()), append(wc, c())
	}
	if !slices.Equal(wa, wb) || slices.Equal(wa, wc) {
		t.Errorf("seed 42: %v and %v, seed 43: %v", wa, wb, wc)
	}
}

func TestSecureWordsInRange(t *testing.T) {
	source := SecureWords()
	for range 100 {
		if v := IntInRange(source, 1, 6); v < 1 || v > 6 {
			t.Fatalf("got %d", v)
		}
	}
}

func TestTokenHex(t *testing.T) {
	for _, n := range []int{0, 1, 16, 32} {
		re := regexp.MustCompile("^[0-9a-f]{" + strconv.Itoa(2*n) + "}$")
		if got := TokenHex(n); !re.MatchString(got) {
			t.Errorf("TokenHex(%d) = %q", n, got)
		}
	}
	if TokenHex(16) == TokenHex(16) {
		t.Error("two tokens are equal")
	}
}
