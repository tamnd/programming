// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

// Package color gives a Color type with a display form and a Go syntax form.
package color

import "fmt"

// Color is a color with three channels of 0 to 255.
type Color struct {
	r, g, b uint8
}

// New makes a Color, or returns an error when a value is out of range.
func New(r, g, b int) (Color, error) {
	for _, v := range []int{r, g, b} {
		if v < 0 || v > 255 {
			return Color{}, fmt.Errorf("not a channel value: %d", v)
		}
	}
	return Color{uint8(r), uint8(g), uint8(b)}, nil
}

// String is the display form. fmt calls it for %v and %s and in Print and Println.
func (c Color) String() string {
	return fmt.Sprintf("#%02X%02X%02X", c.r, c.g, c.b)
}

// GoString is the Go syntax form. fmt calls it for %#v.
func (c Color) GoString() string {
	return fmt.Sprintf("color.New(%d, %d, %d)", c.r, c.g, c.b)
}

// FormatColors returns the display form of each color.
func FormatColors(reds, greens, blues []int) ([]string, error) {
	if len(reds) != len(greens) || len(reds) != len(blues) {
		return nil, fmt.Errorf("the lists must have the same length")
	}
	out := make([]string, 0, len(reds))
	for i := range reds {
		c, err := New(reds[i], greens[i], blues[i])
		if err != nil {
			return nil, err
		}
		out = append(out, fmt.Sprint(c))
	}
	return out, nil
}
