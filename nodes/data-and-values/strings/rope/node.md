---
tag: C3HG4C
kind: concept
slug: rope
name: Rope
aliases: [rope, cord, rope data structure, string tree, Ropey, crope]
area: data-and-values/strings
level: advanced
summary: >-
  A rope stores a long string as a tree of short pieces, so concatenation, insertion and deletion change a few nodes and do not copy the whole text. Text editors use it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1995
  people: [Hans-J. Boehm, Russ Atkinson, Michael Plass]
  note: Ropes were used in the Cedar environment at Xerox, and the C cords of the Boehm garbage collector have a Xerox copyright from 1993. The paper of Boehm, Atkinson and Plass in 1995 described them, and the SGI STL rope class is based on it.
see_also: [string-concept, string-memory-layout, immutability, string-indexing, grapheme-cluster]
same_as:
  wikidata: Q1156047
  wikipedia: Rope_(data_structure)
sources:
- url: https://en.wikipedia.org/wiki/Rope_(data_structure)
  title: Rope (data structure), Wikipedia
  locator: Description; Operations; Comparison with monolithic arrays
- url: https://www.boost.org/sgi/stl/ropeimpl.html
  title: Rope Implementation Overview, SGI Standard Template Library (mirror on boost.org)
  locator: Introduction; Basic Algorithms and Rope Balancing
- url: https://www.boost.org/sgi/stl/Rope.html
  title: rope<T, Alloc>, SGI Standard Template Library Programmer's Guide (mirror on boost.org)
  locator: Description; Notes
- url: https://xi-editor.io/docs/rope_science_01.html
  title: Raph Levien, Rope science, part 1, MapReduce for text, xi-editor docs
  locator: Whole post
- url: https://github.com/bdwgc/bdwgc/blob/master/include/gc/cord.h
  title: cord.h, the C cord package of the Boehm-Demers-Weiser garbage collector
  locator: Header comment
- url: https://docs.rs/ropey/latest/ropey/
  title: Crate ropey 1.6.1, docs.rs
  locator: Crate description; Low-level APIs
- url: https://zed.dev/blog/zed-decoded-rope-sumtree
  title: Thorsten Ball, Rope and SumTree, Zed Decoded, Zed blog (2024)
  locator: Problems with strings; Ropes; Why use a rope in Zed?
- url: https://code.visualstudio.com/blogs/2018/03/23/text-buffer-reimplementation
  title: Peng Lyu, Text Buffer Reimplementation, Visual Studio Code blog (2018)
  locator: Avoiding too much meta-data by using a piece table; Piece tree
- url: https://github.com/xi-editor/xi-editor/blob/master/README.md
  title: xi-editor README
  locator: Design decisions
- url: https://doi.org/10.1002/spe.4380251203
  title: Hans-J. Boehm, Russ Atkinson and Michael Plass, Ropes, an Alternative to Strings, Software Practice and Experience 25(12), 1995
  locator: pages 1315 to 1330
- url: https://xi-editor.io/docs/rope_science_00.html
  title: Raph Levien, Rope science, introduction, xi-editor docs
  locator: Whole post
---

## Idea

A flat string keeps all of its characters in one block of memory. To insert a word in the middle, a program moves every character after the insertion point, and to join two strings, it copies at least one of them. For a short string that cost is too small to see. For a file of many megabytes in a text editor, each key press can copy the whole file. A *rope* is "a type of binary tree where each leaf (end node) holds a string of manageable size and length (also known as a weight), and each node further up the tree holds the sum of the lengths of all the leaves in its left subtree"[^1]. An edit changes the tree and keeps the pieces of text.

```python run id=idea
# A rope is a tree. A leaf holds a short string, and a node joins two ropes.
class Leaf:
    def __init__(self, s):
        self.s, self.n = s, len(s)

class Node:
    def __init__(self, left, right):
        self.left, self.right = left, right
        self.n = left.n + right.n  # the length of the whole subtree
        self.weight = left.n       # the length of the left part

EMPTY = Leaf("")

def concat(a, b):
    # O(1): no characters are copied.
    if a.n == 0:
        return b
    if b.n == 0:
        return a
    return Node(a, b)

def index(r, i):
    # Walk down from the root. The cost is the depth of the tree.
    while isinstance(r, Node):
        if i < r.weight:
            r = r.left
        else:
            i, r = i - r.weight, r.right
    return r.s[i]

def split(r, i):
    # Return two ropes: the first i characters and the rest.
    if i <= 0:
        return EMPTY, r
    if i >= r.n:
        return r, EMPTY
    if isinstance(r, Leaf):
        return Leaf(r.s[:i]), Leaf(r.s[i:])
    if i < r.weight:
        a, b = split(r.left, i)
        return a, concat(b, r.right)
    a, b = split(r.right, i - r.weight)
    return concat(r.left, a), b

def insert(r, i, s):
    a, b = split(r, i)
    return concat(concat(a, Leaf(s)), b)

def text(r):
    return r.s if isinstance(r, Leaf) else text(r.left) + text(r.right)

world = Leaf("world")
r = concat(Leaf("Hello, "), world)
print(r.n, index(r, 7))
r2 = insert(r, 7, "big ")
print(text(r2))
# The old rope does not change, and the new rope shares the leaf "world".
print(text(r))
print(r2.right is world)
a, b = split(r2, 5)
print(text(a) + "|" + text(b))
```

