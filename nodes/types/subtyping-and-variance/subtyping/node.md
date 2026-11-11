---
tag: MRV58N
kind: concept
slug: subtyping
name: Subtyping
aliases: [subtype polymorphism, inclusion polymorphism, Liskov substitution principle, variance]
area: types/subtyping-and-variance
level: core
summary: >-
  A type S is a subtype of a type T when a value of S can be used wherever a value of T is expected. Code written for T then works for every subtype, and the compiler or the runtime checks the uses.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1987
  people: [Barbara Liskov]
  note: In a keynote at OOPSLA '87, Liskov defined a subtype by substitution. A program written in terms of T must behave the same when an object of S takes the place of an object of T.
see_also: [parametric-polymorphism, static-and-dynamic-typing, immutability, null-references, higher-order-functions]
same_as:
  wikidata: Q2713292
  wikipedia: Subtyping
sources:
  - paper: doi:10.1145/62138.62141
    title: Barbara Liskov, Data abstraction and hierarchy, keynote address, OOPSLA '87 Addendum to the Proceedings, October 1987
    locator: 3.3 Type Hierarchy (the substitution property; subtype and supertype are semantic, subclass and superclass are linguistic)
  - paper: doi:10.1145/197320.197383
    title: Barbara H. Liskov and Jeannette M. Wing, A behavioral notion of subtyping, ACM Transactions on Programming Languages and Systems 16(6), 1994
    locator: 1 Introduction (the Subtype Requirement; the signature rules alone do not make a stack a subtype of a queue)
  - paper: doi:10.1016/0890-5401(88)90007-7
    title: Luca Cardelli, A semantics of multiple inheritance, Information and Computation 76(2-3), 1988
    locator: 3 Records (Rule2 for record types, Rule3 subsumption, Rule6 for function types)
  - paper: doi:10.1145/62138.62141
    title: Barbara Liskov, Data abstraction and hierarchy, keynote address, OOPSLA '87 Addendum to the Proceedings, October 1987
    locator: 3.1 Inheritance and 3.2 Implementation Hierarchy (subclasses that violate encapsulation; inheritance used only to implement a type)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.10.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.10.2 Subtyping among Class and Interface Types
  - paper: doi:10.1145/62138.62141
    title: Barbara Liskov, Data abstraction and hierarchy, keynote address, OOPSLA '87 Addendum to the Proceedings, October 1987
    locator: 5.1 Polymorphism (the grouping approach, in which a module accepts any type with the needed operations)
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html#starting-out
    title: TypeScript Handbook, Type Compatibility
    locator: the introduction (structural subtyping) and Starting out
  - url: https://go.dev/ref/spec#Interface_types
    title: The Go Programming Language Specification
    locator: Interface types and Implementing an interface
  - url: https://go.dev/ref/spec#Assignability
    title: The Go Programming Language Specification
    locator: Assignability (a value x is assignable to an interface type T if x implements T)
  - url: https://peps.python.org/pep-0544/#nominal-vs-structural-subtyping
    title: "PEP 544: Protocols: Structural subtyping (static duck typing)"
    locator: Abstract and Nominal vs structural subtyping
  - url: https://docs.python.org/3.14/library/typing.html#typing.Protocol
    title: The Python Standard Library, typing, Python 3.14
    locator: typing.Protocol (protocol classes with runtime_checkable check only the presence of the attributes) and the note on runtime_checkable (isinstance checks can be slow)
  - url: https://docs.python.org/3.14/library/abc.html#abc.ABCMeta.register
    title: The Python Standard Library, abc, Python 3.14
    locator: ABCMeta.register (virtual subclasses)
  - url: https://tc39.es/ecma262/2026/#sec-ordinaryhasinstance
    title: ECMAScript 2026 Language Specification
    locator: 7.3.21 OrdinaryHasInstance (the walk along the prototype chain)
  - url: https://timsong-cpp.github.io/cppwp/n4950/conv.ptr
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[conv.ptr], paragraph 3"
  - url: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#class-types
    title: Itanium C++ ABI
    locator: 2.4 Non-POD Class Types (the primary base is allocated first; Allocation of Members Other Than Virtual Bases)
  - url: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-general
    title: Itanium C++ ABI
    locator: 2.5.1 General (a virtual table is used to dispatch virtual functions)
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification
    locator: Variables (the static type and the dynamic type of a variable of interface type)
  - url: https://research.swtch.com/interfaces
    title: Russ Cox, Go Data Structures, Interfaces, 1 December 2009
    locator: Interface Values (a two-word pair of an itable pointer and a data pointer)
  - url: https://port70.net/~nsz/c/c11/n1570.html#6.7.2.1p15
    title: ISO/IEC 9899:201x, Committee Draft N1570 (C11)
    locator: 6.7.2.1 Structure and union specifiers, paragraph 15
  - paper: doi:10.1145/197320.197383
    title: Barbara H. Liskov and Jeannette M. Wing, A behavioral notion of subtyping, ACM Transactions on Programming Languages and Systems 16(6), 1994
    locator: 5.2, Fig. 4 (the definition of the subtype relation, with the signature rules, the methods rule and the properties rule)
  - url: https://timsong-cpp.github.io/cppwp/n4950/class.virtual
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[class.virtual], paragraph 8 (covariant return types)"
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 8.4.5 Method Result (return-type-substitutable)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.10.3
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.10.3 Subtyping among Array Types
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-10.html#jls-10.5
    title: The Java Language Specification, Java SE 25 Edition
    locator: 10.5 Array Store Exception
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-6.html#jvms-6.5.aastore
    title: The Java Virtual Machine Specification, Java SE 25 Edition
    locator: 6.5 aastore (Run-time Exceptions)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/arrays#176-array-covariance
    title: C# language specification, Arrays
    locator: 17.6 Array covariance
  - url: https://kotlinlang.org/docs/generics.html#type-projections
    title: Kotlin documentation, Generics, in, out, where
    locator: Declaration-site variance and Type projections (Array<T> is invariant in T)
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.collections/-list/
    title: Kotlin standard library API, kotlin.collections.List
    locator: the declaration interface List<out E> and its description (read-only access)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/covariance-contravariance/
    title: C# programming guide, Covariance and Contravariance
    locator: the first example (IEnumerable<string> assigned to IEnumerable<object>, Action<object> assigned to Action<string>)
  - url: https://www.typescriptlang.org/docs/handbook/type-compatibility.html#function-parameter-bivariance
    title: TypeScript Handbook, Type Compatibility
    locator: Function Parameter Bivariance
  - url: https://www.typescriptlang.org/docs/handbook/release-notes/typescript-2-6.html#strict-function-types
    title: TypeScript 2.6 release notes
    locator: Strict function types (methods are excluded)
  - url: https://www.typescriptlang.org/tsconfig/#strictFunctionTypes
    title: TSConfig Reference
    locator: Strict Function Types (the setting applies to function syntax, not to method syntax)
  - url: https://doc.rust-lang.org/reference/subtyping.html#r-subtype.kinds
    title: The Rust Reference, Subtyping and Variance
    locator: "[subtype.kinds] (subtyping is restricted to lifetimes)"
  - url: https://doc.rust-lang.org/reference/subtyping.html#r-subtyping.variance.builtin-types
    title: The Rust Reference, Subtyping and Variance
    locator: "[subtyping.variance.builtin-types] (the variance table)"
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_classes_inheritance?view=powershell-7.5
    title: PowerShell documentation, about_Classes_Inheritance
    locator: Short description and Long description (single inheritance from a base class)
  - url: https://www.postgresql.org/docs/18/ddl-inherit.html
    title: PostgreSQL 18 Documentation
    locator: 5.11 Inheritance (INHERITS, and the limitation for indexes and foreign keys)
  - paper: doi:10.1016/0890-5401(88)90007-7
    title: Luca Cardelli, A semantics of multiple inheritance, Information and Computation 76(2-3), 1988
    locator: 1 Introduction (inheritance first appeared in Simula 67)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html#jls-4.10.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 4.10.1 Subtyping among Primitive Types
