---
tag: 3VCXKD
kind: concept
slug: reference-counting
name: Reference counting
aliases: [refcounting, RC, reference-counted pointer, shared ownership, automatic reference counting]
area: memory-and-resources/automatic-memory-management
level: core
summary: >-
  Reference counting stores in each object the number of references to it and frees the object when the number reaches zero. Objects in a cycle never reach zero.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1960
  people: [George E. Collins]
  note: Collins described reference counts in list cells, so that a list could be erased without erasing the parts that other lists still shared.
see_also: [garbage-collection, rust/ownership, explicit-memory-management, memory-leaks, resource-acquisition-is-initialization, threads, data-races, use-after-free]
same_as:
  wikidata: Q1352761
  wikipedia: Reference_counting
sources:
  - paper: doi:10.1145/367487.367501
    title: George E. Collins, A method for overlapping and erasure of lists, Communications of the ACM 3(12), 1960
    locator: the reference count of each list cell and erasure when the count reaches zero
  - paper: doi:10.1145/360336.360345
    title: L. Peter Deutsch and Daniel G. Bobrow, An efficient, incremental, automatic garbage collector, Communications of the ACM 19(9), 1976
    locator: abstract, reference counts that need to be accurate only when storage is about to be reclaimed, the transaction file and the multiple reference table
  - url: https://docs.python.org/3.14/c-api/refcounting.html#c.Py_DECREF
    title: The Python/C API Reference Manual, Reference Counting, Python 3.14
    locator: Py_REFCNT, Py_INCREF, Py_DECREF and its warning
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/c-api/structures.html#c.PyObject.ob_refcnt
    title: The Python/C API Reference Manual, Common Object Structures, Python 3.14
    locator: PyObject and PyObject.ob_refcnt
    accessed: 2026-11-10
  - url: https://docs.python.org/3.14/library/sys.html#sys.getrefcount
    title: The Python Standard Library, sys, Python 3.14
    locator: sys.getrefcount and sys._is_immortal
    accessed: 2026-11-10
  - url: https://peps.python.org/pep-0683/
    title: "PEP 683: Immortal Objects, Using a Fixed Refcount"
    locator: Abstract; Motivation, Reducing CPU Cache Invalidation and Avoiding Copy-on-Write; Specification; Public Refcount Details; the header fields Created and Python-Version
    accessed: 2026-11-10
  - url: https://peps.python.org/pep-0703/#reference-counting
    title: "PEP 703: Making the Global Interpreter Lock Optional in CPython"
    locator: Reference Counting; Immortalization; Biased Reference Counting; Deferred Reference Counting; Integration With Deferred and Biased Reference Counting; the header fields Python-Version and Resolution
    accessed: 2026-11-10
  - url: https://github.com/python/cpython/blob/main/InternalDocs/garbage_collector.md
    title: CPython InternalDocs, Garbage collector design
    locator: Identifying reference cycles
    accessed: 2026-11-10
  - url: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/automaticreferencecounting/
    title: The Swift Programming Language, Automatic Reference Counting
    locator: the introduction; How ARC Works; Weak References; Unowned References
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/rc/index.html
    title: The Rust Standard Library, module std::rc
    locator: module documentation, the paragraphs on non-atomic counts, Send, Weak and cycles
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/rc/struct.Rc.html#method.strong_count
    title: The Rust Standard Library, struct std::rc::Rc
    locator: downgrade, weak_count, strong_count, get_mut
    accessed: 2026-11-10
  - url: https://doc.rust-lang.org/std/sync/struct.Arc.html#thread-safety
    title: The Rust Standard Library, struct std::sync::Arc
    locator: struct description and Thread Safety
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/util.smartptr.shared
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[util.smartptr.shared.general] paragraphs 1 and 5; [util.smartptr.shared.const] paragraphs 6 to 8; [util.smartptr.shared.dest]; [util.smartptr.shared.obs] use_count"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/util.smartptr.shared.create
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[util.smartptr.shared.create] paragraph 7.1"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/util.smartptr.weak
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[util.smartptr.weak.general] and [util.smartptr.weak.obs] expired and lock"
    accessed: 2026-11-10
  - url: https://timsong-cpp.github.io/cppwp/n4950/util.smartptr.enab
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[util.smartptr.enab] paragraphs 1 and 7, shared_from_this"
    accessed: 2026-11-10
  - url: https://clang.llvm.org/docs/AutomaticReferenceCounting.html#general
    title: Clang documentation, Objective-C Automatic Reference Counting (ARC)
    locator: Background (retain, release and autorelease) and General
    accessed: 2026-11-10
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/unlink.html
    title: The Open Group Base Specifications Issue 8 (POSIX.1-2024), unlink
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/atomic/AtomicInteger.html
    title: Java SE 25 API, java.util.concurrent.atomic.AtomicInteger
    locator: incrementAndGet() and decrementAndGet()
    accessed: 2026-11-10
  - url: https://pkg.go.dev/sync/atomic#Int32.Add
    title: Go standard library, package sync/atomic
    locator: package overview and type Int32, method Add
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.interlocked.decrement
    title: .NET API, Interlocked.Decrement Method
    locator: the summary of the overloads
    accessed: 2026-11-10
