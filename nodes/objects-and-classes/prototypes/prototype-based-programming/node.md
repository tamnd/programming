---
tag: E0G8GA
kind: concept
slug: prototype-based-programming
name: Prototype-based programming
aliases: [prototypes, prototype chain, prototypal inheritance, delegation, classless objects, object-based programming]
area: objects-and-classes/prototypes
level: core
summary: >-
  In prototype-based programming, objects come from other objects: a new object copies a prototype or delegates to it, and a property lookup follows the chain of parents.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1987
  people: [Henry Lieberman, David Ungar, Randall B. Smith, Brendan Eich]
  note: Lieberman described delegation to prototypes in 1986, Self made it a whole language in 1987, and JavaScript brought it to the web in 1995.
see_also: [class-inheritance, dynamic-dispatch, object-oriented-programming, interfaces, expression-problem]
same_as:
  wikidata: Q180868
  wikipedia: Prototype-based_programming
sources:
- url: https://bibliography.selflanguage.org/_static/self-power.pdf
  title: 'David Ungar and Randall B. Smith, SELF: The Power of Simplicity, Lisp and Symbolic Computation 4(3), 1991'
  locator: Abstract; Section 2
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Inheritance_and_the_prototype_chain
  title: Inheritance and the prototype chain, MDN Web Docs
  locator: Inheriting properties
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create
  title: Object.create(), MDN Web Docs
  locator: Description
---

## Idea

In prototype-based programming, objects come from other objects, not from classes. A program makes a new object by copying an existing one, the prototype, or by creating an object that delegates to it: when the new object has no property of a given name, the lookup continues in its parent. Behavior shared by many objects lives in one object that they all delegate to, so a change to that object reaches all of them at once. Self introduced the model in the 1980s, and JavaScript, Lua and Io use it.

```python run id=idea
class Obj:
    """An object with its own slots and one parent to delegate to."""

    def __init__(self, parent=None, **slots):
        self.parent, self.slots = parent, dict(slots)

    def get(self, name):
        o = self
        while o is not None:  # walk the chain until a slot matches
            if name in o.slots:
                return o.slots[name]
            o = o.parent
        raise AttributeError(name)

    def set(self, name, value):  # always writes an own slot
        self.slots[name] = value

    def send(self, name, *args):
        return self.get(name)(self, *args)  # the method runs with the receiver as self

    def clone(self):
        return Obj(self.parent, **self.slots)


point_traits = Obj(  # behavior shared by all points
    describe=lambda self: f"({self.get('x')}, {self.get('y')})",
    moved=lambda self, dx, dy: Obj(self, x=self.get("x") + dx, y=self.get("y") + dy),
)
origin = Obj(point_traits, x=0, y=0)  # the prototype point

p = origin.clone()  # a new point is a copy of the prototype
p.set("x", 3)
q = p.send("moved", 1, 1)  # q delegates to p
print(origin.send("describe"), p.send("describe"), q.send("describe"))

point_traits.set("describe", lambda self: f"<{self.get('x')} {self.get('y')}>")
print(origin.send("describe"), q.send("describe"))  # one change, every point sees it

q.set("describe", lambda self: "special")  # an own slot shadows the parent's
print(q.send("describe"), p.send("describe"))
```

```output for=idea
(0, 0) (3, 0) (4, 1)
<0 0> <4 1>
special <3 0>
```

The class `Obj` is the whole object model. An object is a dictionary of slots and a parent. A read walks from the object to its parent and on up the chain, a write always goes to the object itself, and a method is a slot that holds a function, called with the receiver as `self`. `point_traits` holds the methods of all points, `origin` is a concrete point to copy, and changing `describe` in `point_traits` changes it for every point. The last line gives one object its own `describe`, which hides the shared one for that object only.

## How it works