---

## Idea

A type S is a subtype of a type T, written S <: T, when a value of S can be used wherever a value of T is expected. Code that is written for T then works for every subtype of T, including subtypes that did not exist when the code was written.

Barbara Liskov stated the idea as a substitution property. Suppose that for each object o1 of type S there is an object o2 of type T, such that every program P defined in terms of T behaves the same when o1 replaces o2. Then S is a subtype of T.[^1] This rule is known as the Liskov substitution principle. It is a rule about behaviour, not only about names and signatures. Liskov and Wing later made it precise: a property that you can prove about the objects of T must also hold for the objects of S.[^2]

In the Java program below, `report` is written for the interface `Shape`. It works for `Square` and for `Circle`, because each class implements `Shape`.

```java run id=idea
import java.util.List;

public class Main {
    interface Shape {
        double area();
    }

    record Square(double side) implements Shape {
        public double area() {
            return side * side;
        }
    }

    record Circle(double radius) implements Shape {
        public double area() {
            return Math.PI * radius * radius;
        }
    }

    static void report(Shape shape) {
        System.out.printf("%s %.2f%n", shape.getClass().getSimpleName(), shape.area());
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Square(2), new Circle(1));
        for (Shape shape : shapes) {
            report(shape);
        }
    }
}
```

```output for=idea
Square 4.00
Circle 3.14
```