---

## Idea

Some objects have more than one user. A buffer can be read by a parser and kept by a cache at the same time. No single part of the program knows when the last user is done, so no single part can free the buffer safely. Reference counting solves this with a number in each object: the count of references that point to it. Each added reference adds one, and each reference that goes away subtracts one. When the count reaches zero, nothing can reach the object any more, and the object is freed at that moment.[^1]

CPython keeps such a count for every object. The count lives in the header of the object.[^4] In this run, two list entries add two references, and clearing the list removes them. The object is freed on the line that drops its last name.

```python run id=idea
import sys


class Buffer:
    def __del__(self):
        print("buffer freed")


buffer = Buffer()
base = sys.getrefcount(buffer)
users = [buffer, buffer]
print("references added by the list:", sys.getrefcount(buffer) - base)
users.clear()
print("back to the start:", sys.getrefcount(buffer) == base)
print("before del")
del buffer
print("after del")
```

```output
references added by the list: 2
back to the start: True
before del
buffer freed
after del
```

The example compares two counts instead of printing one, because `sys.getrefcount` includes the temporary reference to its own argument.[^5] Reference counting is one of the two main ways of automatic memory management. The other is the tracing collection that [[garbage-collection]] describes.

## How it works

Every reference-counted object carries a count, and two operations change it. Retain (increment) runs when the program stores a reference in a variable, a field or a container. Release (decrement) runs when such a reference goes away. The release that brings the count to zero frees the object. Before the memory goes, the object releases the references that it holds itself. That can bring other counts to zero, so one release can free a whole structure.[^1][^6]

```text
before: report and cache both refer to Page

report ─┐
        ├──> Page (count 2) ──> Header (count 1)
cache  ─┘

release report:  Page (count 1)               nothing is freed
release cache:   Page (count 0)   free Page
                   └─ release Header: Header (count 0)   free Header
```

```pseudo
RETAIN(x)                                 ▷ a new reference to x
L1  count(x) ← count(x) + 1

RELEASE(x)                                ▷ one reference to x goes away
L2  count(x) ← count(x) − 1
L3  if count(x) = 0 then
L4      for each reference from x to an object y do RELEASE(y)
L5      if weak(x) = 0 then free the memory of x
L6      else destroy the value of x and keep its memory for the weak references
```

Who calls retain and release depends on the language. In C code that uses CPython, the programmer calls `Py_INCREF` to take a strong reference and `Py_DECREF` to give it back. When the last strong reference goes, `Py_DECREF` calls the deallocation function of the type of the object.[^3] Python code never calls them: the interpreter does it as values move through variables and containers. A Rust `Rc` or a C++ `shared_ptr` is a value that owns one reference. Its copy (`Rc::clone`, the copy constructor) retains, and its drop or destructor releases.[^10][^13] Swift and Objective-C under ARC let the compiler insert the retains and releases.[^9][^17]

**Weak references.** A weak reference points to an object but does not add to its count. So it does not keep the object alive. Before use, the program must turn it into a strong reference, and that step fails when the object is gone. Rust calls this `upgrade` on a `Weak`, and it returns `None` after the value is dropped.[^10] C++ calls it `weak_ptr::lock`, which returns an empty `shared_ptr` when the object has expired.[^15] A Swift `weak` reference is always optional, and ARC sets it to `nil` when the instance is freed.[^9] To answer "is it gone?", the runtime keeps a second count, of weak references. In Rust, a `Weak` keeps the allocation alive but not the value inside it.[^10] That is line L6 of the pseudocode.

