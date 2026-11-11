---
tag: HE663P
kind: pattern
slug: composite-pattern
name: Composite pattern
aliases: [Composite, part-whole hierarchy, object tree]
area: software-design/structural-patterns
level: core
summary: >-
  Leaves and containers share one interface, and a container forwards each call to its children. Client code then treats one object and a whole tree of objects in the same way.
family: structural-design
polarity: positive
intent: "Compose objects into tree structures that represent part-whole hierarchies, so that clients treat individual objects and compositions of objects uniformly."
also_known_as: []
refactorings: [replace-implicit-tree-with-composite]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with a drawing editor whose pictures contain lines, text and other pictures as the motivating example.
see_also: [recursion, iterators, decorator-pattern, builder-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 4, Composite (Intent, Motivation, Participants, Consequences, Implementation, Related Patterns)
  - url: https://homepages.inf.ed.ac.uk/wadler/papers/expression/expression.txt
    title: Philip Wadler, The Expression Problem, message to the java-genericity mailing list, November 12, 1998
    locator: the first paragraphs (cases as rows, functions as columns)
  - url: https://docs.python.org/3.14/library/sys.html#sys.getrecursionlimit
    title: The Python Standard Library, sys, Python 3.14
    locator: sys.getrecursionlimit and sys.setrecursionlimit
  - url: https://docs.python.org/3.14/library/exceptions.html#RecursionError
    title: The Python Standard Library, Built-in Exceptions, Python 3.14
    locator: RecursionError
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/java/awt/Container.java#L499-L507
    title: OpenJDK jdk-25+36 source, java/awt/Container.java
    locator: checkAddToSelf, lines 499 to 507; addImpl, lines 1099 to 1119; validateTree, lines 1712 to 1737
  - url: https://dom.spec.whatwg.org/#concept-node-ensure-pre-insertion-validity
    title: DOM Standard, WHATWG
    locator: 4.2.3 Mutation algorithms, ensure pre-insertion validity (step 2, HierarchyRequestError)
  - url: https://doc.rust-lang.org/book/ch15-06-reference-cycles.html#creating-a-tree-data-structure-a-node-with-child-nodes
    title: The Rust Programming Language, chapter 15.6, Reference Cycles Can Leak Memory
    locator: Creating a Tree Data Structure, a Node with Child Nodes; Adding a Reference from a Child to Its Parent
  - url: https://www.typescriptlang.org/docs/handbook/2/narrowing.html#discriminated-unions
    title: TypeScript Handbook, Narrowing
    locator: Discriminated unions
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.1.1.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.1.1.2 sealed, non-sealed, and final Classes
  - url: https://sqlite.org/lang_with.html#recursive_common_table_expressions
    title: SQLite documentation, The WITH Clause
    locator: Recursive Common Table Expressions
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.desktop/java/awt/Container.html
    title: Java SE 25 API, java.awt.Container
    locator: class description; add
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 7, Simplification, Replace Implicit Tree with Composite
---

## Intent

Compose objects into tree structures that represent part-whole hierarchies. Clients then treat a single object and a composition of objects in the same way.[^1] The client calls one operation on the root, and the tree does the work at every level.

## Problem

A program shows a directory tree with the size of each entry. A file has a size. A directory has the sum of the sizes of everything below it. A directory can hold files and other directories, to any depth.

When the program treats files and directories as different things, each operation must test which one it has. The tests repeat in every operation, and an operation that forgets a level gives a wrong answer with no error. This one handles one level of directories:

```python wrong id=one-level
project = {
    "readme.md": 200,
    "src": {"main.go": 1200, "util.go": 300},
}


def total(entries):
    result = 0
    for size in entries.values():
        if isinstance(size, int):
            result += size  # a file
        # a directory is skipped: the author handled only one level
    return result


print(total(project))
```

```output for=one-level
200
```

Design Patterns uses a drawing editor. A picture holds lines, rectangles, text and other pictures. Code that must tell the primitive objects from the containers is more complex, and the pattern removes that need.[^1] The forces are these. The structure is recursive. Most operations mean the same thing for a part and for a whole. And clients should not have to know which one they hold.

## Solution

Design Patterns has four participants.[^1]

- **Component** declares the interface for all objects in the tree, such as `size` and `lines`.
- **Leaf** is an object with no children. It does the operation itself.
- **Composite** holds children and implements the operation by calling it on each child, often with some work before or after.
- **Client** handles all objects through the component interface.

The key is that the composite is also a component. A directory is an entry, so a directory can hold directories. The recursion of the operation follows the recursion of the data (see [[recursion]]). The client calls `size` on the root and does not test anything.

### Transparency or safety

The child operations, `add`, `remove` and `children`, raise a design question. Design Patterns presents a trade-off.[^1] When the component declares them, all objects have one interface. This is transparency, but a client can call `add` on a leaf, and the leaf must fail at run time. When only the composite declares them, the compiler stops a call to `add` on a leaf. This is safety, but the client must know when it holds a composite. The Python and C++ example files choose safety: only `Directory` has children.

## Example

The example files build the same tree and print it with the size of each entry. A directory shows a slash and the total of its children:

| Language | Form |
|---|---|
| Python | an abstract base class with a leaf class and a composite class |
| JavaScript | two classes with the same methods and no base class |
| TypeScript | a discriminated union and functions that switch on the kind[^8] |
| Java | a sealed interface with two records, and `switch` with pattern matching[^9] |
| C# | an abstract class with two sealed classes |
| C++ | an abstract class, and children owned by `std::unique_ptr` |
| Go | an interface, with a slice of the interface in the composite |
| Rust | an enum with a leaf variant and a composite variant, and `match` |
| SQL | a table where each row names its parent, and a recursive common table expression[^10] |

### Objects or a sum type

The examples use two forms. Python, JavaScript, C#, C++ and Go put each operation in each class. TypeScript, Java and Rust list the cases in one type and write each operation as one function with a case for each kind. Both forms are composites, because the client calls `size` on any entry and the same code handles both kinds.

The two forms favour different changes. Wadler named this the expression problem. Take the kinds of data as rows of a table and the operations as columns. In a functional language, the rows are fixed in a datatype declaration, and a column is cheap to add. In an object-oriented language, the columns are fixed in a class declaration, and a row is cheap to add.[^2] The table below shows what each change costs here:

| Change | Classes with methods | Sum type with functions |
|---|---|---|
| add a kind of entry, such as a link | one class; no other code changes | every function changes; the compiler lists them |
| add an operation, such as a count of files | every class changes | one function |
| who can add a kind | any code, also outside the module | only the module that declares the type |

Choose by the change that the program expects. A file tree has few kinds and many operations, so a sum type suits it. A widget toolkit has many kinds of widgets that users add, so classes suit it.

## Consequences

Design Patterns lists these consequences.[^1] The pattern defines class hierarchies of primitive and composite objects, and wherever client code expects a primitive object, it can take a composite. The client code is shorter, because it does not test what it holds. Kinds of components are cheap to add, because a class that implements the component interface works with the existing composites. The liability follows from this. The design can be too general. It is harder to restrict a composite to certain kinds of children, and the type system cannot help, so the check moves to run time.[^1]

**Cycles.** A composite that can gain children after it is built can gain itself, directly or through a child. Each operation then recurses without end. In Python, the stack grows until the interpreter raises `RecursionError`:[^4]

```python fails expect=exception:RecursionError id=cycle
class Directory:
    def __init__(self, name):
        self.name = name
        self.children = []

    def add(self, child):
        self.children.append(child)

    def size(self):
        return sum(child.size() for child in self.children)


project = Directory("project")
src = Directory("src")
project.add(src)
src.add(project)  # a cycle: src is inside project, and project is inside src
print(project.size())
```

The fix is a check in `add`. A composite must not gain a child that is itself or one of its ancestors. With links to the parent, the check walks up from the composite to the root. AWT does this in `Container.checkAddToSelf`, and it throws `IllegalArgumentException` with the message "adding container's parent to itself".[^5] The DOM standard has the same rule. Insertion throws a `HierarchyRequestError` when the node to insert is an inclusive ancestor of the parent.[^6] Without parent links, search the child's subtree for the composite:

```python run id=cycle-check
class Directory:
    def __init__(self, name):
        self.name = name
        self.children = []

    def contains(self, entry):
        return any(child is entry or child.contains(entry) for child in self.children)

    def add(self, child):
        if child is self or child.contains(self):
            raise ValueError(f"cannot add {child.name} inside itself")
        self.children.append(child)


project = Directory("project")
src = Directory("src")
project.add(src)
try:
    src.add(project)
except ValueError as error:
    print(f"error: {error}")
```

```output for=cycle-check
error: cannot add project inside itself
```

An immutable tree has no cycles, because a node can hold only nodes that exist before it. The Java and Rust examples are immutable after construction. In Rust, a node owns its children, so a node cannot own itself (see [[ownership]]). A parent link in Rust needs `Rc` for the children and `Weak` for the parent, so that the two directions do not keep each other alive.[^7]

**Parent links.** Design Patterns describes explicit parent references as a help for moving up the tree and for deleting a component.[^1] They also add an invariant. Each child must appear in its parent's list, and in no other list. AWT keeps the invariant in `addImpl`. A component that already has a parent is removed from that parent before it joins the new one.[^5]

**Depth.** Each level of the tree is one level of the call stack. CPython limits the depth of the stack and raises `RecursionError` above the limit, which `sys.getrecursionlimit` returns.[^3][^4] A tree that comes from user data, such as nested JSON or a deep directory, can pass that limit. Walk such a tree with an explicit stack.

**Cost of an operation.** In the examples, `size` of a directory walks its subtree each time. `lines` calls `size` at each level, so a deep tree does the same sums many times. A composite can cache its total and clear the cache when a child changes. The cost is a link to the parent and code that keeps the cache correct.[^1]

## When not to use it

Do not use the pattern when the structure is flat. A list of items needs a loop, not a tree.

Do not use it when parts and wholes have different operations. If most methods mean something for a directory and nothing for a file, a common interface forces empty methods or errors on the leaf. Use two types, and a function that handles each.

Do not use it to model a graph. A composite is a tree: each child has one parent. Shared children or cycles need a graph with an explicit visited set.

Do not build a class hierarchy when the kinds are fixed and the operations change often. A sum type with functions is shorter in that case, as the table above shows.

## Variants and related patterns

**Recursive data in a table.** SQL stores a tree as rows that name their parents. A recursive common table expression walks it, starting from the root and joining each level to the next.[^10] The SQL example file computes the totals this way.

**Iterator.** An iterator can walk a composite and give each entry in turn, so the client needs no recursion. Design Patterns lists Iterator as related.[^1] See [[iterators]].

**Visitor.** A visitor puts an operation in one class with a method for each kind of component. In a language without sum types, it gives the functional side of the expression problem: a new operation is one class.[^1][^2]

**Decorator.** A decorator has one child, and a composite has many. Both forward calls to what they hold, and they are often used together, with a common component class.[^1] See [[decorator-pattern]].

**Builder.** A builder often makes a composite, and hides the calls that link the nodes.[^1] See [[builder-pattern]].

**Chain of Responsibility.** The parent links of a composite can form a chain. A request that a component does not handle passes up to its parent.[^1]

**Refactoring.** Kerievsky's refactoring Replace Implicit Tree with Composite starts from code that builds a tree as strings or nested primitive data, as in the problem above. It moves that code to component classes.[^12]

## Known uses

- OpenJDK AWT: `java.awt.Container` extends `java.awt.Component`, and holds a list of components, which can be containers.[^11] In [Container.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/java/awt/Container.java#L499-L507), `checkAddToSelf` walks the parent links and throws when the new child is the container or one of its ancestors. `addImpl` removes a child from its old parent before it adds it. `validateTree` lays out the container and then recurses into each child container.[^5]
- The DOM: an element is a node with child nodes, and text is a node with none. The insertion algorithm rejects a node that is an ancestor of the parent, so the document stays a tree.[^6]

## References