The call `shape.area()` selects the method of the object, not the method of the static type. This is dynamic dispatch. Subtyping and dynamic dispatch together are called subtype polymorphism, or inclusion polymorphism.

## How it works

Cardelli gave the basic rules in a type system with records and functions.[^3] Three of them carry most of the idea:

- **Subsumption.** If a value has type S and S <: T, then the value also has type T. A function that takes a T accepts it.
- **Records.** A record type is a subtype of another record type when it has all the fields of the other type, and possibly more, and the types of the common fields are subtypes. In Cardelli's example, `car = {age: int, speed: int, fuel: string}` is a subtype of `vehicle = {age: int, speed: int}`.
- **Functions.** A function type σ→τ is a subtype of σ'→τ' when σ' <: σ and τ <: τ'. The argument goes the opposite way to the result. Variance, below, explains why.

### Subtyping and inheritance

Inheritance and subtyping are different things. Inheritance reuses the code of a class in another class. Subtyping is a relation between types that says one can stand in for the other. Liskov notes that "subclass" and "superclass" are linguistic ideas, while "subtype" and "supertype" are semantic ideas. A subclass can implement a subtype, but a programmer can also use a subclass only to reuse an implementation.[^1][^4] A subclass that overrides a method and changes its meaning is still a subclass, but it is not a behavioural subtype. The Pitfalls section shows an example.

### Nominal and structural subtyping

A language decides S <: T in one of two ways.

- **Nominal subtyping.** S is a subtype of T only when the program says so. In Java, a class is a subtype of its superclass and of the interfaces that it declares in `extends` and `implements`.[^5] C#, Kotlin and C++ work the same way. Two classes with the same members are unrelated unless one names the other.
- **Structural subtyping.** S is a subtype of T when S has the members that T requires. Nobody declares the relation. Liskov described this "grouping approach" in 1987: a module accepts any type that supplies the operations that it needs.[^6] TypeScript bases type compatibility on structural subtyping.[^7] In Go, a type implements an interface when its method set has the methods of the interface, and a value is assignable to an interface type when its type implements the interface.[^8][^9] No `implements` declaration exists.

Python has both. At run time, `isinstance` follows the class hierarchy, which is nominal. An abstract base class can also accept an unrelated class as a "virtual subclass" through `register`.[^12] For static checks, PEP 544 adds protocol classes, which are structural, to the nominal rules of PEP 484.[^10] A type checker accepts any class with the members of the protocol. At run time, a protocol with the decorator `runtime_checkable` checks only that the attributes are present, and ignores their signatures.[^11] The Python example shows both checks.

In TypeScript, an object with more fields than a type requires is a subtype, as in Cardelli's rule for records. The function `norm` below takes a `Point`. It accepts an object with a third field and an instance of an unrelated class.

```typescript run id=structural
interface Point {
  x: number;
  y: number;
}

class Vector {
  x: number;
  y: number;
  constructor(x: number, y: number) {
    this.x = x;
    this.y = y;
  }
}

function norm(p: Point): number {
  return Math.hypot(p.x, p.y);
}

const point3 = { x: 3, y: 4, z: 12 };
console.log(norm(point3));
console.log(norm(new Vector(6, 8)));
```

```output for=structural
5
10
```

JavaScript has no static types. Its `instanceof` operator walks the prototype chain of the object, and gives true when it finds the `prototype` object of the constructor.[^13] The JavaScript example shows how to change the answer at run time.

### At the machine level

