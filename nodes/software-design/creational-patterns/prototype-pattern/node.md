---
tag: KQHJFJ
kind: pattern
slug: prototype-pattern
name: Prototype pattern
aliases: [Prototype, clone, prototypical instance]
area: software-design/creational-patterns
level: core
summary: >-
  A program makes new objects by copying a configured example object, the prototype, and not by calling a constructor of a known class. Each class decides how deep its own copy goes.
family: creational-design
polarity: positive
intent: "Make new objects by copying a prototypical instance, so that the code that makes them does not name their classes."
also_known_as: [clone]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with a framework for graphical editors as the motivating example. Prototype-based languages such as Self make objects by copying other objects and have no classes.
see_also: [abstract-factory-pattern, factory-method-pattern, composite-pattern, decorator-pattern, memento-pattern, flyweight-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 3, Prototype (Intent, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://docs.python.org/3.14/library/copy.html#shallow-vs-deep-copy
    title: The Python Standard Library, copy, Python 3.14
    locator: copy.copy, copy.deepcopy, the difference between shallow and deep copying, __copy__ and __deepcopy__
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Object.html#clone()
    title: Java SE 25 API, java.lang.Object
    locator: method clone, Implementation Requirements
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Cloneable.html
    title: Java SE 25 API, java.lang.Cloneable
    locator: interface description
  - book: isbn:9780134685991
    title: Joshua Bloch, Effective Java, third edition, Addison-Wesley, 2018
    locator: Chapter 3, Item 13, Override clone judiciously
  - url: https://html.spec.whatwg.org/multipage/structured-data.html#structured-cloning
    title: HTML Living Standard, WHATWG
    locator: 2.7.11 Structured cloning API
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Web_Workers_API/Structured_clone_algorithm#things_that_dont_work_with_structured_clone
    title: MDN Web Docs, The structured clone algorithm
    locator: Things that don't work with structured clone
  - url: https://tc39.es/ecma262/2025/#sec-object.create
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: 20.1.2.2 Object.create
  - url: https://tc39.es/ecma262/2025/#sec-ordinaryget
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: 10.1.8.1 OrdinaryGet
  - url: https://bibliography.selflanguage.org/_static/self-power.pdf
    title: David Ungar and Randall B. Smith, SELF, The Power of Simplicity, Lisp and Symbolic Computation 4(3), 1991 (a revision of the OOPSLA 1987 paper, doi:10.1145/38765.38828)
    locator: Abstract; section 2, Prototypes, Blending Classes and Instances (creation by copying)
  - paper: doi:10.1145/28697.28718
    title: Henry Lieberman, Using prototypical objects to implement shared behavior in object-oriented systems, OOPSLA 1986
    locator: title and abstract
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.object.memberwiseclone
    title: .NET API reference, Object.MemberwiseClone
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.icloneable
    title: .NET API reference, ICloneable
    locator: Notes to Implementers
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/record#nondestructive-mutation
    title: C# language reference, Records
    locator: Nondestructive mutation
  - url: https://pkg.go.dev/slices#Clone
    title: Go standard library, package slices
    locator: func Clone
  - url: https://doc.rust-lang.org/std/clone/trait.Clone.html
    title: Rust standard library, std::clone::Clone
    locator: trait description
  - url: https://doc.rust-lang.org/reference/items/traits.html#dyn-compatibility
    title: The Rust Reference, Traits
    locator: Dyn compatibility, rule items.traits.dyn-compatible.sized
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c67-a-polymorphic-class-should-suppress-public-copymove
    title: C++ Core Guidelines
    locator: C.67, A polymorphic class should suppress public copy/move
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c130-for-making-deep-copies-of-polymorphic-classes-prefer-a-virtual-clone-function-instead-of-public-copy-constructionassignment
    title: C++ Core Guidelines
    locator: C.130, For making deep copies of polymorphic classes prefer a virtual clone function
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.replace
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: dataclasses.replace
---

## Intent

Make new objects by copying a prototypical instance. The code that asks for a new object does not name its class. It holds an object of the right kind, and it asks that object for a copy.[^1]

## Problem

Design Patterns starts from a framework for graphical editors. An editor of music scores is built on it. The palette of the editor has a tool for each kind of note and for a staff. The framework owns the tool class, but it does not know the classes of notes and staves, because the music editor defines them later.[^1] One answer is a subclass of the tool for each kind of object, where each subclass calls one constructor. The number of tool classes then grows with the number of object classes.

The same need appears in other places. A level editor of a game has a goblin, an orc and a spike trap, each with its own health, damage and items. A game designer adds a kind of monster from data at run time, not from a class in the code. A plugin adds a class that the main program never saw. A document template holds a styled table that the user wants to insert many times.

The forces are these. The code that makes objects must not depend on their concrete classes. The program must add kinds at run time, often as other values of the same class. And the configuration of a kind can be costly to build, so the program wants to build it once and then copy it.

## Solution

Design Patterns names three participants.[^1]

- **Prototype** declares an operation that copies the object. It is often called `clone` or `copy`.
- **ConcretePrototype** implements the copy for its own class.
- **Client** makes a new object by asking a prototype to copy itself.

A **prototype manager** (a registry) often holds the prototypes by key. The client asks the registry for a key, and the registry returns a copy of the prototype for that key.[^1]

```text
 Client ---> Registry: spawn("goblin")
               prototypes: map of key to Prototype
               spawn(key) = prototypes[key].clone()
                                 |
                       Prototype (interface)
                         clone(), describe()
                          ^              ^
                          |              |
                  Monster                Trap
                  clone: deep copy       clone: shallow copy
                  of the items list      (all fields immutable)
```

The registry depends only on the interface. To add a kind, the program registers a configured object. Design Patterns describes this as a way to specify objects by varying values: a goblin and an orc can be two objects of one class, with different values, and no subclass.[^1]

### Shallow and deep copies

The pattern is only as correct as the copy. A **shallow copy** makes a new object and fills each field with the value of the old field. When the field holds a reference, the copy holds the same reference, so the two objects share the referred object. A **deep copy** makes a new object and copies the referred objects as well, recursively.[^2] Java's `Object.clone` and .NET's `MemberwiseClone` are shallow by definition.[^3][^12] So are a Go struct assignment, `slices.Clone`, `Object.assign` in JavaScript, and a C# `with` expression.[^15][^14]

This program clones the goblin with a shallow copy. The prototype and every clone share one list:

```python wrong id=shallow
import copy


class Monster:
    def __init__(self, name, items):
        self.name = name
        self.items = items

    def clone(self):
        return copy.copy(self)  # shallow: the clone shares self.items


goblin = Monster("goblin", ["dagger"])
first = goblin.clone()
first.items.append("torch")
second = goblin.clone()
print("first:", first.items)
print("second:", second.items)
print("prototype:", goblin.items)
```

```output for=shallow
first: ['dagger', 'torch']
second: ['dagger', 'torch']
prototype: ['dagger', 'torch']
```

The torch went into the prototype, so every later goblin gets it. The fix is `copy.deepcopy`, or a `clone` that copies the list, as the Python example does.

In Go the same bug can hide better. A struct copy copies the slice header, and the header points to the prototype's array. When that array has spare capacity, `append` writes into it and does not allocate. Each clone then writes its next item into the same slot:

```go wrong id=shared-slice
package main

import "fmt"

type Monster struct {
	Name  string
	Items []string
}

func (m *Monster) Clone() *Monster {
	copy := *m // the copy shares the array behind Items
	return &copy
}

func main() {
	items := make([]string, 1, 4)
	items[0] = "dagger"
	goblin := &Monster{Name: "goblin", Items: items}
	first := goblin.Clone()
	first.Items = append(first.Items, "torch")
	second := goblin.Clone()
	second.Items = append(second.Items, "shield")
	fmt.Println("first:", first.Items)
	fmt.Println("second:", second.Items)
	fmt.Println("prototype:", goblin.Items)
}
```

```output for=shared-slice
first: [dagger shield]
second: [dagger shield]
prototype: [dagger]
```

The prototype looks correct, because its length is still 1. The first goblin lost its torch to the shield of the second. The Go example calls `slices.Clone` in `Clone` to give each copy its own array.[^15]

A deep copy is not always right either. The Python documentation names two problems: a recursive structure can make the copy loop for ever, and a deep copy can copy data that was meant to be shared between copies.[^2] `copy.deepcopy` keeps a memo of the objects that it has already copied, which solves the first problem. A class solves the second with `__deepcopy__`, which copies only the fields that must not be shared. A monster can own its items and share its sprite image with all other goblins. So the right copy is a decision for each field, and the class that owns the fields makes it.

### Java's clone and its problems

Java builds a form of the pattern into `Object`. `Object.clone` makes an object of the same class and copies each field "as if by assignment", which is a shallow copy. It throws `CloneNotSupportedException` unless the class implements `Cloneable`.[^3] `Cloneable` has no methods. It only changes the behaviour of a `protected` method in `Object`, so a class that implements `Cloneable` still has no public `clone`. The API says that a call to `clone` on an object that implements `Cloneable` is not sure to succeed, even by reflection.[^4]

Bloch lists more problems. `clone` makes the object with no constructor call. A `clone` that needs a deep copy must assign the copied field after `super.clone()`, so the field cannot be `final`. And the checked exception forces a `try` block on code that cannot fail.[^5] The compiler shows the `final` problem:

```java fails expect=compile id=final-field
import java.util.ArrayList;
import java.util.List;

public class Main {
    static class Monster implements Cloneable {
        final List<String> items = new ArrayList<>();

        @Override
        public Monster clone() {
            try {
                Monster copy = (Monster) super.clone();
                copy.items = new ArrayList<>(items); // a final field cannot be assigned here
                return copy;
            } catch (CloneNotSupportedException error) {
                throw new AssertionError(error);
            }
        }
    }

    public static void main(String[] args) {
        System.out.println(new Monster().clone().items);
    }
}
```

Bloch recommends a copy constructor or a static copy factory instead: a constructor that takes an object of its own class.[^5] The Java example uses a copy constructor behind a `copy` method of an interface. A copy constructor alone does not give the pattern. The caller must name the class to call it, so a `copy` method that calls the constructor is still needed when the caller holds only the interface.

### Copy constructors and slicing in C++

In C++, every class has a copy constructor, and the compiler writes one that copies each member. Members such as `std::vector` and `std::string` copy their elements, so the default is often a deep copy of the data. A raw pointer member is copied as an address. The copy constructor is not virtual: a copy of a `Monster` through a reference to its base class makes a base object, and the derived part is lost. This is slicing:

```cpp wrong id=slicing
#include <iostream>
#include <string>
#include <vector>

struct Thing {
    virtual ~Thing() = default;
    virtual std::string describe() const { return "a thing"; }
};

struct Monster : Thing {
    std::string describe() const override { return "a goblin with a dagger"; }
};

int main() {
    Monster goblin;
    const Thing& prototype = goblin;
    std::vector<Thing> copies{prototype, prototype};  // copies only the Thing part
    for (const auto& copy : copies) {
        std::cout << copy.describe() << '\n';
    }
}
```

```output for=slicing
a thing
a thing
```

The C++ Core Guidelines give the fix: a polymorphic class makes its copy operations `protected` or deleted, and it gives a virtual `clone` that returns an owning pointer to a copy of the most derived type.[^18][^19] The C++ example does this.

### Prototypes in JavaScript

JavaScript has prototypes in the language. Each object has a link to a prototype object, and a property read that does not find an own property follows the link.[^9] `Object.create(p)` makes an object whose prototype is `p`.[^8] The design comes from prototype-based languages. In Self, there are no classes. Any object can be cloned, and a new object is made by copying a prototype.[^10] Lieberman described how prototypical objects can share behaviour, with delegation in place of class inheritance.[^11]

A JavaScript prototype is not a copy. The new object delegates to the prototype. It reads the inherited fields from it until it writes its own. A write of a field makes an own property, but a change inside an inherited array is not a write of the field:

```javascript wrong id=shared-array
const goblin = { name: "goblin", items: ["dagger"] };
const first = Object.create(goblin);
first.items.push("torch"); // reads goblin.items, then changes that array
const second = Object.create(goblin);
console.log("second:", second.items.join(", "));
console.log("prototype:", goblin.items.join(", "));
console.log("first has its own items:", Object.hasOwn(first, "items"));
```

```output for=shared-array
second: dagger, torch
prototype: dagger, torch
first has its own items: false
```

`structuredClone` makes a deep copy, and it handles cycles. It throws a `DataCloneError` for a value that it cannot serialize, such as a function.[^6][^7] It does not walk or copy the prototype chain, and it does not copy private class fields.[^7] So a clone of a class instance is a plain object with no methods:

```javascript fails expect=exception:TypeError id=structured-clone
class Monster {
  constructor(name, items) {
    this.name = name;
    this.items = items;
  }
  describe() {
    return `${this.name}: ${this.items.join(", ")}`;
  }
}

const copy = structuredClone(new Monster("goblin", ["dagger"]));
console.log(Object.getPrototypeOf(copy) === Object.prototype);
console.log(copy.describe());
```

Use `structuredClone` for plain data, such as a configuration or a message. For objects with behaviour, write a `clone` method, as the JavaScript and TypeScript examples do.

### Clone and trait objects in Rust

Rust's `Clone` is explicit, and `#[derive(Clone)]` clones each field. For owned data, such as `Vec<String>`, this is a deep copy. A field of type `Rc<T>` or `Arc<T>` clones the pointer, so the clone shares the value.[^16] A registry of prototypes needs trait objects, and `Clone` cannot be a supertrait of a trait used as `dyn`. `Clone` requires `Sized`, and a trait with `Sized` as a supertrait is not dyn compatible {rule:items.traits.dyn-compatible.sized}.[^17] The compiler reports error E0038.

```rust fails expect=compile id=dyn-clone
trait Prototype: Clone {
    fn describe(&self) -> String;
}

#[derive(Clone)]
struct Trap {
    damage: u32,
}

impl Prototype for Trap {
    fn describe(&self) -> String {
        format!("trap: damage {}", self.damage)
    }
}

fn main() {
    let prototype: Box<dyn Prototype> = Box::new(Trap { damage: 5 });
    println!("{}", prototype.describe());
}
```

The Rust example adds a method `clone_box` that returns `Box<dyn Prototype>`. Each type implements it with its own `clone`.

## Example

Each example file builds the same registry. It has a goblin prototype, a monster with health 30 and a dagger, and a spike trap prototype with damage 5. The program spawns two goblins and a trap, gives the first goblin a torch, and prints all three and the prototype. The prototype must still have only the dagger.

| Language | Form |
|---|---|
| Python | a `clone` method that calls `copy.deepcopy` or `copy.copy` |
| JavaScript | `Object.create` on the prototype, with a copy of each array |
| TypeScript | an interface with `clone(): this` |
| Java | an interface with `copy`, implemented by a copy constructor |
| C# | `MemberwiseClone` and a copy of the list; a record with `with` |
| Go | a struct copy and `slices.Clone` |
| Rust | `#[derive(Clone)]` and a `clone_box` method for trait objects |
| C++ | a virtual `clone` that returns `std::unique_ptr` and uses the copy constructor |

## Consequences

Design Patterns lists these benefits.[^1] The client does not know the concrete classes, as with Abstract Factory and Builder. A program can add and remove kinds at run time by registering prototypes. It can define kinds by changing values, and by changing structure when the prototype is a composite. It needs fewer subclasses than Factory Method, which needs a creator class for each product class. And a program can configure itself with classes that it loads at run time.

The main liability is that each class must implement its copy. Design Patterns says that this can be hard for a class that already exists, and for a class whose internals hold objects that do not support copying or that have circular references.[^1] Each field added to the class is a decision: share it or copy it. A field added later with the wrong decision is a bug that no type checker finds.

A clone also skips the constructor in some languages. `Object.clone`, `MemberwiseClone` and a struct copy do not run the code that checks the invariants of the object or registers it somewhere. A class that counts its instances, opens a file or subscribes to an event in its constructor must repeat that work in its copy. Design Patterns notes that a clone often needs a separate operation to initialize its state after the copy.[^1]

## When not to use it

Do not write a clone method for objects whose fields are all immutable. The object itself can be the prototype and the copy. The Java and C# examples return the same `Trap` or a `with` copy for this reason. A Python frozen dataclass with `dataclasses.replace` makes a variant with some fields changed.[^20]

Do not use the pattern when the set of classes is fixed and known where the objects are made. A constructor call, a factory function, or a map from a key to a constructor function is shorter, and it runs the constructor's checks. In languages with first-class functions and classes, a registry of factory functions does what a registry of prototypes does. It needs no copy code. See [[factory-method-pattern]].

Do not use `structuredClone`, `copy.deepcopy` or serialization to clone objects with behaviour or with identity, such as a connection or a lock. The copy drops methods, fails, or copies a resource that must not have two owners.[^2][^7]

Do not implement `ICloneable` in a public .NET API. Microsoft recommends against it, because the interface does not say whether `Clone` is deep or shallow.[^13]

## Variants and related patterns

**Abstract Factory.** Design Patterns calls Prototype and Abstract Factory competitors in some ways, and says that they can work together: an abstract factory can store a set of prototypes and return copies of them.[^1] See [[abstract-factory-pattern]].

**Factory Method.** A factory method needs a subclass of the creator for each product. A prototype needs no subclass, but it needs a copy operation on each product.[^1] See [[factory-method-pattern]].

**Composite and Decorator.** Design Patterns says that designs that use Composite and Decorator often benefit from Prototype.[^1] A composite can be a prototype, so a copy of it copies a whole tree. This needs a deep copy. See [[composite-pattern]] and [[decorator-pattern]].

**Memento.** A memento is also a copy of the state of an object, but its purpose is different. A prototype copy is a new object that lives on. A memento is a saved state that its originator restores later. See [[memento-pattern]].

**Flyweight.** Flyweight is the opposite choice: it shares one object where Prototype makes many copies. A prototype whose fields are all immutable can be shared as a flyweight. See [[flyweight-pattern]].

**Copy with changes.** Many languages make a copy with some fields changed in one expression: a C# `with` expression on a record, `dataclasses.replace` in Python, the spread syntax `{...goblin, health: 50}` in JavaScript, and the struct update syntax `Monster { health: 50, ..goblin.clone() }` in Rust. The C# `with` expression is shallow: a member of a reference type is shared between the copy and the original.[^14] These forms cover the common use of a prototype for value objects.

## Known uses

- JavaScript: every object has a prototype, and `Object.create` makes an object with a given prototype.[^8][^9]
- Java: arrays implement `Cloneable`, and `clone` on an array of type `T[]` returns a `T[]`.[^3]
- Self: objects are made by copying prototypes, and the language has no classes.[^10]

## References
