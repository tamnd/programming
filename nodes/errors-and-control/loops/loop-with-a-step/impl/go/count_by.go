// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

// Package countby counts from a start to a stop by a step.
package countby

// CountBy returns start, start+step, ... while the value has not passed stop.
// step must not be 0.
func CountBy(start, stop, step int32) []int32 {
	out := []int32{}
	// An int64 counter: with an int32, i+step can wrap past the largest int32.
	for i := int64(start); step > 0 && i <= int64(stop) || step < 0 && i >= int64(stop); i += int64(step) {
		out = append(out, int32(i))
	}
	return out
}
