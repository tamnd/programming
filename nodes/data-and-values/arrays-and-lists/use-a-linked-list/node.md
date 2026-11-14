---
tag: 87XMA6
kind: task
slug: use-a-linked-list
name: Use a linked list
aliases: [linked list operations, doubly linked list, build a linked list, LinkedList, std::list, container/list, push front and push back, remove a node]
area: data-and-values/arrays-and-lists
level: core
summary: Push and pop at both ends of a linked list and remove a node in the middle, with the standard list type or with nodes that you build, and see why a Rust list is hard to write.
contract: contract.yaml
category: collections
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://en.wikipedia.org/wiki/Linked_list
  title: Linked list, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/library/collections.html#collections.deque
  title: The Python Standard Library, collections
  locator: deque objects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/LinkedList.html
  title: Java SE 25 API, Class LinkedList
  locator: Class description; remove(int index); remove(Object o)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.linkedlist-1?view=net-10.0
  title: .NET API reference, LinkedList<T> Class
  locator: Definition; Remarks
- url: https://pkg.go.dev/container/list
  title: Go standard library, package list
  locator: Overview; type Element; func (*Element) Next
- url: https://doc.rust-lang.org/std/collections/struct.LinkedList.html
  title: Rust standard library, Struct LinkedList
  locator: Description; split_off
- url: https://rust-unofficial.github.io/too-many-lists/
  title: Learning Rust With Entirely Too Many Linked Lists
  locator: Introduction
- url: https://rust-unofficial.github.io/too-many-lists/fourth.html
  title: Learning Rust With Entirely Too Many Linked Lists
  locator: A Bad Safe Deque
- url: https://en.cppreference.com/w/cpp/container/list
  title: cppreference.com, std::list
  locator: Description
- url: https://tiswww.case.edu/php/chet/bash/bashref.html#Arrays
  title: Bash Reference Manual
  locator: 6.7 Arrays
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.linkedlist-1.addlast?view=net-10.0
  title: .NET API reference, LinkedList<T>.AddLast Method
  locator: AddLast(LinkedListNode<T>), Exceptions
- url: https://doc.rust-lang.org/std/rc/index.html
  title: Rust standard library, Module rc
  locator: Module description
- url: https://en.cppreference.com/w/c/language/operator_other
  title: cppreference.com, C other operators
  locator: Function call
- url: https://en.cppreference.com/w/cpp/container/list/remove
  title: cppreference.com, std::list::remove, remove_if
  locator: Description
---

## Task

Run a list of operations on a new, empty linked list, and return the items of the list from front to back. The function `run_ops(ops)` takes 0 to 1000 operations. Each operation is one of these strings:

- `push_front N` puts `N` before the first item.
- `push_back N` puts `N` after the last item.
- `pop_front` and `pop_back` remove the first or the last item. The input does not pop an empty list.
- `remove N` removes the first item that is equal to `N`. It does nothing if no item is equal to `N`.

For example, `run_ops(["push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"])` is `[0, 1, 3]`. Each `N` is a 32-bit signed integer.

In computer science, "a linked list is a linear collection of data elements whose order is not given by their physical placement in memory"[^1]. Each node keeps a link to the next node, and in a doubly linked list also a link to the node before it. So a push or a pop at an end, and the removal of a node that the code already has, take constant time. But the code finds a node only by following the links. The concept page [linked list](/linked-list/) compares the two designs, and a [dynamic array](/dynamic-array/) is the better default in most programs.

## Solutions

| Language | Linked list | Remove the first match |
|---|---|---|
| Python | a `Node` class that you build; `collections.deque` for the ends | a loop over the nodes, then unlink |
| JavaScript, TypeScript | a `Node` class that you build | a loop over the nodes, then unlink |
| Java | `java.util.LinkedList<Integer>` | `list.remove(Integer.valueOf(v))` |
| C# | `LinkedList<int>` | `list.Remove(v)` |
| Go | `container/list` | a loop, then `l.Remove(e)` |
| Rust | `std::collections::LinkedList<i32>` | `split_off`, `pop_front` and `append` |
| C | a `struct node` with `malloc` and `free` | a loop, then unlink and `free` |
| C++ | `std::list<int>` | `std::ranges::find`, then `erase` |
| Bash | three arrays of node indexes | a loop, then unlink and `unset` |