```output for=idea
12 w
Hello, big world
Hello, world
True
Hello|, big world
```

`concat` makes one new node and copies no characters. `index` uses the weights to choose the left or the right child at each node, so it reads one leaf. `insert` is a `split` and two `concat` calls. The old rope `r` still says `Hello, world` after the insert, and the new rope uses the same leaf object for `world`, because no node is ever changed.

## How it works

**Weights.** Each node stores the length of its left subtree. To find character `i`, start at the root. If `i` is less than the weight, go left. If not, subtract the weight from `i` and go right. At a leaf, `i` is an index into a short flat string. The cost is one step per level, so it is O(log n) in a balanced tree[^1].

**Split and concat.** These two operations build all the others. `Concat` makes "a new root node with left = S1 and right = S2, which is constant time"[^1]. `Split` at position `i` walks the path to `i`, cuts the leaf there if the point is inside it, and joins the pieces on each side of the path[^1]. Then insert is a split and two concats, and delete is two splits and one concat[^1]. A substring "only copies pieces of leaf nodes at the beginning and end of the substring", and "The interior of the substring can remain shared with the original rope"[^2].

**Immutable pieces.** In the usual design, the nodes and leaves do not change after they are made, and the strings in them "are assumed to be constant immutable objects in the typical nondestructive case"[^1]. So two ropes can share nodes, and "Minor modifications of a rope can share memory with the original"[^3]. When only nondestructive operations are used, the rope is a persistent data structure, which "leads to an easy support for multiple undo levels"[^1]. A node that no rope uses any more is freed by a reference count or by a garbage collector[^2].

**Balance.** Concatenation alone can make a deep and thin tree. A rope built by appending one character at a time has a depth equal to its length, and then an index costs O(n). An implementation rebalances the tree. The SGI rope rebalances "whenever the depth is in danger of overflowing", or when the rope is deep but short, and its implementation notes say that "rebalancing typically consumes a small fraction of the running time"[^2].

**Leaves of a useful size.** A leaf of one character costs a node for each character, so real ropes join short pieces into one leaf. The SGI rope has a special case for a short leaf that is joined to the right of a rope, so that ropes built by repeated appends "will be composed of leaves of a minimum size"[^2].

**Summaries.** A node can store more than a length. If each node also stores the number of line breaks in its subtree, a program finds the start of line `k` in O(log n) time. The xi editor and the Zed editor use this idea: each node keeps a summary of its subtree, and "when you edit the text, you recompute the summary only for the nodes that changed, then bubble those up to the root"[^4].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | classes for leaves and nodes; no rope in the standard library |
| JavaScript | idiom | objects for leaves and nodes; no rope in the language |
| TypeScript | idiom | the JavaScript code with types |
| Java | idiom | a class that implements `CharSequence`; no rope in Java SE |
| Kotlin | idiom | a class that implements `CharSequence` |
| C# | idiom | records for leaves and nodes; no rope in .NET |
| Go | idiom | a struct with pointers to its children |
| Rust | idiom | an `enum` with `Rc` children; the `ropey` crate for real use |
| C | idiom | structs with pointers; the C cords of the Boehm garbage collector |
| C++ | idiom | `shared_ptr` nodes; the SGI `rope` class is an extension, not standard C++ |
| Bash | absent | no tree data types |
| PowerShell | absent | no rope type in .NET |
| SQL | absent | no rope type |
| Regex | n/a | a pattern has no data structures |
| HTML | n/a | no data structures |
| CSS | n/a | no data structures |

