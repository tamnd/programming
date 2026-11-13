---
tag: P9NS07
kind: concept
slug: dynamic-array
name: Dynamic array
aliases: [growable array, resizable array, array list, ArrayList, vector, std::vector, Vec, List<T>, dynamic table]
area: data-and-values/arrays-and-lists
level: core
summary: >-
  A dynamic array keeps its items in an ordinary array with spare slots at the end. When the slots are full, it makes a bigger array and copies the items, so an append takes constant time on average.
applies_to: core-languages
viz: optional
formal: none
see_also: [array, memory-address, stack-and-heap, in-place-and-copying-operations, explicit-memory-management, use-after-free, iterators, ownership]
sources:
- url: https://en.wikipedia.org/wiki/Dynamic_array
  title: Dynamic array, Wikipedia
  locator: Introduction; Bounded-size dynamic arrays and capacity; Geometric expansion and amortized cost; Growth factor; Performance; Language support
- url: https://en.wikipedia.org/wiki/Amortized_analysis
  title: Amortized analysis, Wikipedia
  locator: History; Dynamic array
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html
  title: Class ArrayList, Java SE 25 API
  locator: Class description
- url: https://doc.rust-lang.org/std/vec/struct.Vec.html
  title: Struct Vec, The Rust Standard Library
  locator: Introduction; Capacity and reallocation; Guarantees
- url: https://github.com/python/cpython/blob/main/Objects/listobject.c
  title: Objects/listobject.c, CPython source code
  locator: list_resize
- url: https://github.com/golang/go/blob/master/src/runtime/slice.go
  title: src/runtime/slice.go, Go source code
  locator: nextslicecap
- url: https://en.cppreference.com/w/cpp/container/vector.html
  title: std::vector, cppreference.com
  locator: Introduction
- url: https://en.cppreference.com/w/cpp/container/vector/reserve.html
  title: std::vector::reserve, cppreference.com
  locator: Description; Notes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.capacity?view=net-10.0
  title: List<T>.Capacity Property, .NET API reference
  locator: Remarks
- url: https://docs.python.org/3/faq/design.html
  title: Design and History FAQ, Python documentation
  locator: How are lists implemented in CPython?
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1?view=net-10.0
  title: List<T> Class, .NET API reference
  locator: Remarks
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Appending to and copying slices
- url: https://go.dev/blog/slices-intro
  title: Go Slices, usage and internals, The Go Blog
  locator: Slice internals; Growing slices (the copy and append functions)
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/length
  title: Array length, MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/ArrayBuffer
  title: ArrayBuffer, MDN Web Docs
  locator: Resizing ArrayBuffers
- url: https://en.cppreference.com/w/cpp/container/vector/push_back.html
  title: std::vector::push_back, cppreference.com
  locator: Description
---

## Idea

An [array](/array/) has a fixed length, but most programs do not know at the start how many items they will keep. A dynamic array solves this problem with an ordinary array that is larger than it must be. Wikipedia describes it: "A simple dynamic array can be constructed by allocating an array of fixed-size, typically larger than the number of elements immediately required. The elements of the dynamic array are stored contiguously at the start of the underlying array, and the remaining positions towards the end of the underlying array are reserved"[^1].

A dynamic array has two sizes:

- the **length** (or size, or count), the number of items in the list;
- the **capacity**, the number of slots in the array inside it.

An append that finds a free slot writes the item and adds 1 to the length. An append that finds no free slot must first grow the array: it makes a new, bigger array, copies every item into it and frees the old array. The trick is the size of the new array. If each new array is a constant factor bigger than the old one, such as twice as big, the copies are rare, and the average cost of an append is a constant. The task [append to a list](/append-to-a-list/) shows the append call in each language.

