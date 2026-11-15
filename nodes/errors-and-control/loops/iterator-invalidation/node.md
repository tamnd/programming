---
tag: JEYEDG
kind: concept
slug: iterator-invalidation
name: Iterator invalidation
aliases: [iterator invalidation, invalidated iterator, concurrent modification, ConcurrentModificationException, fail-fast iterator, modifying a collection while iterating, collection was modified, dictionary changed size during iteration, dangling iterator]
area: errors-and-control/loops
level: advanced
summary: >-
  An iterator is invalidated when its collection changes in a way that the iterator cannot follow. Languages throw an error, skip elements, have undefined behavior or reject the code at compile time.
applies_to: core-languages
viz: optional
formal: none
see_also: [iterators, for-each-loop, loop, ownership, copy-on-write, data-races, dynamic-array]
sources:
- url: https://doc.rust-lang.org/book/ch08-01-vectors.html
  title: Storing Lists of Values with Vectors, The Rust Programming Language
  locator: Listing 8-6; Iterating Over the Values in a Vector
- url: https://en.cppreference.com/w/cpp/container.html
  title: Containers library, C++ reference, cppreference.com
  locator: Iterator invalidation
- url: https://en.cppreference.com/w/cpp/container/vector/push_back.html
  title: std::vector::push_back, C++ reference, cppreference.com
  locator: Description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html
  title: ArrayList (Java SE 25 and JDK 25)
  locator: Class description, fail-fast iterators; remove(int index)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ConcurrentModificationException.html
  title: ConcurrentModificationException (Java SE 25 and JDK 25)
  locator: Class description
- url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.getenumerator
  title: List<T>.GetEnumerator Method, .NET API reference
  locator: Remarks
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Dictionary view objects
- url: https://docs.python.org/3/tutorial/controlflow.html
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.2 for Statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
  title: Array, MDN Web Docs
  locator: Mutating initial array in iterative methods
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
---

## Idea

A loop over a list removes the elements that it does not want. The loop looks right, but it skips elements, or it stops with an error, or in C++ it reads freed memory. The loop holds an iterator, a position in the list, and the removal moves the elements under it. The iterator is invalidated: it no longer points where the loop thinks it points.

```python run id=idea
# Removing from a list while looping over it skips elements.
nums = [1, 2, 2, 3, 4]
for n in nums:
    if n % 2 == 0:
        nums.remove(n)
print("in place:", nums)

# Loop over a copy, or build a new list.
nums = [1, 2, 2, 3, 4]
for n in nums[:]:
    if n % 2 == 0:
        nums.remove(n)
print("over a copy:", nums)
print("new list:", [n for n in [1, 2, 2, 3, 4] if n % 2 != 0])

# A dictionary checks its size and raises an error.
ages = {"ann": 31, "bob": 17, "cy": 12}
try:
    for name in ages:
        if ages[name] < 18:
            del ages[name]
except RuntimeError as e:
    print("RuntimeError:", e)

ages = {"ann": 31, "bob": 17, "cy": 12}
for name in list(ages):
    if ages[name] < 18:
        del ages[name]
print(ages)
```

```output for=idea
in place: [1, 2, 3]
over a copy: [1, 3]
new list: [1, 3]
RuntimeError: dictionary changed size during iteration
{'ann': 31}
```

The first loop removes the first 2, and the list moves the second 2 into its place. The loop goes on to the next position, so it never sees the second 2, and the 4 is removed only because it comes later. The loop over the copy `nums[:]` sees all five elements. The dictionary notices that its size changed and raises an error, and the loop over `list(ages)` takes the keys first.

## How it works

**Why iterators break.** An iterator is a position in a collection, often a pointer or an index. A change to the collection can move the elements, free the memory that holds them, or rebuild a hash table. The Rust book gives the reason for a vector: "adding a new element onto the end of the vector might require allocating new memory and copying the old elements to the new space, if there isn't enough room to put all the elements next to each other where the vector is currently stored. In that case, the reference to the first element would be pointing to deallocated memory"[^1].

