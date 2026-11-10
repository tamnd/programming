```go run id=checker
package main

import (
	"fmt"
	"math"
	"strconv"
)

// Rng is xorshift32 with Marsaglia's seed: the same numbers on every run.
type Rng struct{ state uint32 }

func (r *Rng) Next() uint32 {
	r.state ^= r.state << 13
	r.state ^= r.state >> 17
	r.state ^= r.state << 5
	return r.state
}

func (r *Rng) Between(lo, hi int64) int64 {
	return lo + int64(uint64(r.Next())%uint64(hi-lo+1))
}

// Check runs a property on 100 generated cases and shrinks the first failure.
func Check[T any](name string, generate func(*Rng) T, holds func(T) bool, shrink func(T) []T) {
	rng := &Rng{state: 2463534242}
	for trial := 1; trial <= 100; trial++ {
		c := generate(rng)
		if holds(c) {
			continue
		}
		fmt.Printf("%s: falsified on test %d by %v\n", name, trial, c)
		steps := 0
		for shrunk := true; shrunk; {
			shrunk = false
			for _, smaller := range shrink(c) {
				if !holds(smaller) {
					c, shrunk = smaller, true
					steps++
					break
				}
			}
		}
		fmt.Printf("%s: shrunk in %d steps to %v\n", name, steps, c)
		return
	}
	fmt.Printf("%s: passed 100 tests\n", name)
}

// ShrinkInt gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
func ShrinkInt(x, lo int) []int {
	var out []int
	if x > lo {
		out = append(out, lo)
	}
	for d := (x - lo) / 2; d > 0; d /= 2 {
		out = append(out, x-d)
	}
	return out
}

func pages(items, perPage int) int {
	return items / perPage // bug: drops the last page when it is not full
}

type pair struct{ items, perPage int }

func (p pair) String() string { return fmt.Sprintf("(%d, %d)", p.items, p.perPage) }

func main() {
	Check("round trip",
		func(r *Rng) int32 { return int32(r.Between(math.MinInt32, math.MaxInt32)) },
		func(x int32) bool {
			back, err := strconv.ParseInt(strconv.FormatInt(int64(x), 10), 10, 32)
			return err == nil && int32(back) == x
		},
		func(int32) []int32 { return nil })

	Check("pages",
		func(r *Rng) pair { return pair{int(r.Between(0, 1000)), int(r.Between(1, 50))} },
		func(p pair) bool { return pages(p.items, p.perPage)*p.perPage >= p.items },
		func(p pair) []pair {
			var out []pair
			for _, n := range ShrinkInt(p.items, 0) {
				out = append(out, pair{n, p.perPage})
			}
			for _, n := range ShrinkInt(p.perPage, 1) {
				out = append(out, pair{p.items, n})
			}
			return out
		})
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

The standard library has a property checker, `testing/quick`. A seeded `Config.Rand` makes the run repeatable.

```go run id=quick
package main

import (
	"fmt"
	"math/rand"
	"testing/quick"
)

func pages(items, perPage uint16) int {
	return int(items) / int(perPage) // bug: drops the last page when it is not full
}

func main() {
	holds := func(items uint16, perPage uint8) bool {
		per := uint16(perPage%50) + 1
		return pages(items, per)*int(per) >= int(items)
	}
	config := &quick.Config{MaxCount: 100, Rand: rand.New(rand.NewSource(1))}
	if err := quick.Check(holds, config); err != nil {
		fmt.Println(err)
		ce := err.(*quick.CheckError)
		fmt.Printf("test %d, arguments %v\n", ce.Count, ce.In)
	} else {
		fmt.Println("passed")
	}
}
```

```output for=quick
#1: failed on input 0xfd52, 0x4f
test 1, arguments [64850 79]
```

- Verdict: stdlib. `testing/quick.Check` generates the arguments of a function by reflection and calls it until it returns false. The package is frozen.
- `quick.Check` reports the first failing input as it is, 64850 items on pages of 79. It has no shrinker, so the hand-written `Check` above gives a smaller case, (1, 2).
- `Check[T any]` is a generic function. Go has no tuples, so the pair is a struct with a `String` method, which `%v` calls.
- Go has unsigned integers, so the state is a `uint32` and the shifts and the overflow behave as in C. Unsigned arithmetic in Go wraps; it is not undefined.
- In a test file, the same calls go in a `func TestPages(t *testing.T)`. Go's native fuzzing, `go test -fuzz`, is a different tool with a corpus and coverage feedback; see [[fuzz-testing]].