- **Python.** Python has no linked list type. The solution builds a doubly linked list with a `Node` class that has `value`, `prev` and `next`. The list keeps a `head` and a `tail`, and one `unlink` method removes any node and updates the ends. For a queue, use `collections.deque`: "Deques support thread-safe, memory efficient appends and pops from either side of the deque with approximately the same O(1) performance in either direction"[^2]. But for a deque, "Indexed access is O(1) at both ends but slows to O(n) in the middle"[^2].
- **JavaScript and TypeScript.** The solution builds the same `Node` and `LinkedList` classes. The list class has an iterator, so `[...list]` gives the items as an array.
- **Java.** `java.util.LinkedList` is a "Doubly-linked list implementation of the List and Deque interfaces"[^3]. `addFirst`, `addLast`, `removeFirst` and `removeLast` work at the ends. It is also a `List`, but "Operations that index into the list will traverse the list from the beginning or the end, whichever is closer to the specified index"[^3]. So a loop that calls `get(i)` for each `i` is slow.
- **C#.** `LinkedList<T>` "Represents a doubly linked list"[^4]. It gives its nodes as `LinkedListNode<T>` objects, "so insertion and removal are O(1) operations"[^4] when the code has the node. `Remove(T)` searches for the first node with the value.
- **Go.** "Package list implements a doubly linked list"[^5]. `PushFront`, `PushBack`, `Front`, `Back` and `Remove` work on `*list.Element` values. The `Value` field of an element has the type `any`, so the code needs a type assertion such as `e.Value.(int)` to get an `int` back.
- **Rust.** `LinkedList` is "A doubly-linked list with owned nodes"[^6]. Its own documentation says: "It is almost always better to use Vec or VecDeque because array-based containers are generally faster, more memory efficient, and make better use of CPU cache"[^6]. It has no stable method that removes a node in the middle, so the solution uses `split_off`, which "Splits the list into two at the given index"[^6], then `pop_front` on the second part and `append`. A list that you write yourself is harder in Rust than in the other languages, because each node of a doubly linked list has two owners: the node before it and the node after it. The book about this problem says: "Yes, linked lists are so truly awful that you deal with all of these concepts in making them real"[^7]. Its safe deque chapter also says: "Doubly-linked lists are hard, and I always make a mistake somewhere"[^8].
- **C.** The solution builds a `struct node` with `prev` and `next` pointers, and gets each node from `malloc`. `unlink_node` updates the head and the tail, then frees the node. At the end, `free_list` reads `next` before it frees each node.
- **C++.** "std::list is a container that supports constant time insertion and removal of elements from anywhere in the container. Fast random access is not supported"[^9]. The solution finds the first match with `std::ranges::find` and erases it with the iterator.
- **Bash.** Bash has no pointers or objects, so the solution keeps three indexed arrays: `val`, `prv` and `nxt`. A node is an index into the arrays, and the index -1 means that there is no node. This works because Bash arrays have no "requirement that members be indexed or assigned contiguously"[^10], so the indexes of removed nodes can stay empty.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty` and `push-pop`: no operations, and a push followed by a pop. The result is an empty list, and the head and the tail must both be empty.
- `single` and `remove-only`: a list of one node, where the head and the tail are the same node. After `remove-only` removes the only node, the next push must set both ends again.
- `remove-head` and `remove-tail`: the removed node is at an end, so the code must move the head or the tail. A list that keeps the old tail adds the next item to a removed node.
- `pop-both-ends`: a pop from the front and from the back of a list of two nodes.
- `remove-absent`: the value is not in the list, and the list does not change.
- `duplicates` and `push-front-order`: `remove` takes out only the first match, and `push_front` puts each new item before the others, so the order is reversed.
- `negative` and `limit`: the values are negative or at the 32-bit limits, and the parse of each operation must keep them.

## Pitfalls

- **Python a bounded deque drops items without an error.** "Once a bounded length deque is full, when new items are added, a corresponding number of items are discarded from the opposite end"[^2]. This is correct for a buffer of the last items, but it loses data when a queue gets a `maxlen` by mistake:

```python run id=python-deque-maxlen
from collections import deque

