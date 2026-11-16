// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

// Package composition has a generic compose helper.
package composition

// Compose returns the composition of the functions. The last one runs first.
func Compose[T any](fs ...func(T) T) func(T) T {
	return func(x T) T {
		for i := len(fs) - 1; i >= 0; i-- {
			x = fs[i](x)
		}
		return x
	}
}

var ops = map[string]func(int) int{
	"inc":    func(x int) int { return x + 1 },
	"dec":    func(x int) int { return x - 1 },
	"double": func(x int) int { return 2 * x },
	"negate": func(x int) int { return -x },
}

// ComposeAll composes the functions that names names and applies the result to x.
func ComposeAll(names []string, x int) int {
	fs := make([]func(int) int, len(names))
	for i, name := range names {
		fs[i] = ops[name]
	}
	return Compose(fs...)(x)
}
