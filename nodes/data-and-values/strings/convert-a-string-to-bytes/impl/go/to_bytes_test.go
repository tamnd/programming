// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

package tobytes

import (
	"bytes"
	"testing"
)

// The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
var cases = []struct {
	name, s string
	want    []byte
}{
	{"ascii", "abc", []byte{97, 98, 99}},
	{"letter", "A", []byte{65}},
	{"punct", "hi!", []byte{104, 105, 33}},
	{"empty", "", []byte{}},
	{"control", "\t\n", []byte{9, 10}},
	{"two-byte", "\u00e9", []byte{195, 169}},
	{"latin-1-max", "\u00ff", []byte{195, 191}},
	{"three-byte", "\u20ac", []byte{226, 130, 172}},
	{"bom", "\ufeff", []byte{239, 187, 191}},
	{"astral", "\U0001f642", []byte{240, 159, 153, 130}},
	{"combining", "e\u0301", []byte{101, 204, 129}},
	{"mixed", "a\u00e9\U0001f642", []byte{97, 195, 169, 240, 159, 153, 130}},
}

func TestToBytes(t *testing.T) {
	for _, c := range cases {
		if got := ToBytes(c.s); !bytes.Equal(got, c.want) {
			t.Errorf("%s: got %v, want %v", c.name, got, c.want)
		}
	}
}
