// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

// Package roundrobin runs jobs in turn with a queue.
package roundrobin

// FinishOrder returns the job numbers in the order in which round-robin
// finishes them. Go has no queue type in the language. A slice is the queue:
// append adds at the back, and queue[1:] removes the front.
func FinishOrder(work []int, slice int) []int {
	left := append([]int(nil), work...)
	queue := make([]int, 0, len(work))
	for i := range work {
		queue = append(queue, i)
	}
	done := []int{}
	for len(queue) > 0 {
		job := queue[0]
		queue = queue[1:]
		left[job] -= min(slice, left[job])
		if left[job] == 0 {
			done = append(done, job)
		} else {
			queue = append(queue, job)
		}
	}
	return done
}
