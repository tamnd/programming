// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

// Package randomnumbers makes random integers in a range, shuffles a slice,
// and makes seeded word sources and secure tokens.
package randomnumbers

import (
	crand "crypto/rand"
	"encoding/binary"
	"encoding/hex"
	"math/rand/v2"
)

// A WordSource returns a random integer from 0 to 2^32 - 1.
type WordSource func() uint32

const words = 1 << 32

// IntInRange returns an integer from lo to hi, each with equal probability.
// A word below 2^32 % n is rejected, so that each remainder comes from the
// same number of words. It panics if hi < lo or the range has more than
// 2^32 values.
func IntInRange(nextWord WordSource, lo, hi int64) int64 {
	// hi - lo can overflow an int64, but as a uint64 it is the true difference.
	span := uint64(hi) - uint64(lo)
	if hi < lo || span >= words {
		panic("randomnumbers: need lo <= hi and at most 2^32 values")
	}
	n := span + 1
	least := words % n
	for {
		word := uint64(nextWord())
		if word >= least {
			return lo + int64(word%n)
		}
	}
}

// Shuffle shuffles items in place with the Fisher-Yates method.
func Shuffle[T any](nextWord WordSource, items []T) {
	for i := len(items) - 1; i > 0; i-- {
		j := IntInRange(nextWord, 0, int64(i))
		items[i], items[j] = items[j], items[i]
	}
}

// SeededWords returns a word source that gives the same words for the same
// seed.
func SeededWords(seed uint64) WordSource {
	return rand.New(rand.NewPCG(seed, 0)).Uint32
}

// SecureWords returns a word source from crypto/rand.
func SecureWords() WordSource {
	return func() uint32 {
		var b [4]byte
		_, _ = crand.Read(b[:]) // Read never returns an error.
		return binary.LittleEndian.Uint32(b[:])
	}
}

// TokenHex returns nbytes secure random bytes as 2 * nbytes lowercase
// hexadecimal digits.
func TokenHex(nbytes int) string {
	b := make([]byte, nbytes)
	_, _ = crand.Read(b) // Read never returns an error.
	return hex.EncodeToString(b)
}