**The limit: cycles.** A count tells how many references point to an object, not whether the program can still reach it. Two objects that refer to each other each keep a count of at least one, even after the program has lost both. Reference counting alone never frees a cycle. The Rust standard library says that a cycle of `Rc` pointers will never be deallocated. For a tree, it suggests strong pointers from parents to children and `Weak` pointers from children to parents.[^10] CPython adds a cycle collector for this case, and Swift and Objective-C leave it to the programmer.[^8][^9][^17]

## In languages

The example files show the native and standard library forms in Python, Rust, C++ and C. The languages with a tracing collector have no counts for memory. Their files use a count for a resource instead: a shared connection that closes when its last user releases it.

| Language | Verdict | Form |
|---|---|---|
| Python | native | CPython: a count in each object header, a cycle collector for cycles; immortal objects keep a fixed count[^4][^6] |
| Rust | stdlib | `Rc` (plain counts, single thread) and `Arc` (atomic counts), with `Weak` for both[^10][^12] |
| C++ | stdlib | `std::shared_ptr` and `std::weak_ptr`; `std::make_shared` puts the object and the counts in one allocation[^13][^14] |
| C | idiom | a count field in the struct, with retain and release functions that every caller pairs by hand |
| Java | idiom | tracing collector; `AtomicInteger` counts the users of a resource[^19] |
| JavaScript | idiom | tracing collector; a plain number counts the users of a resource |
| TypeScript | idiom | as JavaScript |
| C# | idiom | tracing collector; `Interlocked.Increment` and `Interlocked.Decrement` count the users of a resource[^21] |
| Go | idiom | tracing collector; `sync/atomic` counts the users of a resource[^20] |
| Kotlin | idiom | the collector of the platform; on the JVM, `AtomicInteger` as in Java |
| PowerShell | absent | the .NET collector frees objects; scripts dispose resources instead |
| Bash | n/a | a script has no references to objects |
| SQL | n/a | a query has no references to objects |
| Regex | n/a | a pattern has no objects |
| HTML/CSS | n/a | markup and style have no objects |

In CPython, each object starts with its reference count and a pointer to its type.[^4] The C API says to trust only the values 0 and 1, because some objects are immortal and report a very high count.[^3] The free-threaded build changes the representation, as the Variations section describes, but not the rule.[^7]

Rust and C++ make shared ownership a library type. Most values have one owner and need no count (see [[rust/ownership]] and [[resource-acquisition-is-initialization]]). `Rc<T>`, `Arc<T>` and `shared_ptr<T>` are for a value with several owners, where no owner is known to outlive the others. `Rc::strong_count` and `Rc::weak_count` read the two counts, and `shared_ptr::use_count` reads the strong count.[^11][^13] With several threads, the value of `use_count` is only approximate.[^13]

C has no smart pointers, so a library writes the count into its own structs. It documents which functions return a reference that the caller must release. A missing release is a [[memory-leaks|memory leak]]. An extra release frees the object while a pointer to it is still in use, which leads to a [[use-after-free]]. These are the errors of [[explicit-memory-management]], one level up.

Java, JavaScript, TypeScript, C#, Go and Kotlin free memory with a tracing collector. A count still helps for a resource that must close at a known time, such as a connection or a file. With [[threads]], each change of the count must be one atomic step. Otherwise two threads can read the same old value, and the resource closes twice or never. The example files use the atomic types of each standard library.[^19][^20][^21]

## Variations and related ideas

