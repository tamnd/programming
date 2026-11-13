// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

// Package graphemes counts the extended grapheme clusters of UAX #29 for the
// code points of the contract.
package graphemes

const (
	other = iota
	cr
	lf
	control
	extend
	zwj
	ri
	pict
)

// kind returns the Grapheme_Cluster_Break kind of c, or pict for
// Extended_Pictographic.
func kind(c rune) int {
	switch {
	case c == 0x0D:
		return cr
	case c == 0x0A:
		return lf
	case c < 0x20 || (c >= 0x7F && c <= 0x9F) || c == 0xAD:
		return control
	case (c >= 0x300 && c <= 0x36F) || (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0x1F3FB && c <= 0x1F3FF):
		return extend
	case c == 0x200D:
		return zwj
	case c >= 0x1F1E6 && c <= 0x1F1FF:
		return ri
	case c == 0xA9 || c == 0xAE || c == 0x2764 || (c >= 0x1F400 && c <= 0x1F4FD) || (c >= 0x1F600 && c <= 0x1F64F):
		return pict
	}
	return other
}

// CountGraphemes returns the number of graphemes in s.
// utf8.RuneCountInString counts code points.
func CountGraphemes(s string) int {
	n, prev, riRun := 0, -1, 0
	isPict, zwjAfterPict := false, false
	for _, c := range s {
		k := kind(c)
		join := false
		switch {
		case prev < 0:
		case prev == cr && k == lf: // GB3
			join = true
		case prev == cr || prev == lf || prev == control || k == cr || k == lf || k == control: // GB4, GB5
		case k == extend || k == zwj: // GB9
			join = true
		case k == pict && zwjAfterPict: // GB11
			join = true
		case k == ri && riRun%2 == 1: // GB12, GB13
			join = true
		}
		if !join {
			n++
		}
		zwjAfterPict = k == zwj && isPict
		isPict = k == pict || (k == extend && isPict)
		if k == ri {
			riRun++
		} else {
			riRun = 0
		}
		prev = k
	}
	return n
}