```python run id=idea
class DynamicArray:
    """A list that keeps its items in a fixed array and grows it when it is full."""

    def __init__(self, factor=2.0):
        self.factor = factor
        self.capacity = 1
        self.length = 0
        self.slots = [None] * self.capacity
        self.copies = 0

    def append(self, item):
        if self.length == self.capacity:
            # Full: make a bigger array and copy every item into it.
            new_capacity = max(self.capacity + 1, int(self.capacity * self.factor))
            new_slots = [None] * new_capacity
            for i in range(self.length):
                new_slots[i] = self.slots[i]
            self.copies += self.length
            self.slots, self.capacity = new_slots, new_capacity
        self.slots[self.length] = item
        self.length += 1

    def __getitem__(self, i):
        if not 0 <= i < self.length:
            raise IndexError(i)
        return self.slots[i]


a = DynamicArray()
seen = []
for x in range(20):
    a.append(x * 10)
    if a.capacity not in seen:
        seen.append(a.capacity)
print(a.length, a.capacity, a[19], seen)


def copies_for(n, grow):
    """Count the item copies that n appends make with a growth rule."""
    capacity, copies = 1, 0
    for length in range(n):
        if length == capacity:
            copies += length
            capacity = grow(capacity)
    return copies


n = 100_000
for name, grow in [("double", lambda c: 2 * c),
                   ("times 1.5", lambda c: max(c + 1, c * 3 // 2)),
                   ("plus 100", lambda c: c + 100)]:
    c = copies_for(n, grow)
    print(f"{name}: {c} copies, {c / n:.2f} per append")
```

```output for=idea
20 32 190 [1, 2, 4, 8, 16, 32]
double: 131071 copies, 1.31 per append
times 1.5: 276521 copies, 2.77 per append
plus 100: 49951000 copies, 499.51 per append
```

Twenty appends grow the array only five times, through the capacities 2, 4, 8, 16 and 32. For 100,000 appends, doubling copies about 1.3 items for each append, and a growth factor of 1.5 copies about 2.8. A rule that adds 100 slots each time copies about 500 items for each append, and the number grows with `n`: the total work is quadratic.

## How it works

**Growth.** Wikipedia gives the rule: "To avoid incurring the cost of resizing many times, dynamic arrays resize by a large amount, such as doubling in size, and use the reserved space for future expansion"[^1]. With a growth factor `a`, the capacities form a geometric series. Wikipedia continues: "Expanding the array by any constant proportion a ensures that inserting n elements takes O(n) time overall, meaning that each insertion takes amortized constant time"[^1].

The reason is a sum. With doubling, the last growth copies at most `n` items, the one before it at most `n / 2`, the one before that `n / 4`, and so on. The sum `n + n/2 + n/4 + ...` is less than `2n`. The page on amortized analysis says it in the same way: "the times for steps that scale the array add in a geometric series to O(n), while the constant times for each remaining push also add to O(n). Therefore the average time per push operation is O(n)/n=O(1)"[^2]. A rule that adds a constant number of slots does not give this result. With a step of `c` slots, the copies are `c + 2c + 3c + ...`, which is about `n * n / (2c)`.

**Amortized time.** Amortized constant time is a promise about a sequence of operations, not about each one. Most appends write one slot, and some appends copy the whole list. Java states the promise in these words: "The add, and addLast operations runs in amortized constant time, that is, adding n elements requires O(n) time"[^3]. Rust says that, whatever growth rule `Vec` uses, it "will of course guarantee O(1) amortized push"[^4].

**The growth factor.** The factor is a trade between time and memory. Wikipedia says: "For growth factor a, the average time per insertion operation is about a/(a−1)"[^1], and a bigger factor leaves more slots empty. It also notes that a factor of 2 can work badly with some memory allocators, and that "There have been various discussions on ideal growth factor values, including proposals for the golden ratio as well as the value 1.5"[^1]. The libraries use different rules:

- **CPython** grows a list by about one eighth, plus a small constant. The source code computes `newsize + (newsize >> 3) + 6`, rounded to a multiple of 4, and the comment gives "The growth pattern is:  0, 4, 8, 16, 24, 32, 40, 52, 64, 76, ..."[^5].
- **Java** does not say. "The details of the growth policy are not specified beyond the fact that adding an element has constant amortized time cost"[^3].
- **Rust** also does not say. "Vec does not guarantee any particular growth strategy when reallocating when full, nor when reserve is called"[^4]. The Rust example shows the capacities 4, 8, 16, 32, 64 and 128 for `i32` items.
- **Go** doubles small slices and grows large slices more slowly. The runtime code has a comment about a "Transition from growing 2x for small slices" to "growing 1.25x for large slices"[^6], and it starts the change at a capacity of 256. The Go example shows the capacities 512, 848, 1280 and 1792 after this point. The runtime also rounds each size up to a size that its allocator uses.
- **.NET** `List<T>` and **libstdc++** `std::vector` double in the example files: C# shows 4, 8, 16, 32, 64 and 128, and C++ shows 1, 2, 4, 8 and so on.

**Operations.** A dynamic array keeps the speed of an array for reads and writes. Wikipedia lists the costs: "Getting or setting the value at a particular index (constant time)", "Iterating over the elements in order (linear time, good cache performance)", "Inserting or deleting an element in the middle of the array (linear time)" and "Inserting or deleting an element at the end of the array (constant amortized time)"[^1]. An insert at position `i` moves every item after `i` one slot to the right, so an insert at the front moves all the items. cppreference gives the same costs for `std::vector`: "Insertion or removal of elements at the end - amortized constant 𝓞(1)" and "Insertion or removal of elements - linear in the distance to the end of the vector 𝓞(n)"[^7].

**Reserving memory.** When the program knows the final number of items, it can make the capacity big enough at the start, and then no append copies. Java has `ensureCapacity`, and the documentation says: "This may reduce the amount of incremental reallocation"[^3]. C# has `EnsureCapacity`, C++ has `reserve`, Rust has `with_capacity` and `reserve`, and Go has `make([]T, 0, n)`. cppreference warns that "inappropriate uses of reserve() (for instance, calling it before every push_back() call) may actually increase the number of reallocations (by causing the capacity to grow linearly rather than exponentially)"[^8].

**Shrinking.** Most dynamic arrays do not give memory back when items are removed. Rust says: "Vec will never automatically shrink itself, even if completely empty"[^4]. A program can ask for a smaller array with `shrink_to_fit` in Rust and C++, `trimToSize` in Java and `TrimExcess` in C#. .NET explains when to do it: "If the capacity is significantly larger than the count and you want to reduce the memory used by the List<T>, you can decrease capacity by calling the TrimExcess method"[^9]. Wikipedia notes that some dynamic arrays shrink by themselves when the length drops below a threshold, "such as 30% of the capacity"[^1].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `list`, a dynamic array of references with `append`, `extend`, `insert` and `pop`; the capacity is hidden |
| JavaScript | native | `Array` with `push`, `pop`, `unshift` and a writable `length`; resizable `ArrayBuffer` for bytes |
| TypeScript | native | the JavaScript `Array`, typed as `T[]` |
| Java | stdlib | `java.util.ArrayList<E>` with `ensureCapacity` and `trimToSize`; the capacity is hidden |
| Kotlin | stdlib | `mutableListOf` and `ArrayList<E>`, which is the Java class on the JVM |
| C# | stdlib | `System.Collections.Generic.List<T>` with `Capacity`, `EnsureCapacity` and `TrimExcess` |
| Go | native | slices with `append`, `len` and `cap`; `make([]T, len, cap)` reserves capacity |
| Rust | stdlib | `Vec<T>` with `push`, `capacity`, `reserve`, `with_capacity` and `shrink_to_fit` |
| C | idiom | a struct with a pointer, a length and a capacity, grown with `realloc` |
| C++ | stdlib | `std::vector<T>` with `push_back`, `capacity`, `reserve` and `shrink_to_fit` |
| Bash | native | indexed arrays grow with `a+=(x)`; they are not stored as contiguous arrays |
| PowerShell | stdlib | `[System.Collections.Generic.List[int]]::new()` from .NET; `+=` on an array copies the whole array |
| SQL | n/a | tables grow by rows; array columns where they exist are values, not growable buffers |
| Regex | n/a | a pattern matches text and has no collections |
| HTML | n/a | markup has no collections |
| CSS | n/a | style rules have no collections |