d = deque([1, 2, 3], maxlen=3)
d.append(4)
print(list(d))
d.appendleft(0)
print(list(d))
```

```output for=python-deque-maxlen
[2, 3, 4]
[0, 2, 3]
```

- **JavaScript a removal at the end must also move the tail.** The function below removes the node, but it does not change `tail` when the node is the last one. The next `pushBack` then links the new node to the removed node, and the new item is lost:

```javascript run id=javascript-stale-tail
const list = { head: null, tail: null };
function pushBack(value) {
  const node = { value, next: null };
  if (list.tail === null) {
    list.head = node;
  } else {
    list.tail.next = node;
  }
  list.tail = node;
}
function removeBad(value) {
  let prev = null;
  for (let node = list.head; node !== null; prev = node, node = node.next) {
    if (node.value === value) {
      if (prev === null) {
        list.head = node.next;
      } else {
        prev.next = node.next;
      }
      return; // The fix: if (list.tail === node) list.tail = prev;
    }
  }
}
pushBack(1);
pushBack(2);
removeBad(2);
pushBack(3);
const items = [];
for (let node = list.head; node !== null; node = node.next) {
  items.push(node.value);
}
console.log(items);
```

```output for=javascript-stale-tail
[ 1 ]
```

- **Java `remove(1)` removes an index, not a value.** `LinkedList` has two methods: `remove(int index)` "Removes the element at the specified position in this list", and `remove(Object o)` "Removes the first occurrence of the specified element from this list, if it is present"[^3]. With an `int` argument, Java calls the first one:

```java run id=java-remove-index-or-value
import java.util.LinkedList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    LinkedList<Integer> a = new LinkedList<>(List.of(1, 5, 7));
    a.remove(1);
    System.out.println(a);
    LinkedList<Integer> b = new LinkedList<>(List.of(1, 5, 7));
    b.remove(Integer.valueOf(1));
    System.out.println(b);
  }
}
```

```output for=java-remove-index-or-value
[1, 7]
[5, 7]
```

- **C# a node belongs to one list.** `AddLast(LinkedListNode<T>)` throws `InvalidOperationException` when the "node belongs to another LinkedList<T>"[^11]. Remove the node from the first list before you add it to the second:

```csharp run id=csharp-node-other-list
var a = new LinkedList<int>(new[] { 1, 2 });
var b = new LinkedList<int>();
var node = a.First!;
try
{
    b.AddLast(node);
}
catch (InvalidOperationException e)
{
    Console.WriteLine(e.GetType().Name);
}
a.Remove(node);
b.AddLast(node);
Console.WriteLine(string.Join(",", a) + " " + string.Join(",", b));
```

```output for=csharp-node-other-list
InvalidOperationException
2 1
```

- **Go `Remove` in a loop stops the loop.** `Next` "returns the next list element or nil"[^5], and after `l.Remove(e)` the element has no links, so `e.Next()` is nil and the loop ends early. Read the next element before you remove:

```go run id=go-remove-in-loop
package main

import (
	"container/list"
	"fmt"
)

func values(l *list.List) []int {
	var out []int
	for e := l.Front(); e != nil; e = e.Next() {
		out = append(out, e.Value.(int))
	}
	return out
}

func main() {
	l := list.New()
	for _, v := range []int{1, 2, 2, 3} {
		l.PushBack(v)
	}
	for e := l.Front(); e != nil; e = e.Next() {
		if e.Value.(int) == 2 {
			l.Remove(e)
		}
	}
	fmt.Println(values(l))
	for e := l.Front(); e != nil; {
		next := e.Next()
		if e.Value.(int) == 2 {
			l.Remove(e)
		}
		e = next
	}
	fmt.Println(values(l))
}
```

```output for=go-remove-in-loop
[1 2 3]
[1 3]
```

- **Rust two `Rc` links that point at each other are never freed.** A safe doubly linked list often uses `Rc<RefCell<Node>>`, so that two nodes can own a node. But "A cycle between Rc pointers will never be deallocated. For this reason, Weak is used to break cycles"[^12]. Below, the first two nodes link to each other with `Rc` in both directions, and their `drop` never runs. The second two use a `Weak` link back, and both are freed:

```rust run id=rust-rc-cycle
use std::cell::RefCell;
use std::rc::{Rc, Weak};