**C++: rules for each container.** C++ lists, for each container, which changes invalidate which iterators. cppreference says that "Read-only methods never invalidate iterators or references. Methods which modify the contents of a container may invalidate iterators and/or references"[^2]. For a vector, `push_back` invalidates all iterators when it must grow: "If after the operation the new size() is greater than old capacity() a reallocation takes place, in which case all iterators (including the end() iterator) and all references to the elements are invalidated. Otherwise only the end() iterator is invalidated"[^3]. A `list` keeps its other iterators valid, and an erasure invalidates only the erased elements[^2]. The use of an invalid iterator is undefined behavior: the program can crash, print garbage or seem to work.

**Java: fail-fast iterators.** The iterators of `ArrayList` "are fail-fast: if the list is structurally modified at any time after the iterator is created, in any way except through the iterator's own remove or add methods, the iterator will throw a ConcurrentModificationException"[^4]. A structural modification "is any operation that adds or deletes one or more elements, or explicitly resizes the backing array; merely setting the value of an element is not a structural modification"[^4]. The check is not a promise: "Fail-fast iterators throw ConcurrentModificationException on a best-effort basis"[^4]. The name of the exception is misleading. "Note that this exception does not always indicate that an object has been concurrently modified by a different thread"[^5]: one thread that changes a list in its own for-each loop gets it too.

**C#: the enumerator is invalidated.** In .NET, "An enumerator remains valid as long as the collection remains unchanged. If changes are made to the collection, such as adding, modifying, or deleting elements, the enumerator is irrecoverably invalidated and the next call to MoveNext or IEnumerator.Reset throws an InvalidOperationException"[^6].

**Python: a check for dictionaries and sets, none for lists.** "Iterating views while adding or deleting entries in the dictionary may raise a RuntimeError or fail to iterate over all entries"[^7]. A list has no check, so a loop that removes elements skips some, as the example shows. The tutorial advises: "Code that modifies a collection while iterating over that same collection can be tricky to get right. Instead, it is usually more straight-forward to loop over a copy of the collection or to create a new collection"[^8].

**JavaScript: defined but surprising.** The array methods such as `forEach` have exact rules. "only indexes between 0 and arrayLength - 1 are visited, where arrayLength is the length of the array at the time the array method was first called"[^9], so elements added at the end are not visited, and "Removed elements are not visited"[^9]. MDN still warns that such changes "frequently lead to hard-to-understand code and are generally to be avoided"[^9].

**Go: defined for maps.** The Go specification says what happens when a `range` loop changes a map: "If a map entry that has not yet been reached is removed during iteration, the corresponding iteration value will not be produced. If a map entry is created during iteration, that entry may be produced during the iteration or may be skipped"[^10]. For a slice, "The range expression x is evaluated before beginning the loop"[^10], so an `append` in the body does not make the loop longer.

**Rust: rejected at compile time.** Rust's borrow rules forbid the change. "Iterating over a vector, whether immutably or mutably, is safe because of the borrow checker's rules"[^1]. A `for` loop over `&v` borrows the vector, and "The reference to the vector that the for loop holds prevents simultaneous modification of the whole vector"[^1]. The compiler reports error E0502, "cannot borrow `v` as mutable because it is also borrowed as immutable"[^1].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `RuntimeError` when a dict or set changes size during a loop; no check for lists |
| JavaScript | native | array methods visit the indexes below the first length; `Map` and `Set` iterators see new entries |
| TypeScript | native | the rules of JavaScript |
| Java | native | fail-fast iterators throw `ConcurrentModificationException`; `Iterator.remove` and `removeIf` are safe |
| Kotlin | native | the collections of Java; `MutableIterator.remove` and `removeAll` |
| C# | native | `InvalidOperationException` on the next `MoveNext` after a change; `RemoveAll` is safe |
| Go | native | a `range` over a map defines what happens on a change; a `range` over a slice evaluates it once |
| Rust | native | the borrow checker rejects a change to a collection while a loop borrows it; `retain` is safe |
| C | absent | no iterators in the language; a pointer into a buffer dangles after `realloc` |
| C++ | native | each container lists which changes invalidate iterators; the use of an invalid iterator is undefined behavior |
| Bash | n/a | `for x in "${a[@]}"` expands the array once, before the loop starts |
| PowerShell | native | `foreach` over a list that changes fails with an error that the collection was modified |
| SQL | n/a | no iterators; an `UPDATE` or `DELETE` works on a set of rows |
| Regex | n/a | no collections |
| HTML | n/a | no collections |
| CSS | n/a | no collections |