The dynamic array is the default list in most languages. The Python FAQ says: "CPython’s lists are really variable-length arrays, not Lisp-style linked lists. The implementation uses a contiguous array of references to other objects"[^10]. Java describes `ArrayList` as a "Resizable-array implementation of the List interface"[^3]. .NET says that `List<T>` "implements the IList<T> generic interface by using an array whose size is dynamically increased as required"[^11]. Rust calls `Vec<T>` "A contiguous growable array type"[^4]. cppreference says of `std::vector`: "The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using offsets to regular pointers to elements"[^7].

Go builds the dynamic array into the language with slices and the `append` function. The specification says: "If the capacity of s is not large enough to fit the additional values, append allocates a new, sufficiently large underlying array that fits both the existing slice elements and the additional values. Otherwise, append re-uses the underlying array"[^12]. The Go blog says that this "technique is how dynamic array implementations from other languages work behind the scenes"[^13]. C has no dynamic array in its library, so programs write a struct with a pointer, a length and a capacity, and they grow it with `realloc`.

JavaScript arrays hide their storage. The `length` of an array "represents the number of slots in that array"[^14], and the engine decides how to store the slots. A program can set `length`: a smaller value removes items, and "When length is set to a bigger value than the current length, the array is extended by adding empty slots, not actual undefined values"[^14]. For binary data, "ArrayBuffer objects can be made resizable by including the maxByteLength option when calling the ArrayBuffer() constructor"[^15].

The example files show these forms. Python shows that a list asks for memory only now and then, and shows `extend`, `pop`, `insert` and a list made at its full size. JavaScript shows `push`, a writable `length`, holes, `unshift` and a resizable `ArrayBuffer`. Java shows `ArrayList`, `ensureCapacity`, `trimToSize` and the exception when a loop changes the list. C# prints the capacities of a `List<T>` and shows `EnsureCapacity`, `TrimExcess` and the same exception. Go prints the capacities of a slice and shows two appends that share one array. Rust prints the capacities of a `Vec` and shows `with_capacity` and `shrink_to_fit`. C builds a dynamic array with `realloc`. C++ prints the capacities of a `std::vector` and shows that a push past the capacity moves the items.

## Variations and related ideas

- **Bounded arrays.** Some arrays keep a count of the items in use but never grow. They work like a dynamic array with a fixed maximum capacity, and an append to a full array fails. A ring buffer is also bounded.
- **Slices.** A Go slice is a view of a dynamic array: "A slice is a descriptor of an array segment. It consists of a pointer to the array, the length of the segment, and its capacity (the maximum length of the segment)"[^13]. Two slices can share one array, so an append to one can change the other. See [slice](/slice-concept/).
- **Deques and gap buffers.** A plain dynamic array is fast only at the end. A double-ended queue, such as Python `collections.deque`, Java `ArrayDeque` or Rust `VecDeque`, is also fast at the front. A gap buffer keeps the free slots at the cursor of a text editor, so inserts near the cursor are fast.
- **Linked lists.** The other common way to store a [list](/list/) is a [linked list](/linked-list/), a chain of cells with one item each. It inserts at the front in constant time but reaches item `i` in `i` steps.
- **Persistent vectors.** Functional languages such as Clojure and Scala use trees of small arrays, so a change makes a new version and keeps the old one.
- **Strings and builders.** A string builder, such as Java `StringBuilder` or Rust `String`, is a dynamic array of characters or bytes. See [build a string efficiently](/build-a-string-efficiently/).
- **Linked lists.** A linked list adds at the front in constant time but reads item `i` in linear time. Wikipedia compares them: a dynamic array has good "locality of reference and data cache utilization, compactness (low memory use), and random access"[^1].