No language in this list has a rope in its standard library, so each example writes a small one by hand. In C++, `rope` is an SGI class, and "The rope class, and the rope header, are SGI extensions; they are not part of the C++ standard"[^3]. In C, the cord package of the Boehm garbage collector gives "immutable character strings" in which "concatenation takes constant time"[^5]. In Rust, "Ropey is a utf8 text rope for Rust"[^6], and all of its edits use `char` indexes, "which prevents accidental creation of invalid utf8 data"[^6].

The examples show different parts of the idea. Python builds a rope with 64 appends, so its depth is 63, and a rebalance makes the depth 6. JavaScript keeps each version of a buffer in an array, so an undo is a step back. Java implements `CharSequence`, so `Pattern.matcher` reads the rope with no copy, but each `charAt` walks the tree. C# cuts a long string into leaves of at most 8 characters and copies a range from only the leaves that hold it. Go stores a count of newlines in each node to find the start of a line. Rust uses `Rc` and shows that the old and the new rope share their leaves. C joins a rope with itself 20 times, so 21 nodes hold 2 MiB of text. C++ inserts into a rope of 4096 characters with 13 new nodes, where `std::string::insert` moves 2046 bytes.

## Variations and related ideas

- **Gap buffer.** A gap buffer is one array with an empty gap at the cursor, and it "allows efficient insertion and deletion operations clustered near the same location"[^1]. The author of Ropey compares the two: ropes are "not amazing at anything, but they're always solidly good"[^7], while gap buffers "degrade poorly with unfavorable editing patterns"[^7]. "Emacs uses gap buffers"[^7].
- **Piece table.** A piece table keeps the original file in one buffer, the added text in a second buffer, and a list of pieces that point into the two. The initial memory use "is close to the size of the document"[^8]. VS Code uses a piece table in a balanced tree, which its author calls a "piece tree"[^8].
- **Functions as leaves.** A leaf can be a function that makes its characters when they are read. The SGI rope says that "a piece of a rope may be a 100MByte file, which is read only when that section of the string is examined"[^3].
- **B-tree ropes.** A rope does not have to be a binary tree. The Zed rope is a SumTree, a "concurrency-friendly B-tree"[^7] in which each node stores a summary of its subtree.

## Pitfalls

- **A rope for short strings.** For a few kilobytes of text, a flat string is faster for nearly every operation. "Array-based strings have smaller overhead, so (for example) concatenation and split operations are faster on small datasets"[^1]. Use a rope when the text is large and edited often.
- **Character-by-character access.** Each index walks the tree. A loop that calls `charAt(i)` for every `i` costs O(n log n). Use an iterator, which keeps its place in a leaf[^2]. The SGI rope iterator is "slower than for vector <char> by a significant constant factor (roughly a factor of 5 or 10 if little processing is done on each character and the string is long)"[^3].
- **No rebalance.** A rope that is built only with appends and never rebalanced becomes a list, and each index is O(n).
- **Splits in the middle of a character.** A rope of UTF-8 bytes can split a leaf inside a multi-byte character. Index the rope by code points, as Ropey does, or check the boundary before the split, as the Rust example does.
- **Single-character updates.** A change of one character is a split and a concat, so "Single character replacements in a rope are expensive"[^3] compared with an array write.

## Cost

For a balanced rope of length n: index, split, insert and delete are O(log n), and concatenation is O(1) plus a rebalance when one is needed[^1]. Reading a range of j characters costs O(j + log n)[^1]. A rope uses more memory than a flat string, because of the nodes, but "The overhead is always O(n), but the constant can be made arbitrarily small"[^1] with large leaves. In practice, the SGI documentation says that "Inserting a character in the middle of a 10 megabyte rope should take on the order of 10s of microseconds", where a flat string "would take on the order of a second"[^3]. A copy of a rope is a pointer copy, so a snapshot for an undo step or for a background save is cheap. The xi editor uses this for autosave: "the persistent rope data structure is copy-on-write so this operation is nearly free"[^9].

## History

The Cedar programming environment of Xerox used ropes "almost since its inception"[^1][^2]. The C cords of the Boehm garbage collector have a Xerox copyright from 1993 and 1994, and their header lists a compiler and a text editor with an edit history as uses in which C strings would be too slow[^5]. In December 1995, Hans-J. Boehm, Russ Atkinson and Michael Plass described the data structure in "Ropes: an Alternative to Strings"[^10][^1]. The SGI STL rope class "is based loosely on the ropes in the Xerox Cedar environment or C "cords", as described in"[^2] that paper. In April and May 2016, Raph Levien worked out the design of the xi editor[^11], which uses "A persistent rope data structure"[^9] and is written in Rust. Zed, Helix and other new editors use ropes too[^7], while VS Code chose a piece tree[^8].

## References
