---
tag: QR0ZQA
kind: task
slug: use-a-priority-queue
name: Use a priority queue
aliases: [heap queue, max-heap, min-heap, pop the largest, serve by priority, custom priority order]
area: data-and-values/stacks-queues-and-trees
level: core
summary: Serve requests by priority with the standard priority queue of each tier 1 language, with a custom order, a max-heap and ties in arrival order.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/library/heapq.html
    title: heapq, Heap queue algorithm, Python documentation
    locator: the introduction; heappush_max; Priority Queue Implementation Notes
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/util/PriorityQueue.html
    title: PriorityQueue (Java SE 27 and JDK 27)
    locator: the class description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/container/priority_queue
    title: std::priority_queue, cppreference.com
    locator: the introduction; Template parameters
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.priorityqueue-2?view=net-11.0
    title: PriorityQueue<TElement,TPriority> Class, .NET API reference
    locator: Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/container/heap
    title: Package heap, Go standard library
    locator: Overview; type Interface
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/collections/struct.BinaryHeap.html
    title: Struct BinaryHeap, The Rust Standard Library
    locator: the description; Min-heap
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-15.html
    title: The Java Language Specification, Java SE 27 Edition, Chapter 15
    locator: 15.18.2 Additive Operators (+ and -) for Numeric Types
    accessed: 2026-11-16
  - url: https://man7.org/linux/man-pages/man1/sort.1.html
    title: sort(1), Linux manual page
    locator: DESCRIPTION
    accessed: 2026-11-16
---

## Task

Use a priority queue to serve requests by priority. The function `serve_order(priorities)` gets a list: request `i` has the priority `priorities[i]`. It pushes each request onto a priority queue, then pops the requests one at a time. A larger priority comes out first. Requests with equal priorities come out in arrival order, so the smaller request number comes first. The function returns the request numbers in that order.