## Pitfalls

- **Pointers into a dynamic array become invalid.** When the array grows, the items move to new memory. cppreference says that after `push_back`, if "the new size() is greater than old capacity() a reallocation takes place, in which case all iterators (including the end() iterator) and all references to the elements are invalidated"[^16]. A C or C++ pointer to an item then points to freed memory, which is a [use after free](/use-after-free/). Rust rejects this program at compile time, because a reference to an item borrows the `Vec`, and `push` needs a mutable borrow. See [ownership](/ownership/).
- **A change during iteration.** Java and C# detect a change to a list during a `for` loop over it and throw `ConcurrentModificationException` or `InvalidOperationException`. Python does not detect it, and the loop can skip items or never end.
- **Shared backing in Go.** Two appends to one slice that has spare capacity write into the same array. In the Go example, `x` and `y` both end with 2. When the program needs a separate slice, copy it first with `slices.Clone`.
- **An insert at the front in a loop.** `insert(0, x)`, `add(0, x)` and `unshift` move all the items, so `n` inserts at the front take quadratic time. Append and then reverse, or use a deque.
- **Growing by a constant.** A custom dynamic array that adds a fixed number of slots, or a loop that grows a plain array by one with `Array.Resize` or PowerShell `+=`, does quadratic work. Grow by a factor.
- **Memory that is not given back.** A list that once had a million items keeps the large array after `clear` or `truncate`. Call `shrink_to_fit`, `trimToSize` or `TrimExcess`, or make a new list.
- **realloc in C.** `p = realloc(p, n)` loses the old block if `realloc` fails and returns `NULL`. Keep the result in a second variable, as the C example does, and also check that `n * sizeof *p` does not overflow.
- **reserve in a loop.** In C++, a `reserve(size() + 1)` before each `push_back` can turn the geometric growth into linear growth, which is quadratic work[^8].

## Cost

| Operation | Cost |
|---|---|
| read or write item `i` | constant |
| append at the end | amortized constant; linear when the array grows |
| remove at the end | constant |
| insert or remove at position `i` | linear in the number of items after `i` |
| iterate over all items | linear, with good cache use |
| extra memory | up to about `(a - 1) * n` slots for growth factor `a` |

A dynamic array uses more memory than a plain array with the same items. cppreference says: "Vectors usually occupy more space than static arrays, because more memory is allocated to handle future growth. This way a vector does not need to reallocate each time an element is inserted, but only when the additional memory is exhausted"[^7]. Wikipedia gives the bound: "the number of wasted cells is bounded above by (a−1)n"[^1]. The header is small: Rust and Go keep a pointer, a length and a capacity. The single growth step is slow, so a dynamic array is a poor choice where each append must finish within a fixed time, as in some real-time code.

## History

Wikipedia lists dynamic arrays in many libraries: "C++'s std::vector and Rust's std::vec::Vec are implementations of dynamic arrays", and so are `java.util.ArrayList` in Java and `System.Collections.ArrayList` in the .NET Framework. It adds: "The generic System.Collections.Generic.List<T> class in version 2.0 of the .NET Framework is also implemented with dynamic arrays"[^1]. Java added `ArrayList` in version 1.2, with the Java Collections Framework, and the documentation says that it "is roughly equivalent to Vector, except that it is unsynchronized"[^3]. Go made the dynamic array part of the language with slices and the built-in `append`[^12].

The analysis of the cost came later than the data structure. Wikipedia says that amortized analysis "was first formally introduced by Robert Tarjan in his 1985 paper Amortized Computational Complexity"[^2], and that "Dynamic arrays are a common example when teaching amortized analysis"[^1].

## References
