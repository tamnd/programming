// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

package utf8codec

import (
	"bytes"
	"testing"
)

// Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
var encodeCases = []struct {
	name string
	cp   rune
	want []byte
}{
	{"ascii-a", 0x41, []byte{65}},
	{"euro", 0x20AC, []byte{226, 130, 172}},
	{"emoji", 0x1F642, []byte{240, 159, 153, 130}},
	{"nul", 0x0, []byte{0}},
	{"one-byte-max", 0x7F, []byte{127}},
	{"two-byte-min", 0x80, []byte{194, 128}},
	{"two-byte", 0xE9, []byte{195, 169}},
	{"two-byte-max", 0x7FF, []byte{223, 191}},
	{"three-byte-min", 0x800, []byte{224, 160, 128}},
	{"three-byte-max", 0xFFFF, []byte{239, 191, 191}},
	{"four-byte-min", 0x10000, []byte{240, 144, 128, 128}},
	{"max", 0x10FFFF, []byte{244, 143, 191, 191}},
}

var decodeCases = []struct {
	name string
	b    []byte
	want rune
}{
	{"ascii-a", []byte{65}, 65},
	{"e-acute", []byte{195, 169}, 233},
	{"emoji", []byte{240, 159, 153, 130}, 128578},
	{"max", []byte{244, 143, 191, 191}, 1114111},
	{"replacement", []byte{239, 191, 189}, 65533},
	{"empty", []byte{}, -1},
	{"stray-continuation", []byte{128}, -1},
	{"truncated", []byte{195}, -1},
	{"bad-continuation", []byte{195, 40}, -1},
	{"overlong-two", []byte{192, 129}, -1},
	{"overlong-three", []byte{224, 128, 128}, -1},
	{"surrogate", []byte{237, 160, 128}, -1},
	{"above-max", []byte{244, 144, 128, 128}, -1},
	{"two-sequences", []byte{65, 66}, -1},
}

func TestEncodeUTF8(t *testing.T) {
	for _, c := range encodeCases {
		if got := EncodeUTF8(c.cp); !bytes.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}

func TestDecodeUTF8(t *testing.T) {
	for _, c := range decodeCases {
		if got := DecodeUTF8(c.b); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}
