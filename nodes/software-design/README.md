This area is about the shape of a program above the level of a single function. It covers the principles that guide design, the catalogue of design patterns, architecture styles and the design of APIs.

## What it holds

- **Design fundamentals.** The ideas that every later page of the area uses: what design is, why complexity is the enemy, and how coupling and cohesion measure a design.
- **Design principles.** The general rules that guide a design. Each page states the rule, what it asks the programmer to do, and its limits.
- **Object-oriented design principles.** The principles that are stated for classes and packages: SOLID, GRASP and the package principles of Robert C. Martin.
- **Creational patterns.** Patterns that make objects. The five GoF creational patterns first, then the other named creational patterns.
- **Structural patterns.** Patterns that compose classes and objects into larger structures. The seven GoF structural patterns first.
- **Behavioral patterns.** Patterns that assign responsibilities and share work between objects. The eleven GoF behavioral patterns first, then the other named behavioral patterns.
- **Software architecture.** What architecture is and how architects describe and judge it: quality attributes, views, decisions and evaluation.
- **Architecture styles and patterns.** The named architecture styles and architectural patterns, with POSA1's architectural patterns first.
- **Presentation patterns.** Patterns that separate the user interface from the rest of the program, for desktop, mobile and web.
- **Enterprise application patterns.** The PoEAA patterns for domain logic, distribution, offline concurrency, session state and the base patterns.
- **Data access patterns.** The PoEAA data source and object-relational patterns, and the related data access patterns. ORMs as libraries are in data-and-persistence/orms.
- **Domain-driven design.** Evans's method and patterns: the model-driven building blocks, supple design, strategic design with bounded contexts, distillation and large-scale structure.
- **Concurrency patterns.** The named patterns for concurrent and networked programs: the POSA2 event handling, synchronization and concurrency patterns, the patterns of *Java Concurrency in Practice*, and the other common ones. The mechanisms they use are in concurrency-and-parallelism.
- **Resilience patterns.** Patterns that keep a program working when the things it depends on fail or slow down. The detail of distributed failure (consensus, idempotency of messages) is in distributed-and-cloud.
- **Resource and service patterns.** The POSA2 service access and configuration patterns, the POSA3 resource management patterns, and the POSA1 idioms and POSA4 patterns about resources and interfaces.
- **Distribution patterns.** The patterns for objects that talk across process boundaries, from POSA1, POSA4 and the remoting patterns. Messaging systems and cloud patterns are in distributed-and-cloud.
- **API design.** How to design an interface that other programmers use: a library API, a module interface or a service contract. HTTP styles are in web-development/apis.
- **Modules and components.** How to split a system into modules and components with clear boundaries, and how to keep the dependency graph healthy. The module systems of languages are in functions-and-abstraction/modules-and-namespaces.
- **Design for change.** Design that keeps the cost of a future change small, and the patterns that move a running system from one design to another.
- **Design anti-patterns.** Named designs that look right and do harm, at the level of classes, modules and architecture. Code-level smells are in construction-practice/code-smells.
- **Design modelling and notation.** The notations that programmers use to sketch and record designs, from UML to CRC cards.

## What it does not hold

- The language features that patterns use, such as interfaces and closures. They are in [Objects and classes](/objects-and-classes/) and [Functions and abstraction](/functions-and-abstraction/).
- Microservices and distributed architecture. They are in [Distributed and cloud programming](/distributed-and-cloud/).
- Code-level practice, such as naming and refactoring. It is in [Construction practice](/construction-practice/).
- HTTP API styles, such as REST and GraphQL. They are in [Web development](/web-development/).
