---
tag: 7BMZQ3
kind: pattern
slug: type-object
name: Type Object
aliases: [type object pattern, TypeObject]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  A type object is an instance that represents a kind of object. Each typed object refers to its type object, so new kinds are new data, not new classes.
family: behavioral-design
polarity: positive
intent: "Allow the flexible creation of new classes by creating a single class, each instance of which represents a different type of object."
also_known_as: []
history:
  people: [Ralph Johnson, Bobby Woolf]
  note: Johnson and Woolf described Type Object in Pattern Languages of Program Design 3, the book of the PLoP '96 and EuroPLoP '96 conferences. Robert Nystrom later described it for games in Game Programming Patterns.
see_also: [class-inheritance, flyweight-pattern, state-pattern, prototype-pattern, interpreter-pattern, null-object-pattern]
sources:
  - url: https://gameprogrammingpatterns.com/type-object.html
    title: Robert Nystrom, Game Programming Patterns, Type Object
    locator: Intent; Motivation; The Pattern; When to Use It; Keep in Mind; Design Decisions; See Also
  - book: isbn:9780201310115
    title: Robert Martin, Dirk Riehle and Frank Buschmann (editors), Pattern Languages of Program Design 3, Addison-Wesley
    locator: Chapter 4, Type Object, Ralph Johnson and Bobby Woolf
  - url: https://www.informit.com/store/pattern-languages-of-program-design-3-9780201310115
    title: Pattern Languages of Program Design 3, InformIT
    locator: Description; Table of Contents, I. General Purpose Design Patterns
  - url: https://github.com/python/cpython/blob/v3.14.0/Include/object.h#L104-L145
    title: CPython v3.14.0 source, Include/object.h
    locator: struct _object and its comment, lines 104 to 145
  - url: https://github.com/python/cpython/blob/v3.14.0/Include/cpython/object.h#L148-L161
    title: CPython v3.14.0 source, Include/cpython/object.h
    locator: struct _typeobject, lines 148 to 161
---

## Intent

Type Object allows the flexible creation of new "classes" by creating a single class, each instance of which represents a different type of object.[^1]

## Problem

A program often has many kinds of a thing, and each kind has its own data. In Nystrom's example, a game has monsters of different breeds, such as dragons and trolls. The breed determines the starting health of a monster and its attack string.[^1]

The obvious design makes each breed a subclass of `Monster`. Then each new breed needs new code, and the kinds are fixed in the type system of the language. This is too rigid in these cases:[^1]

- You do not know which types you will need up front. For example, a game can download content with new breeds of monsters.
- You want to modify or add types without a change to the code or a new compilation.

## Solution

Define a type object class and a typed object class. Each instance of the type object class represents a different logical type. Each typed object stores a reference to the type object that describes its type.[^1]

```text
   Breed (type object)              Monster (typed object)
   name, health, attack   <-------  breed
   new_monster()                    health   (its own data)
       ^
       |  one instance for each kind: goblin, dragon, wyvern, ...
```

Data that is specific to one instance is in the typed object. Data or behaviour that all instances of the same conceptual type share is in the type object. Objects that refer to the same type object function as if they were the same type.[^1] Johnson and Woolf called the pattern Type Object.[^2][^3]

## Example

The example files show one program in each language. A `Breed` has a name, a starting health and an attack. A list of data defines three breeds, and no breed has its own class. `new_monster` is a constructor function on the breed, and each monster keeps a reference to its breed and its own health. The program prints:

```text
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

## Consequences

The benefits:[^1]

- Data and behaviour are shared across a set of similar objects, as with subclassing, but without a fixed set of hard-coded subclasses.
- New types can come from data, for example from a file, so designers can add them without a programmer.

The costs:[^1]

- **Manual tracking.** The compiler keeps the classes of a program for you. With type objects, you must make sure that all of them are created and kept in memory as long as the typed objects need them, and that each new typed object refers to a valid type object.
- **Behaviour is harder to define.** A subclass can override a method and do anything. A type object usually holds data. For behaviour, it can hold a pointer to one of a fixed set of functions, or the behaviour can be defined in data with the Interpreter or Bytecode patterns.

Data in the wrong place is a common error. Here the damage to one goblin is stored in the breed, so a new goblin starts with less health:

```python wrong id=shared-health
class Breed:
    def __init__(self, name, health):
        self.name, self.health = name, health

    def new_monster(self):
        return Monster(self)


class Monster:
    def __init__(self, breed):
        self.breed = breed

    def take_damage(self, amount):
        self.breed.health -= amount  # changes every goblin


goblin = Breed("goblin", 20)
first = goblin.new_monster()
first.take_damage(5)
second = goblin.new_monster()
print("second goblin:", second.breed.health)  # meant to be 20
```

```output for=shared-health
second goblin: 15
```

## When not to use it

- The set of kinds is small and fixed, and each kind needs very different behaviour. Subclasses can override methods freely.[^1]
- The program depends on the bookkeeping that the compiler does for classes.[^1]

## Variants and related patterns

- **Constructor on the type object.** The type object can have a factory method that creates its typed objects. The type object then controls memory allocation, for example from an object pool.[^1]
- **Change of type.** An object can change its type object over time. For example, a dead monster can become a zombie by a change of its breed.[^1]
- **Inheritance between type objects.** With single inheritance, a type object has a parent. To get a value, the program might need to walk up the inheritance chain to the type that ultimately decides the value.[^1]

```python run id=parent-breed
class Breed:
    def __init__(self, name, parent=None, health=None, attack=None):
        self.name, self.parent = name, parent
        self._health, self._attack = health, attack

    def get(self, field):
        breed = self
        while getattr(breed, field) is None:  # walk up the chain
            breed = breed.parent
        return getattr(breed, field)

    @property
    def health(self):
        return self.get("_health")

    @property
    def attack(self):
        return self.get("_attack")


troll = Breed("troll", health=25, attack="a club")
archer = Breed("troll archer", parent=troll, attack="arrows")
print(f"{archer.name}: {archer.health} health, attacks with {archer.attack}")
```

```output for=parent-breed
troll archer: 25 health, attacks with arrows
```

- **Flyweight.** Both let instances share data. A flyweight saves memory, and its shared data need not be a type. A type object is for organization and flexibility.[^1]
- **State.** Both delegate part of an object to another object. A type object is usually what the object is; a state is what the object is now.[^1]
- **Prototype.** It solves the same high-level problem of sharing data and behaviour between objects in a different way.[^1]

## Known uses

- In CPython, every pointer to a Python object can be cast to `PyObject*`, which the source calls "inheritance built by hand". Each object has a field `ob_type`, a pointer to a `PyTypeObject`.[^4] The type object `struct _typeobject` holds the type name, the sizes for allocation, and function pointers such as `tp_repr`.[^5]
- Nystrom says that C++ classes are the Type Object pattern applied to C: each instance has a pointer to the vtable of its class, as a monster has a reference to its breed.[^1]

## References