**Objects without classes.** A class-based language has two kinds of entity: classes, which describe the structure and behavior of objects, and instances, which hold the data. A prototype-based language has only objects. David Ungar and Randall Smith, the designers of Self, put the difference this way: "In a class-based language, an object would be created by instantiating a plan in its class. In a prototype-based language like SELF, an object would be created by cloning (copying) a prototype. In SELF, any object can be cloned."[^1] An object can also be made from nothing, as a literal with its own properties.

**Delegation.** Each object can have a link to a parent object, and the lookup of a property follows these links. MDN describes the chain in JavaScript: "Each object has an internal link to another object called its prototype. That prototype object has a prototype of its own, and so on until an object is reached with null as its prototype."[^2] A read looks first in the object, then in its prototype, and so on, "until either a property with a matching name is found or the end of the prototype chain is reached."[^2] Self uses the same rule: "If an object receives a message and it has no matching slot, the search continues via a parent pointer. This is how SELF implements inheritance."[^1] A method found in a parent runs with the original receiver as `self` or `this`, so shared code works on the data of each object.

**Shadowing and writes.** A write creates or changes a property of the object itself, never of its parent. An own property with the same name as one in the chain hides the parent's property for that object, which is how an object overrides shared behavior. Because reads go through the chain and writes do not, a mutable value that lives in the parent, such as an array, is shared by every object that reads it. The wrong blocks in the Python and JavaScript files show this.

**Traits objects and concrete prototypes.** Programs in Self and JavaScript divide the work between two kinds of object. A traits object, the `prototype` property of a JavaScript constructor, holds the methods that all objects of a kind share. A concrete prototype holds default data and is the object to copy or to delegate to. Ungar and Smith claimed that "Prototypes combine inheritance and instantiation to provide a framework that is simpler and more flexible than most object-oriented languages."[^1]

**Change at run time.** A parent link is an ordinary reference, so a program can add a method to a shared object, replace it, or give one object a different parent while the program runs. JavaScript exposes the link through `Object.create`, which "creates a new object, using an existing object as the prototype of the newly created object", and through `Object.getPrototypeOf` and `Object.setPrototypeOf`.[^3] This flexibility is what makes a prototype language easy to extend and hard to optimize.

**Classes on top of prototypes.** A class-like layer is a short step from prototypes: a constructor function makes objects that delegate to one traits object. JavaScript's `class` syntax does exactly this, and MDN notes that "classes do not bring a new inheritance pattern."[^2] The reverse also holds: Python and Smalltalk find methods by searching an object, then its class, then the superclasses, which is delegation along a chain that the language builds from classes.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | attribute lookup through instance, class and bases; classes change at run time |
| JavaScript | native | prototype chain; `Object.create`; classes built on prototypes |
| TypeScript | native | as JavaScript |
| Java | absent | maps of slots by hand; the prototype pattern with copies |
| Kotlin | absent | classes; class delegation with `by` |
| C# | absent | dictionaries of slots by hand; `ExpandoObject` |
| Go | absent | maps of slots by hand; struct embedding |
| Rust | absent | `Rc` parents and `RefCell` slots by hand |
| C | absent | structs with a parent pointer and a slot table |
| C++ | absent | maps of slots and a `shared_ptr` parent |
| Bash | n/a | no objects |
| PowerShell | idiom | `Add-Member` on single objects |
| SQL | n/a | no objects |
| Regex | n/a | no objects |
| HTML | n/a | no objects |
| CSS | n/a | no objects |

JavaScript and TypeScript are the only core languages of this site with prototypes in the language. Every other language with objects is class-based, and the example files build a small prototype system by hand: a table of slots, a parent reference and a lookup that walks the chain. The files show how much each language must work around its static types: Java, C#, Go, Rust and C++ store methods as function values and cast or match on every read.

Python stands between the two models. It has classes, but a class is an object that can change at run time, and attribute lookup searches the instance, then the class and its bases, much as a prototype language searches the chain. Outside the core set, Lua builds objects from tables with a metatable whose `__index` field names the table to search next, and Self, Io and NewtonScript are prototype languages from the ground up.