- **Compiler-inserted counts.** Objective-C objects have `retain` and `release` methods. Under ARC, the compiler inserts these calls, and ARC has no cycle collector.[^17] Swift ARC counts references to class instances only. Structures and enumerations are value types and have no count.[^9]
- **Strong, weak and unowned.** Swift has three kinds of reference. A strong reference keeps the instance alive. A weak reference does not, and becomes `nil` when the instance goes. An unowned reference does not keep the instance alive either, but it is not optional. It is for an instance with the same or a longer lifetime. An access after that instance is freed is a runtime error.[^9]
- **Cycle collection.** CPython keeps reference counting and adds a pass that finds cycles. For each container object in the set that it scans, it copies the count and subtracts each reference that comes from inside the set. An object that keeps a count above zero has a reference from outside. The collector keeps it and everything that it reaches, and frees the rest.[^8]
- **Deferred reference counting.** Deutsch and Bobrow kept the counts accurate only when storage was about to be reclaimed. They wrote the count changes to a transaction file and kept counts only for items with more than one reference.[^2] The free-threaded CPython build defers the counts of top-level functions, code objects, modules and methods. The interpreter skips their count updates when it pushes them on its stack and pops them. The true count is then known only when all threads pause for a collection, so these objects are freed only during collection.[^7]
- **Biased reference counting.** Most objects are used by one thread only. The free-threaded build gives each object an owner thread. The owner changes a local count with plain instructions. Other threads change a shared count with atomic instructions. The two counts are merged before the object can be freed.[^7]
- **Immortal objects.** CPython marks some objects, such as `None`, with a fixed count. `Py_INCREF` and `Py_DECREF` do not change it, so the object is never freed while the runtime runs.[^6][^3] `sys._is_immortal` reports whether an object is immortal.[^5]
- **Intrusive and separate counts.** A CPython object, a C struct with a count field and an Objective-C object hold their count inside the object. A `shared_ptr` made from a raw pointer must keep its counts elsewhere. `std::make_shared` should do only one allocation, and the standard notes that this gives the efficiency of an intrusive smart pointer.[^14]
- **The unique-owner test.** A count of one means that no one else can see the value. Rust uses this in `Rc::get_mut`, which returns a mutable reference only when there are no other `Rc` or `Weak` pointers.[^11] `Arc::make_mut` clones the value only when it is shared.[^12]
- **File system links.** A POSIX file has a link count. `unlink` removes a name and decrements the count. The file system frees the file when the count is 0 and no process has the file open.[^18]

## Pitfalls

### A cycle never reaches zero

A tree where each child holds a strong pointer to its parent is a cycle. The parent owns the child, and the child owns the parent. When `main` drops its own pointers, both counts stay above zero. No `drop` line prints, and both folders leak until the process ends.

```rust wrong id=strong-parent
use std::cell::RefCell;
use std::rc::Rc;

struct Folder {
    name: String,
    parent: Option<Rc<Folder>>,
    children: RefCell<Vec<Rc<Folder>>>,
}

impl Drop for Folder {
    fn drop(&mut self) {
        println!("drop {}", self.name);
    }
}

fn main() {
    let root = Rc::new(Folder { name: "root".into(), parent: None, children: RefCell::new(Vec::new()) });
    let docs = Rc::new(Folder { name: "docs".into(), parent: Some(Rc::clone(&root)), children: RefCell::new(Vec::new()) });
    root.children.borrow_mut().push(Rc::clone(&docs));
    println!("root strong count: {}", Rc::strong_count(&root));
    drop(docs);
    drop(root);
    println!("end of main");
}
```

```output
root strong count: 2
end of main
```

The fix makes the link up the tree weak. The parent owns its children, and a child only observes its parent. The drop of `root` brings its count to zero, and the release of its children drops `docs` in turn.[^10]

```rust run id=weak-parent
use std::cell::RefCell;
use std::rc::{Rc, Weak};

struct Folder {
    name: String,
    parent: Weak<Folder>,
    children: RefCell<Vec<Rc<Folder>>>,
}

impl Drop for Folder {
    fn drop(&mut self) {
        println!("drop {}", self.name);
    }
}

fn main() {
    let root = Rc::new(Folder { name: "root".into(), parent: Weak::new(), children: RefCell::new(Vec::new()) });
    let docs = Rc::new(Folder { name: "docs".into(), parent: Rc::downgrade(&root), children: RefCell::new(Vec::new()) });
    root.children.borrow_mut().push(Rc::clone(&docs));
    println!("root strong {}, weak {}", Rc::strong_count(&root), Rc::weak_count(&root));
    if let Some(parent) = docs.parent.upgrade() {
        println!("parent of docs: {}", parent.name);
    }
    drop(docs);
    drop(root);
    println!("end of main");
}
```