In C++, a pointer to a derived class converts to a pointer to a base class. The result points to the base class subobject of the object.[^14] The Itanium C++ ABI allocates the primary base class first.[^15] So with single inheritance the base subobject is at offset zero, and the conversion does not change the address. A second base class is at a different offset, so a conversion to it adds that offset (see the C++ example).

A call of a virtual function goes through a virtual table. Each object of a dynamic class holds a pointer to a table of information for its class, which includes the addresses of its virtual functions.[^16] A call loads the table pointer from the object, loads the address from the table, and calls it. The caller does not need to know the dynamic class.

A Go variable of interface type has a static type, the interface, and a dynamic type, the type of the value that it holds at run time.[^17] The specification does not say how the pair is stored. Russ Cox describes the implementation: an interface value is two words, a pointer to an interface table and a pointer to the data. The table holds the function pointers of the methods.[^18]

C has no subtyping. A common idiom puts the "base" struct as the first member of the "derived" struct. A pointer to a structure, suitably converted, points to its first member, and there is no padding at the start of a structure.[^19] So a `struct square *` converts to a pointer to its first member, a `struct shape`. The C example shows the idiom. The compiler does not check that the cast back is to the right type.

### Variance

Variance says how subtyping of parts gives subtyping of a whole. Take Cat <: Animal.

- **Results are covariant.** A method that returns a Cat can replace a method that returns an Animal. The caller expects an Animal and gets one.
- **Parameters are contravariant.** A function that accepts any Animal can replace a function that accepts a Cat. The caller passes a Cat, and the function can handle it.

Liskov and Wing list these as the signature rules of a subtype: contravariance of arguments, covariance of the result, and a rule for exceptions. Their definition adds rules for behaviour: pre-conditions, post-conditions, invariants and history constraints.[^20] The signature rules only prevent type errors. They do not make a stack a subtype of a queue, even when both have `put` and `get` of the right types.[^2]

C++ and Java allow covariant return types. An override in C++ can return a pointer or reference to a class derived from the class that the overridden function returns.[^21] In Java, the return type of an override must be return-type-substitutable for the return type of the overridden method, which for reference types allows a subtype.[^22] The Java and C++ examples show this.

A mutable container must be invariant. You read elements from it, which is a covariant use, and you write elements to it, which is a contravariant use. Only the same type is safe for both. Languages handle this in several ways.

- **Covariant arrays with a check at run time.** In Java, if S <: T for reference types, then S[] <: T[].[^23] The language pays for this with a check on every store into an array of references. A store of the wrong type throws `ArrayStoreException`.[^24][^25] C# arrays of reference types are covariant in the same way, with the same kind of check, and a wrong store throws `System.ArrayTypeMismatchException`. Arrays of value types are not covariant: no conversion exists from `int[]` to `object[]`.[^26] The first two pitfalls show both.
- **Invariant arrays.** Kotlin's `Array<T>` is invariant in `T`, so `Array<Int>` is not an `Array<Any>`.[^27]
- **Variance on read-only interfaces.** Kotlin declares `List<out E>`, which gives only read access, so `List<Int>` is a `List<Any>`.[^28] In C#, an `IEnumerable<string>` is an `IEnumerable<object>`, and an `Action<object>` is an `Action<string>`.[^29] Java decides variance at each use with wildcards. [[parametric-polymorphism]] covers generic variance and the invariance of `List<Integer>`.
- **Bivariant parameters.** When TypeScript compares two function types, it accepts the assignment if either parameter type is assignable to the other. The handbook calls this unsound and says that it allows many common JavaScript patterns.[^30] The `strictFunctionTypes` option checks parameters of function types contravariantly. It does not apply to methods, which stay bivariant.[^31][^32] The fourth pitfall shows the result.
- **Lifetimes only.** Rust has subtyping only between lifetimes and types with higher-ranked lifetimes. If you erase the lifetimes, the only subtyping is type equality.[^33] `&'static str` is a subtype of `&'a str`. The variance table of the Rust Reference makes `&'a T` covariant in `T`, `&'a mut T` invariant in `T`, and `fn(T)` contravariant in `T`.[^34] Traits are not types in a subtype relation. A `Box<dyn Trait>` is made by a coercion.

## In languages