struct Strong {
    name: &'static str,
    other: RefCell<Option<Rc<Strong>>>,
}

impl Drop for Strong {
    fn drop(&mut self) {
        println!("drop {}", self.name);
    }
}

struct WithWeak {
    name: &'static str,
    next: RefCell<Option<Rc<WithWeak>>>,
    prev: RefCell<Weak<WithWeak>>,
}

impl Drop for WithWeak {
    fn drop(&mut self) {
        println!("drop {}", self.name);
    }
}

fn main() {
    {
        let a = Rc::new(Strong { name: "a", other: RefCell::new(None) });
        let b = Rc::new(Strong { name: "b", other: RefCell::new(Some(Rc::clone(&a))) });
        *a.other.borrow_mut() = Some(Rc::clone(&b));
        println!("strong counts: {} {}", Rc::strong_count(&a), Rc::strong_count(&b));
    }
    println!("end of the first scope");
    {
        let c = Rc::new(WithWeak { name: "c", next: RefCell::new(None), prev: RefCell::new(Weak::new()) });
        let d = Rc::new(WithWeak { name: "d", next: RefCell::new(None), prev: RefCell::new(Rc::downgrade(&c)) });
        *c.next.borrow_mut() = Some(Rc::clone(&d));
        println!("d links back: {}", d.prev.borrow().upgrade().is_some());
    }
    println!("end of the second scope");
}
```

```output for=rust-rc-cycle
strong counts: 2 2
end of the first scope
d links back: true
drop c
drop d
end of the second scope
```

- **C a function that gets the head by value cannot change it.** For a function call, "the function can modify its parameters, and those changes do not affect the arguments; C function calls are only call-by-value"[^13]. So `push_bad` below changes its own copy of `head`, and the list of the caller stays empty. Pass a pointer to the head:

```c run id=c-head-by-value
#include <stddef.h>
#include <stdio.h>

struct node {
    int value;
    struct node *next;
};

static void push_bad(struct node *head, struct node *n) {
    n->next = head;
    head = n;
}

static void push(struct node **head, struct node *n) {
    n->next = *head;
    *head = n;
}

int main(void) {
    struct node a = {1, NULL};
    struct node b = {2, NULL};
    struct node *head = NULL;
    push_bad(head, &a);
    printf("%s\n", head == NULL ? "empty" : "not empty");
    push(&head, &a);
    push(&head, &b);
    for (const struct node *n = head; n != NULL; n = n->next) {
        printf("%d\n", n->value);
    }
    return 0;
}
```

```output for=c-head-by-value
empty
2
1
```

- **C++ `list::remove` removes every match.** `remove` "Removes all elements that are equal to value (using operator==)"[^14]. To remove only the first match, find it and erase it:

```cpp run id=cpp-list-remove-all
#include <algorithm>
#include <iostream>
#include <list>

static void print(const std::list<int> &l) {
    const char *sep = "";
    for (int x : l) {
        std::cout << sep << x;
        sep = " ";
    }
    std::cout << "\n";
}

int main() {
    std::list<int> a{4, 5, 4};
    a.remove(4);
    print(a);
    std::list<int> b{4, 5, 4};
    b.erase(std::ranges::find(b, 4));
    print(b);
}
```

```output for=cpp-list-remove-all
5
5 4
```

- **Bash `unset` leaves a gap in an array.** "unset name[subscript] unsets the array element at index subscript"[^10], and the indexes after it do not move. So a loop from 0 to the count of items misses the last item. The Bash solution follows the `nxt` links, and does not count:

```bash run id=bash-unset-gap
a=(10 20 30)
unset 'a[1]'
echo "${#a[@]} ${!a[*]}"
for ((i = 0; i < ${#a[@]}; i++)); do
  echo "item ${a[i]:-none}"
done
```

```output for=bash-unset-gap
2 0 2
item 10
item none
```

## Related tasks

- [[append-to-a-list]]: add an item at the end of a dynamic array.
- [[reverse-a-list]]: put the items in the opposite order.
- [[find-the-index-of-a-value]]: find the position of a value in an array.
- [[concatenate-two-lists]]: join two lists, as `append` does for two linked lists in Rust.

## References