```output
root strong 1, weak 1
parent of docs: root
drop root
drop docs
end of main
```

The same leak happens with `shared_ptr` in C++ and with strong references in Swift.[^9] In CPython, the cycle collector frees the strong version, but only when it runs.

### A weak reference outlives its target

A weak reference is not a promise that the object exists. Code that assumes the target is there fails after the owner lets it go. Here a request keeps a weak reference to its session, and the session ends first.

```python fails expect=exception:AttributeError id=dead-weakref
import weakref


class Session:
    def __init__(self, user):
        self.user = user


class Request:
    def __init__(self, session):
        self.session = weakref.ref(session)

    def user(self):
        return self.session().user


session = Session("ada")
request = Request(session)
print(request.user())
del session
print(request.user())
```

The call `self.session()` returns `None` once the session is freed, and `None` has no `user` attribute. Check the result of the upgrade every time, and decide what the code does when the target is gone. In Rust, match on the `Option` from `upgrade` instead of calling `unwrap`. In C++, test the `shared_ptr` from `lock` before use.[^10][^15]

```python run id=checked-weakref
import weakref


class Session:
    def __init__(self, user):
        self.user = user


class Request:
    def __init__(self, session):
        self.session = weakref.ref(session)

    def user(self):
        session = self.session()
        if session is None:
            return "no session"
        return session.user


session = Session("ada")
request = Request(session)
print(request.user())
del session
print(request.user())
```

```output
ada
no session
```

### An Rc cannot cross threads

`Rc` changes its counts with plain operations. Two threads that clone one `Rc` at the same time could lose an update, and the value would be freed too early or never. So `Rc` does not implement `Send`, and the compiler rejects the program.[^10]

```rust fails expect=compile id=rc-thread
use std::rc::Rc;
use std::thread;

fn main() {
    let config = Rc::new(String::from("mode=fast"));
    let copy = Rc::clone(&config);
    let worker = thread::spawn(move || copy.len());
    println!("{}", worker.join().unwrap());
}
```

`Arc` changes its counts with atomic operations, so it can go to another thread. `Arc` makes the ownership thread-safe, not the value. A value that threads change still needs a `Mutex` or an atomic type.[^12] See [[data-races]].

```rust run id=arc-thread
use std::sync::Arc;
use std::thread;

fn main() {
    let config = Arc::new(String::from("mode=fast"));
    let copy = Arc::clone(&config);
    let worker = thread::spawn(move || copy.len());
    println!("{}", worker.join().unwrap());
    println!("strong count after join: {}", Arc::strong_count(&config));
}
```

```output
9
strong count after join: 1
```

### sys.getrefcount counts its own argument

`sys.getrefcount` returns a count that is generally one higher than expected, because the call holds a temporary reference to its argument.[^5] Here only one name refers to the order, but the test for "only one reference" fails.

```python wrong id=refcount-one
import sys

order = {"id": 42}
print("only one reference:", sys.getrefcount(order) == 1)
```

```output
only one reference: False
```

The exact value also depends on the interpreter, and immortal objects report a very large count. The documentation says not to rely on the value except for 0 or 1.[^5] To check that some code adds or removes a reference, compare two counts taken the same way.

```python run id=refcount-delta
import sys

order = {"id": 42}
before = sys.getrefcount(order)
pending = [order]
print("added by the list:", sys.getrefcount(order) - before)
pending.clear()
print("removed again:", sys.getrefcount(order) == before)
```

```output
added by the list: 1
removed again: True
```

### Two owner groups for one object

A `shared_ptr` made from a raw pointer starts an owner group with a count of 1.[^13] A second `shared_ptr` made from the same raw pointer starts a second group, with its own count. Each group deletes the object when its own count reaches zero, so the object is deleted twice. The second delete is undefined behaviour. AddressSanitizer reports it as a double free.

```cpp fails expect=sanitizer:address timeout=30 id=two-groups
#include <memory>
#include <print>

struct Session {
    int user_id = 7;
};

int main() {
    Session* raw = new Session;
    std::shared_ptr<Session> first(raw);
    std::shared_ptr<Session> second(raw);
    std::println("{} {}", first.use_count(), second.use_count());
}
```