The example files show subtyping in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | classes, with `isinstance` at run time; ABCs with `register`; protocols checked by a type checker[^10][^12] |
| JavaScript | native | `class ... extends`, the prototype chain and `instanceof`; no static check[^13] |
| TypeScript | native | structural; any value with the required members is compatible[^7] |
| Java | native | nominal, with `extends` and `implements`; covariant arrays[^5][^23] |
| C | idiom | a struct whose first member is the base struct[^19] |
| C++ | native | nominal, with base classes and virtual functions[^14] |
| C# | native | nominal, with base classes and interfaces; covariant arrays of reference types[^26] |
| Go | native | structural, through interfaces; no subtyping between struct types[^8] |
| Rust | native | lifetimes only; traits give polymorphism without a subtype relation[^33] |
| Kotlin | native | nominal, with `open` classes and interfaces; declaration-site variance[^27] |
| PowerShell | native | classes with single inheritance, and `-is`[^35] |
| Bash | n/a | no types |
| SQL | absent | no subtype relation between tables or row types; PostgreSQL has table inheritance[^36] |
| Regex | n/a | a pattern has no types |
| HTML | n/a | markup has no types |
| CSS | n/a | style rules have no types |

PostgreSQL lets a table inherit the columns of a parent table with `INHERITS`, and a query on the parent can include the rows of the children. The documentation calls the lack of index and foreign key support across the hierarchy a serious limitation.[^36]

## Variations and related ideas

- **Subtyping and generics.** Parametric polymorphism makes one definition work for all types, and subtyping makes one type stand in for another. Bounded type parameters, such as `<T extends Comparable<T>>`, join the two. [[parametric-polymorphism]] describes generics and their variance.
- **Width and depth.** Cardelli's rule for records allows two things: more fields (width) and more specific types for the common fields (depth).[^3] His records are values that a program does not update. For a field that a program can write, depth subtyping has the problem of a mutable container, which Variance explains. [[immutability]] describes read-only data.
- **Functions as values.** The rule for function types matters most when functions are passed as values, as in callbacks. [[higher-order-functions]] covers them.
- **Nullable types.** A type that allows null can act as a supertype of the same type without null. The Kotlin example assigns a `String` to a `String?`. [[null-references]] covers null checks.
- **Duck typing.** A dynamic language without static checks does not need a declared relation. A call works when the object has the method. The error, if any, comes at run time. [[static-and-dynamic-typing]] explains the difference.
- **Numeric widening.** Java also defines subtyping among the primitive types. `long` is a supertype of `int`, and `double` is a supertype of `float`.[^38] This relation is separate from subtyping between classes.

## Pitfalls

### A wrong store into a covariant array

A `String[]` is an `Object[]` in Java, so the compiler accepts a store of an `Integer` through the `Object[]` view. The JVM checks the store and throws `ArrayStoreException`.[^24][^25]

```java fails expect=exception:ArrayStoreException id=java-array-store
public class Main {
    public static void main(String[] args) {
        String[] names = {"Ada", "Grace"};
        Object[] items = names;
        items[0] = Integer.valueOf(42);
        System.out.println(names[0].length());
    }
}
```

The program stops with `java.lang.ArrayStoreException: java.lang.Integer`. Use a `List<String>` instead. A generic list is invariant, so the compiler rejects the assignment ([[parametric-polymorphism]] shows this).

### The same store in C#

C# arrays of reference types are covariant too. The store compiles and throws at run time.[^26]

```csharp fails expect=exception:ArrayTypeMismatchException id=csharp-array-store
string[] names = { "Ada", "Grace" };
object[] items = names;
items[0] = new object();
Console.WriteLine(names[0].Length);
```

The program stops with `System.ArrayTypeMismatchException`. A read-only view, such as `IEnumerable<object>`, is safe, because it has no store.[^29]

### A subclass that breaks the contract of its base

A class `Square` that extends `Rectangle` looks natural: every square is a rectangle. But a mutable `Rectangle` promises that `set_width` changes the width and keeps the height. A `Square` must keep its sides equal, so it breaks that promise. Code written for `Rectangle` then gives a wrong answer. The type checker accepts the program, because the signatures match. This is what Liskov and Wing mean when they say that the signature rules do not ensure behaviour.[^2][^20]

