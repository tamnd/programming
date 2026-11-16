// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

// Package idgen has one IdGenerator for the whole program. sync.OnceValue
// makes it on the first call and returns the same pointer after that.
package idgen

import (
	"sync"
	"sync/atomic"
)

// IdGenerator hands out consecutive ids.
type IdGenerator struct {
	next atomic.Int64
}

var constructions atomic.Int64

func newIdGenerator() *IdGenerator {
	constructions.Add(1)
	return &IdGenerator{}
}

// Instance returns the one IdGenerator.
var Instance = sync.OnceValue(newIdGenerator)

// NextID returns the next id, starting at 1. It is safe for concurrent use.
func (g *IdGenerator) NextID() int64 {
	return g.next.Add(1)
}

// Constructions returns how many times the generator has been made.
func Constructions() int64 {
	return constructions.Load()
}

// Report gets the instance calls times and takes one id each time.
func Report(calls int) []int64 {
	seen := map[*IdGenerator]bool{}
	var first, last int64
	for i := 0; i < calls; i++ {
		g := Instance()
		seen[g] = true
		last = g.NextID()
		if i == 0 {
			first = last
		}
	}
	return []int64{last - first, Constructions(), int64(len(seen))}
}
