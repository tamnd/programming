// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

// Package serveorder serves requests by priority with container/heap.
package serveorder

import "container/heap"

// requests is a heap of request numbers. It implements heap.Interface: the
// request for which Less is true comes out first.
type requests struct {
	priority []int32
	items    []int
}

func (r *requests) Len() int { return len(r.items) }

func (r *requests) Less(i, j int) bool {
	a, b := r.items[i], r.items[j]
	if r.priority[a] != r.priority[b] {
		return r.priority[a] > r.priority[b] // the largest priority first
	}
	return a < b // equal priorities: the arrival order
}

func (r *requests) Swap(i, j int) { r.items[i], r.items[j] = r.items[j], r.items[i] }

func (r *requests) Push(x any) { r.items = append(r.items, x.(int)) }

func (r *requests) Pop() any {
	n := len(r.items)
	x := r.items[n-1]
	r.items = r.items[:n-1]
	return x
}

// ServeOrder returns the request numbers in serve order: the largest
// priority first, and for equal priorities, the smaller request number first.
func ServeOrder(priorities []int32) []int {
	r := &requests{priority: priorities}
	for i := range priorities {
		heap.Push(r, i) // call heap.Push, not r.Push
	}
	served := []int{}
	for r.Len() > 0 {
		served = append(served, heap.Pop(r).(int))
	}
	return served
}