```python wrong id=square-rectangle
class Rectangle:
    def __init__(self, width: float, height: float) -> None:
        self.width = width
        self.height = height

    def set_width(self, width: float) -> None:
        self.width = width

    def area(self) -> float:
        return self.width * self.height


class Square(Rectangle):
    def __init__(self, side: float) -> None:
        super().__init__(side, side)

    def set_width(self, width: float) -> None:
        self.width = width
        self.height = width


def stretch(shape: Rectangle) -> float:
    shape.set_width(shape.width * 2)
    return shape.area()


print(stretch(Rectangle(2, 3)))
print(stretch(Square(3)))
```

```output for=square-rectangle
12
36
```

`stretch` expects the area to double, from 6 to 12 and from 9 to 18. The square gives 36. An immutable `Square`, or a common base `Shape` with no setters, avoids the problem.

### Method parameters in TypeScript

TypeScript checks the parameters of methods bivariantly, also with `strictFunctionTypes`.[^31][^32] The object below declares `handle` with a `Dog` parameter, and the compiler accepts it as a `Handler`, whose method takes any `Animal`. A call with an `Animal` then fails at run time. The runner strips the types and does not run the type checker, so the block shows the run time failure.

```typescript fails expect=exception:TypeError id=ts-method-bivariance
class Animal {
  name = "animal";
}

class Dog extends Animal {
  bark(): string {
    return "woof";
  }
}

interface Handler {
  handle(animal: Animal): string;
}

const dogsOnly: Handler = {
  handle(dog: Dog): string {
    return dog.bark();
  },
};

console.log(dogsOnly.handle(new Animal()));
```

The program stops with `TypeError: dog.bark is not a function`. If `Handler` declares a property of function type, `handle: (animal: Animal) => string`, then `strictFunctionTypes` rejects the assignment.[^32]

### Slicing in C++

A C++ function that takes a base class by value copies only the base part of the argument. The copy is a real base object, so a virtual call on it uses the base class version.

```cpp wrong id=cpp-slicing
#include <iostream>
#include <string>

struct Shape {
    virtual ~Shape() = default;
    virtual std::string name() const { return "shape"; }
};

struct Circle : Shape {
    std::string name() const override { return "circle"; }
};

void by_value(Shape shape) { std::cout << shape.name() << "\n"; }
void by_reference(const Shape& shape) { std::cout << shape.name() << "\n"; }

int main() {
    Circle circle;
    by_value(circle);
    by_reference(circle);
}
```

```output for=cpp-slicing
shape
circle
```

Take a polymorphic argument by reference or by pointer.

## Cost

- **Dynamic dispatch.** A virtual call loads the table pointer from the object and the address from the table, and then calls the address.[^16] A Go interface call goes through the interface table in the same way.[^18]
- **Pointer conversion.** A conversion to the primary base class costs nothing under the Itanium ABI, because the base is at offset zero. A conversion to another base adds an offset.[^14][^15]
- **The array store check.** Covariant arrays in Java and C# need a type check on each store into an array of references.[^24][^25][^26] Arrays of primitive types and value types have no such check.
- **Checked casts.** A cast from a supertype to a subtype, such as `(Square) shape` in Java, checks the dynamic type at run time. The JVM uses the same rules for the store check of `aastore` as for the cast instruction `checkcast`.[^25]
- **Structural checks.** Structural subtyping in TypeScript costs nothing at run time, because the types are erased. In Python, the documentation warns that an `isinstance` check against a runtime-checkable protocol can be slow compared with a check against an ordinary class.[^11]

## History

- **Simula 67.** According to Cardelli, inheritance and object-oriented programming first appeared in Simula 67. Objects of a class could appear wherever objects of a superclass were expected.[^37]
- **1987.** Barbara Liskov gave the keynote "Data abstraction and hierarchy" at OOPSLA '87. It defined a subtype by the substitution property and separated type hierarchy from implementation hierarchy.[^1][^4]
- **1988.** Luca Cardelli published "A semantics of multiple inheritance" in Information and Computation. It gave the subtyping rules for records and for function types.[^3]
- **1994.** Barbara Liskov and Jeannette Wing published "A behavioral notion of subtyping". It defined subtypes by the properties that a program can prove about the supertype, including invariants and history constraints.[^2][^20]
- **TypeScript 2.6.** This release added the `strictFunctionTypes` option, which checks the parameters of function types contravariantly but not those of methods.[^31]

## References
