// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

// Package uselinkedlist runs operations on the linked list of container/list.
package uselinkedlist

import (
	"container/list"
	"strconv"
	"strings"
)

// RunOps runs the operations on a new linked list and returns its items.
func RunOps(ops []string) []int {
	l := list.New()
	for _, op := range ops {
		name, arg, _ := strings.Cut(op, " ")
		v := 0
		if arg != "" {
			n, err := strconv.Atoi(arg)
			if err != nil {
				panic("bad operation: " + op)
			}
			v = n
		}
		switch name {
		case "push_front":
			l.PushFront(v)
		case "push_back":
			l.PushBack(v)
		case "pop_front":
			l.Remove(l.Front())
		case "pop_back":
			l.Remove(l.Back())
		case "remove":
			for e := l.Front(); e != nil; e = e.Next() {
				// Value has the type any, so it needs a type assertion.
				if e.Value.(int) == v {
					l.Remove(e)
					break
				}
			}
		default:
			panic("bad operation: " + op)
		}
	}
	out := make([]int, 0, l.Len())
	for e := l.Front(); e != nil; e = e.Next() {
		out = append(out, e.Value.(int))
	}
	return out
}