The languages pick one of four answers. Java, C#, Python dictionaries and PowerShell detect the change and throw an error. JavaScript and Go define the result, so the loop goes on with rules that a reader must know. C++ leaves the result undefined and documents which operations are safe. Rust rejects the program before it runs.

The example files show these differences. Python removes from a list in place, over a copy and with a comprehension, and catches the error of a dictionary. JavaScript adds and removes elements in `forEach` and filters into a new array. Java catches a `ConcurrentModificationException` and removes with `Iterator.remove` and `removeIf`. C# catches an `InvalidOperationException` and uses `RemoveAll`. Go deletes map entries in a `range` loop and appends to a slice in one. Rust uses `retain` and collects the changes before it applies them. C keeps an index, not a pointer, across a `realloc`. C++ uses the iterator that `erase` returns and `std::erase_if`.

## Variations and related ideas

- **Removal through the iterator.** Java's `Iterator.remove` and the `erase` of C++, which returns the next valid iterator, change the collection in a way that the iterator follows.
- **Removal in one call.** `removeIf` in Java, `RemoveAll` in C#, `retain` in Rust and `std::erase_if` in C++ remove all the matching elements without a loop in user code.
- **Two passes.** Collect the keys to remove in the first loop, then remove them in a second loop.
- **A new collection.** A filter or a comprehension builds a new collection and does not change the old one.
- **Loops by index from the end.** A loop that goes from the last index down to 0 can remove elements, because a removal moves only the elements that it has already passed.
- **Snapshots.** A copy-on-write collection gives the loop a snapshot, so changes do not affect it. See [copy-on-write](/copy-on-write/).
- **Threads.** When another thread changes the collection, the problem is a [data race](/data-races/), and a fail-fast check does not fix it.

## Pitfalls

- **Errors that come late or not at all.** Java's check is best-effort, so "it would be wrong to write a program that depended on this exception for its correctness"[^5]. In Python, a list gives no error at all.
- **References and pointers, not only iterators.** In C++, a reference or a pointer to a vector element is invalidated by the same reallocation as an iterator[^3].
- **The `end()` iterator.** A C++ loop that saves `end()` before the loop and inserts into a vector compares with an invalid iterator. cppreference notes that "std::vector::end is always invalidated"[^2] by a change.
- **Erase in a for loop.** In C++, `for (auto it = v.begin(); it != v.end(); ++it) if (bad(*it)) v.erase(it);` uses `it` after the erase. Use `it = v.erase(it)` and step only when nothing was erased.
- **Changing a value is often allowed.** Setting an element of an `ArrayList` is "not a structural modification"[^4], so it does not throw. Changing a key of a hash table is different: it moves the entry.

## Cost

A fail-fast check costs a counter in the collection and a comparison in each step of the iterator. Copying the collection before the loop costs memory and time in proportion to its size. A removal in one call, such as `retain` or `removeIf`, moves each element at most once, but a loop that removes elements one by one from an array list can take quadratic time. Java's `ArrayList.remove` "Shifts any subsequent elements to the left"[^4], so each removal moves all the elements after it.

## References