## Variations and related ideas

- **Delegation and concatenation.** Delegation shares the parent, so later changes to the parent show in every child. Concatenation copies all properties into the new object, so each object is complete and independent. Antero Taivalsaari's Kevo used concatenation.
- **The prototype design pattern.** In class-based languages, a factory keeps configured objects and makes new ones by copying them. The pattern keeps the copying of prototype languages without the delegation.
- **Class inheritance.** Classes put structure and behavior in a separate kind of entity, and objects find methods through their class. See [class inheritance](/class-inheritance/).
- **Dynamic dispatch.** A prototype chain is one way to decide at run time which code a call runs. See [dynamic dispatch](/dynamic-dispatch/).
- **Mixins and traits.** Several languages compose behavior from small units of methods. JavaScript programs copy methods into a prototype with `Object.assign` to do this.
- **Metatables.** Lua attaches a table of hooks to a table, and the `__index` hook gives delegation to another table or to a function.
- **Hidden classes.** Fast JavaScript engines group objects with the same layout under one internal map, a technique from the Self compiler, so that property access can be as fast as a field read in a class-based language.

## Pitfalls

- **Mutable data on the prototype.** An array or object stored on a prototype is shared by every object that delegates to it. Changing it through one object changes it for all. Set mutable state on each object in its constructor.
- **Shallow clones.** Copying a prototype copies its references, so the clone shares the nested lists and maps of the original. The wrong blocks of the Python, C# and Go files show this. Copy each mutable part, or use a deep copy.
- **Changing built-in prototypes.** Adding a method to `Array.prototype` or `Object.prototype` in JavaScript changes every array or object in the program, including those of libraries, and can clash with later versions of the language. Use functions or subclasses instead.
- **Inherited properties in loops.** A `for...in` loop in JavaScript visits inherited enumerable properties as well as own ones. Use `Object.keys` or `Object.hasOwn` to see only the object's own properties.
- **Changing the parent of a live object.** `Object.setPrototypeOf` on an object that is in use makes engines discard their optimizations for it, and code that read the old chain sees different behavior. Set the prototype when the object is created.
- **Long chains.** Each level of a chain is one more lookup for a property that is far up or missing. Keep chains short.

## Cost

A property read in a naive implementation searches each object of the chain in turn, so its cost grows with the depth of the chain and the number of properties at each level. Modern engines remove most of this cost. The Self compiler introduced maps, now called hidden classes or shapes, which record the layout of objects created the same way, and inline caches, which remember at each call site where the property was found the last time. With these, a property read in V8 or SpiderMonkey costs a check of the shape and a load at a fixed offset. Changing an object's shape or prototype in a hot path makes the caches miss and costs a slow lookup. Each object stores one reference to its parent, and shared methods cost no memory per object.

## History

Henry Lieberman described prototypes with delegation in 1986, in a paper on implementing shared behavior with prototypical objects, which grew from his work on actor languages at MIT. David Ungar and Randall Smith designed Self at Xerox PARC and published it in 1987, as a language with "prototypes, slots, and behavior" and no classes.[^1] The Self project at Stanford and Sun Microsystems built the techniques that made such a dynamic language fast, among them maps, inline caches and adaptive compilation, and Lars Bak and other members of the team later used them in the HotSpot Java virtual machine and the V8 JavaScript engine.

Brendan Eich created JavaScript at Netscape in 1995 with prototypes taken from Self and syntax that looked like Java. NewtonScript, for the Apple Newton in 1993, also used prototypes, Steve Dekorte's Io followed in 2002, and Lua 5.0 added metatables in 2003, which programs use for delegation between tables. ECMAScript 5 added `Object.create` in 2009, which gave programs the prototype link without constructor functions, and ECMAScript 2015 added `class` syntax as a layer on top of the same chain.[^2]

## References
