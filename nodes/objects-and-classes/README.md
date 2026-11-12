This area is about objects and the ways that languages let objects share behaviour. It covers classes and prototypes, inheritance and composition, interfaces, and the different kinds of dispatch.

## What it holds

- **Objects.** What an object is: state, behaviour and identity, and the idea of sending a message to an object. The sub-area also compares the object models of the main languages.
- **Classes.** Classes as templates for objects: fields, methods, static members, nested classes and the ways to add methods to a class that exists.
- **Prototypes.** Objects that inherit directly from other objects, as in Self, JavaScript and Lua.
- **Encapsulation.** The boundary between the inside and the outside of an object, and the language tools that enforce it.
- **Inheritance.** Single and multiple inheritance, abstract classes, mixins and traits, and the rules that decide which method runs.
- **Interfaces and protocols.** Contracts that a type promises to meet, with an explicit declaration or by its shape.
- **Polymorphism and dispatch.** How a call picks the code to run: overriding, overloading, virtual methods and dispatch on more than one argument.
- **Composition over inheritance.** Building objects from other objects instead of from a base class, and handing work to them.
- **Constructors and initialization.** How an object gets a valid first state: constructors, chaining, the order of initialization and the alternatives.
- **Object lifecycle.** The life of an object from creation to destruction, from the view of the object model. The memory side is in memory-and-resources.
- **Standard object methods.** The methods that most object types must give: equality, hashing, ordering, a string form, copying and operators. The data ideas behind them are in data-and-values; these pages show how a type implements them.
- **Object-oriented design problems.** Problems that the object model itself causes. General design smells and principles are in software-design; these pages need classes, inheritance or dispatch to exist.
- **The expression problem.** Adding new types and new operations without changing old code, and the solutions in object and functional languages.

## What it does not hold

- Design patterns, such as visitor or strategy. They are in [Software design](/software-design/).
- Memory management of objects, such as garbage collection. It is in [Memory and resources](/memory-and-resources/).
- Subtyping and variance as type system ideas. They are in [Types](/types/).
- The object-oriented style as a whole. It is in [Paradigms](/paradigms/).