The fix makes the object with `std::make_shared` and copies the `shared_ptr`, never the raw pointer. A member function that must hand out an owner of its own object has the same problem with `shared_ptr<Session>(this)`. The class derives from `std::enable_shared_from_this` instead, and `shared_from_this()` returns an owner in the existing group.[^14][^16]

```cpp run id=one-group
#include <memory>
#include <print>

struct Session : std::enable_shared_from_this<Session> {
    int user_id = 7;
    std::shared_ptr<Session> self() { return shared_from_this(); }
};

int main() {
    auto first = std::make_shared<Session>();
    auto second = first;
    auto third = first->self();
    std::println("{} {} {}", first.use_count(), second.use_count(), third.use_count());
}
```

```output
3 3 3
```

`shared_from_this()` works only on an object that a `shared_ptr` already owns. On any other object it throws `std::bad_weak_ptr`.[^16][^13]

## Cost

- **A count in each object.** In a normal CPython build, the header of every object holds the reference count and a pointer to the type.[^4] The free-threaded build adds fields. It stores the owner thread id, a 4-byte local count, a shared count, a 1-byte mutex and a byte of collector flags.[^7]
- **A shared_ptr is two pointers wide.** The `shared_ptr` must reach both the object and its counts. In this run it is twice the size of a raw pointer. The standard does not fix this size.

```cpp run id=sizes
#include <memory>
#include <print>

int main() {
    std::println("shared_ptr / raw pointer: {}", sizeof(std::shared_ptr<int>) / sizeof(int*));
    std::println("weak_ptr / raw pointer: {}", sizeof(std::weak_ptr<int>) / sizeof(int*));
}
```

```output
shared_ptr / raw pointer: 2
weak_ptr / raw pointer: 2
```

- **A write for each copy.** Each retain and each release writes to the object, even when the program only reads the object. PEP 683 notes that every change of a count invalidates the cache line of the object. Two threads on different cores that use one object invalidate each other's caches with each increment and decrement, even when neither thread changes the object.[^6] The writes also reduce the memory that processes share after `fork`, because copy-on-write copies each page where a count changes.[^6]
- **Atomic counts.** Each clone and each drop of an `Arc`, and each copy and destruction of a `shared_ptr`, is an atomic read-modify-write. The Rust documentation says that atomic operations "are more expensive than ordinary memory accesses", and suggests `Rc` when no allocation is shared between threads.[^12] The C++ standard lets threads copy `shared_ptr` objects of the same owner group without a data race, so the library must update the count atomically.[^13] When many threads copy one pointer, they all write one cache line, and the line moves from core to core.[^6] PEP 703 calls such atomic read-modify-write operations "expensive on contemporary processors", and uses biased counting to avoid many of them.[^7]
- **Work at the moment of the release.** The release of the head of a long list frees the whole list in one step. So the time of one release can grow with the size of the structure. A release can also run code. In CPython, a deallocation can run a `__del__` method, so C code must leave its data consistent before it calls `Py_DECREF`.[^3]
- **No pause to find garbage.** In exchange, memory comes back as soon as the last reference goes. The program does not wait for a collector to find garbage. Cycles are the exception: they need a cycle collector or a design with weak links.

## History

- **1960.** George E. Collins described reference counts for lists that share ("overlap") their parts. The counts record how far the lists overlap, so the system can erase a list without erasing the parts that other lists still use.[^1]
- **1976.** L. Peter Deutsch and Daniel G. Bobrow described a collector for Lisp in which reference counts need to be accurate only when storage is about to be reclaimed. It kept a transaction file of count changes and a table for the items with more than one reference.[^2]
- **2018.** Jiho Choi, Thomas Shull and Josep Torrellas described biased reference counting, at PACT 2018, as PEP 703 records.[^7]
- **2022.** PEP 683 proposed immortal objects for CPython, with a fixed count that `Py_INCREF` and `Py_DECREF` do not change. It was accepted for Python 3.12.[^6]
- **2023.** PEP 703 was accepted on 24 October 2023 for Python 3.13. It makes the global interpreter lock optional. In the build without the lock, it uses biased counting, immortal objects and a limited form of deferred counting.[^7]

## References