For example, `serve_order([2, 5, 1, 5])` is `[1, 3, 0, 2]`. Requests 1 and 3 have the priority 5, and request 1 arrived first. Each priority fits in a signed 32-bit integer. For the idea, see [[priority-queue]]. For how a binary heap works, see [binary heap](https://algorithm.tamnd.com/binary-heap/) on the algorithms site.

Three details make this task harder than a call to `push` and `pop`:

- The largest priority must come out first, but some standard types put the smallest first.
- Most heaps do not keep the arrival order of equal priorities.
- A comparison that subtracts two 32-bit priorities can overflow.

## Solutions

| Language | Priority queue | How the largest comes first, with ties in arrival order |
|---|---|---|
| Python | `heapq` on a `list` | push `(-p, i)` |
| JavaScript | a small binary heap class | `before(a, b)` compares `p[a] > p[b]`, then `a < b` |
| TypeScript | a small generic `Heap<T>` class | the same `before` function |
| Java | `PriorityQueue<Integer>` | `comparingInt(priorities::get).reversed().thenComparingInt(i -> i)` |
| C | a binary heap in an array | `before` compares with `>` and `==` |
| C++ | `std::priority_queue` | a lambda that returns true if `a` comes out after `b` |
| C# | `PriorityQueue<int, (int, int)>` | an `IComparer` made with `Comparer.Create` |
| Go | `container/heap` | `Less` on a type that implements `heap.Interface` |
| Rust | `BinaryHeap<(i32, Reverse<usize>)>` | a max-heap; `Reverse(i)` for the ties |
| Bash | no priority queue; `sort` | `sort -k1,1nr -k2,2n` |

- **Python `heapq` is a min-heap.** "heap[0] is the smallest item"[^1]. The solution pushes the tuple `(-p, i)`, so the largest priority is the smallest tuple. The request number `i` is the tie-breaker. The documentation gives the same method: "The entry count serves as a tie-breaker so that two tasks with the same priority are returned in the order they were added."[^1] Python 3.14 and later also have `heappush_max` and `heappop_max` for a max-heap: "Added in version 3.14."[^1] The solution uses `-p`, so it also runs on older versions.
- **Java needs a comparator for a max-heap.** "The head of this queue is the least element with respect to the specified ordering."[^2] The comparator makes the largest priority the least. For equal elements, "ties are broken arbitrarily"[^2], so the comparator also compares the request numbers.
- **C++ has a max-heap with a reversed comparator.** By default, the queue gives "constant time lookup of the largest (by default) element"[^3]. The comparator is a "less than": "Note that the Compare parameter is defined such that it returns true if its first argument comes before its second argument in a weak ordering."[^3] So the lambda returns true if `a` comes out after `b`.
- **C# has a min-heap with no arrival order.** "Elements with the lowest priority are dequeued first."[^4] Also, "Note that the type does not guarantee first-in-first-out semantics for elements of equal priority."[^4] The solution uses the pair `(priority, index)` as the priority and a comparer that puts the largest priority first.
- **Go gives heap functions, not a heap type.** "Package heap provides heap operations for any type that implements heap.Interface."[^5] The type gives `Len`, `Less`, `Swap`, `Push` and `Pop`, and `Less` decides the order.
- **Rust `BinaryHeap` is a max-heap.** "A priority queue implemented with a binary heap. This will be a max-heap."[^6] Tuples compare item by item. `(p, Reverse(i))` puts the largest priority first, and `Reverse` makes the smaller request number the larger value. For a min-heap, "Either core::cmp::Reverse or a custom Ord implementation can be used to make BinaryHeap a min-heap."[^6]
- **JavaScript, TypeScript and C have no priority queue.** The solutions contain a small binary heap with a `before` function. `push` moves the new item up, and `pop` moves the last item from the root down.
- **Bash uses `sort`.** In this task, all of the requests come before the first one is served, so a sort gives the same order as a priority queue. `-k1,1nr` sorts the priorities as numbers, largest first, and `-k2,2n` sorts equal priorities by request number. A program that adds requests while it serves others needs a real priority queue.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no requests give an empty list.
- `one`: one request gives `[0]`.
- `ties`: four equal priorities come out in the order 0, 1, 2, 3. A heap with no tie-breaker can give another order.
- `negative`: all priorities are below 0, and `-1` is the largest.
- `int32-extremes`: the smallest and the largest 32-bit integers are in the list. A comparator that computes `b - a` overflows here. Negation also fails: in Java, "negation of the maximum negative int or long results in that same maximum negative number."[^7] The Python solution negates with no overflow.
- `sorted` and `reversed`: the order of the input does not change the rule.

## Pitfalls

- **Python `heapq` pops the smallest item.** If you push the priorities, the smallest priority comes out first. Push `-p` for the largest first:

```python run id=python-min-heap
import heapq

heap = []
for p in [2, 5, 1]:
    heapq.heappush(heap, p)
print("pushed p, popped", heapq.heappop(heap))

heap = []
for p in [2, 5, 1]:
    heapq.heappush(heap, -p)
print("pushed -p, popped", -heapq.heappop(heap))
```

```output for=python-min-heap
pushed p, popped 1
pushed -p, popped 5
```

- **Java `(a, b) -> b - a` overflows.** "If an integer addition overflows, then the result is the low-order bits of the mathematical sum as represented in some sufficiently large two's-complement format."[^7] And "a-b produces the same result as a+(-b)"[^7]. So for large priorities, `b - a` gives the wrong sign, and the queue gives the wrong head. Use `Integer.compare(b, a)`:

```java run id=java-subtract-comparator
import java.util.PriorityQueue;

public class Main {
  public static void main(String[] args) {
    PriorityQueue<Integer> subtract = new PriorityQueue<>((a, b) -> b - a);
    PriorityQueue<Integer> compare = new PriorityQueue<>((a, b) -> Integer.compare(b, a));
    for (int p : new int[] {Integer.MIN_VALUE, 1, Integer.MAX_VALUE}) {
      subtract.offer(p);
      compare.offer(p);
    }
    System.out.println("b - a: " + subtract.poll());
    System.out.println("Integer.compare(b, a): " + compare.poll());
  }
}
```

```output for=java-subtract-comparator
b - a: -2147483648
Integer.compare(b, a): 2147483647
```

- **C++ `std::greater` gives the smallest on top.** The documentation says that "using std::greater<T> would cause the smallest element to appear as the top()"[^3]. This is the opposite of what the name seems to say:

```cpp run id=cpp-greater-is-min
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::priority_queue<int> by_less;
    std::priority_queue<int, std::vector<int>, std::greater<int>> by_greater;
    for (int p : {2, 5, 1}) {
        by_less.push(p);
        by_greater.push(p);
    }
    std::cout << "std::less (the default), top: " << by_less.top() << '\n';
    std::cout << "std::greater, top: " << by_greater.top() << '\n';
}
```

```output for=cpp-greater-is-min
std::less (the default), top: 5
std::greater, top: 1
```

- **C# equal priorities do not come out in arrival order.** The type "does not guarantee first-in-first-out semantics for elements of equal priority."[^4] Here, five items with the same priority come out in another order. Add the arrival number to the priority, as the solution does:

```csharp run id=csharp-equal-priorities
using System;
using System.Collections.Generic;

var queue = new PriorityQueue<string, int>();
foreach (string name in new[] { "A", "B", "C", "D", "E" })
{
    queue.Enqueue(name, 0);
}
var order = new List<string>();
while (queue.TryDequeue(out var item, out _))
{
    order.Add(item);
}
Console.WriteLine(string.Join(" ", order));
```

```output for=csharp-equal-priorities
A E D C B
```

- **Go: call `heap.Push`, not the `Push` method.** "Note that Push and Pop in this interface are for package heap's implementation to call. To add and remove things from the heap, use heap.Push and heap.Pop."[^5] The method only appends, so the heap is then not in order:

```go run id=go-method-push
package main

import (
	"container/heap"
	"fmt"
)

type ints []int

func (h ints) Len() int           { return len(h) }
func (h ints) Less(i, j int) bool { return h[i] < h[j] }
func (h ints) Swap(i, j int)      { h[i], h[j] = h[j], h[i] }
func (h *ints) Push(x any)        { *h = append(*h, x.(int)) }
func (h *ints) Pop() any {
	old := *h
	x := old[len(old)-1]
	*h = old[:len(old)-1]
	return x
}

func main() {
	a := &ints{5, 2, 8}
	heap.Init(a)
	a.Push(1)
	fmt.Println("after a.Push(1), smallest out:", heap.Pop(a))

	b := &ints{5, 2, 8}
	heap.Init(b)
	heap.Push(b, 1)
	fmt.Println("after heap.Push(b, 1), smallest out:", heap.Pop(b))
}
```

```output for=go-method-push
after a.Push(1), smallest out: 2
after heap.Push(b, 1), smallest out: 1
```

- **Bash `sort` without `-n` compares text.** `-n` is "compare according to string numerical value"[^8]. Without it, `9` comes after `10`, because the character `9` comes after the character `1`:

```bash run id=bash-sort-numbers
printf '%s\n' 9 10 -2 | sort -r | head -n 1
printf '%s\n' 9 10 -2 | sort -nr | head -n 1
```

```output for=bash-sort-numbers
9
10
```

## Related tasks

- [[use-a-queue]]: the first item in is the first item out.
- [[use-a-stack]]: the last item in is the first item out.
- [[priority-queue]]: the idea behind this task and the operations.
- [[sort-with-a-custom-key]]: the same order rules for a sort.
- [[stable-sort]]: keep the input order of equal items.

## References
