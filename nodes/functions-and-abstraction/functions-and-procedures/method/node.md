---
tag: Z3ZCTB
kind: concept
slug: method
name: Method
aliases: [member function, instance method, static method, associated function]
area: functions-and-abstraction/functions-and-procedures
level: intro
summary: >-
  A method is a function that belongs to a type. It gets a value of the type, the receiver, as an extra parameter, and a program calls it with the receiver before a dot, as in r.area().
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1967
  people: [Ole-Johan Dahl, Kristen Nygaard]
  note: Simula 67 put procedures in classes, and Smalltalk used the word method for the code that a class runs for a message. Go and Rust later allowed methods on types that are not classes.
see_also: [method-concept, function, function-call, function-signature, overloading, extension-methods, dynamic-dispatch, parameter-passing]
sources:
- url: https://go.dev/ref/spec#Method_declarations
  title: The Go Programming Language Specification
  locator: Method declarations; Method expressions
- url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
  title: Methods, The Rust Programming Language
  locator: Method Syntax; Associated Functions
- url: https://docs.python.org/3/tutorial/classes.html#method-objects
  title: Classes, The Python Tutorial
  locator: 9.3.4 Method Objects; 9.4 Random Remarks
- url: https://doc.rust-lang.org/reference/items/associated-items.html
  title: Associated Items, The Rust Reference
  locator: items.associated.fn.method.intro
- url: https://developer.mozilla.org/en-US/docs/Glossary/Method
  title: Method, MDN Web Docs Glossary
  locator: the whole page
- url: https://www.typescriptlang.org/docs/handbook/2/classes.html#methods
  title: Classes, The TypeScript Handbook
  locator: Methods
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html#jls-8.4.3.2
  title: Classes, The Java Language Specification, Java SE 25 Edition
  locator: 8.4.3.2 static Methods
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/methods
  title: Methods, C# Programming Guide
  locator: Method signatures
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/static-classes-and-static-class-members
  title: Static Classes and Static Class Members, C# Programming Guide
  locator: Static Members
- url: https://en.cppreference.com/w/c/language/functions.html
  title: Functions, C reference, cppreference.com
  locator: Explanation
- url: https://en.cppreference.com/w/cpp/language/member_functions.html
  title: Non-static member functions, C++ reference, cppreference.com
  locator: the introduction
- url: https://en.wikipedia.org/wiki/Method_(computer_programming)
  title: Method (computer programming), Wikipedia
  locator: the introduction
- url: https://go.dev/doc/faq#methods_on_values_or_pointers
  title: Frequently Asked Questions, The Go Programming Language
  locator: Should I define methods on values or pointers?
- url: https://en.wikipedia.org/wiki/Simula
  title: Simula, Wikipedia
  locator: History
- url: https://en.wikipedia.org/wiki/Smalltalk
  title: Smalltalk, Wikipedia
  locator: Messages
---

## Idea

A method is a function that belongs to a type. It works on one value of that type, called the receiver. A program calls a function as `area(r)`, and it calls a method as `r.area()`. The receiver comes before the dot, and the method gets it as an extra parameter.

The Go specification says it in one line: "A method is a function with a receiver."[^1] The Rust book compares the two: "Methods are similar to functions: We declare them with the fn keyword and a name, they can have parameters and a return value, and they contain some code that’s run when the method is called from somewhere else."[^2] The difference is that methods "are defined within the context of a struct (or an enum or a trait object, which we cover in Chapter 6 and Chapter 18, respectively), and their first parameter is always self, which represents the instance of the struct the method is being called on"[^2].

```python run id=idea
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height


def area(rectangle):
    return rectangle.width * rectangle.height


r = Rectangle(3, 4)
print(area(r))
print(r.area())
```

```output for=idea
12
12
```

The function `area` and the method `Rectangle.area` do the same work. The function takes the rectangle as a normal parameter. The method takes it as `self`, and the call `r.area()` passes `r` without writing it in the parentheses.

This page compares methods with functions. [[method-concept]] describes methods from the side of objects and classes: bound and unbound methods, lookup through the type and `this`.

## How it works

1. **The declaration.** A method is declared inside a class, a struct or an `impl` block, or with a receiver parameter in Go. The declaration ties the method to one type.
2. **The receiver parameter.** The method has one more parameter than its call shows. Python names it `self` in the parameter list. Python says that "the special thing about methods is that the instance object is passed as the first argument of the function"[^3]. Java, C#, C++, JavaScript and TypeScript add the parameter `this` without a declaration.
3. **The call.** A call `r.area()` finds the method through the type of `r`, then calls it with `r` as the receiver. The rest of the call works as a [[function-call]].
4. **The function form.** Many languages can also call a method as a plain function with the receiver as the first argument. In Go, "T.M is a function that is callable as a regular function with the same arguments as M prefixed by an additional argument that is the receiver of the method"[^1]. In Rust, a method "may be invoked using the method call operator, for example, x.foo(), as well as the usual function call notation"[^4].

## In languages

- **Python.** A method is a function defined in a class body. Its first parameter is the receiver, by convention `self`. Inside the method, the code reads the fields through `self`: "There is no shorthand for referencing data attributes (or other methods!) from within methods."[^3]
- **JavaScript.** "A method is a function which is a property of an object."[^5] The object can be a class instance or a plain object literal. The receiver is `this`.
- **TypeScript.** "A function property on a class is called a method."[^6] Methods have the same type annotations as functions.
- **Java.** All functions are methods. A `static` method has no receiver: "A class method is always invoked without reference to a particular object."[^7] "An instance method is always invoked with respect to an object, which becomes the current object to which the keywords this and super refer during execution of the method body."[^7]
- **C#.** "Methods are declared in a class, struct, or interface"[^8]. As in Java, a `static` method has no receiver, and "The static member is always accessed by the class name, not the instance name."[^9]
- **C.** C has no methods. "each function definition must appear at file scope"[^10], so a struct cannot hold a function. A C program writes a function whose first parameter is a pointer to the struct.
- **C++.** C++ has free functions and member functions. "A non-static member function is a function that is declared in a member specification of a class without a static or friend specifier"[^11].
- **Go.** Go has no classes. A method is a function with a receiver parameter before its name. The receiver type "must be a defined type T or a pointer to a defined type T"[^1], and it "must be declared in the same package as the method"[^1]. So a Go program can put methods on any type that it defines, such as `type Celsius float64`.
- **Rust.** Methods are in `impl` blocks, and the receiver is `self`, `&self` or `&mut self`. An `impl` block can be for a struct or for an enum[^2]. A function in an `impl` block with no `self` is an associated function and not a method[^2].
- **Bash.** Bash has no types with functions, so it has no methods.

## Variations and related ideas

- **Static methods.** A static method in Java or C# and an associated function in Rust belong to a type but have no receiver. They are plain functions in the namespace of the type[^7][^2].
- **Methods on types that are not classes.** Go puts methods on any defined type in the package[^1], and Rust puts them on structs and enums[^2]. A method does not need a class.
- **Value and pointer receivers.** Go and Rust let the method choose if it gets a copy of the receiver or a reference to it. See the pitfall below.
- **Method or function.** The Rust book says: "The main reason for using methods instead of functions, in addition to providing method syntax and not having to repeat the type of self in every method’s signature, is for organization."[^2] A method keeps the operations of a type in one place.
- **Overriding.** A subclass can give its own version of a method, and the call chooses the version from the type of the receiver at run time. Wikipedia says that "One of the most important capabilities that a method provides is method overriding"[^12]. See [[dynamic-dispatch]].
- **Extension methods.** C# and Kotlin can add methods to a type without changing it. See [[extension-methods]].
- **Overloading.** A class can have several methods with one name and different parameters. See [[overloading]].

## Pitfalls

### A field name without self in Python

A Python method must read and write fields through `self`. A name without `self.` is a local variable of the method:

```python wrong id=python-missing-self
class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        count = self.count + 1


c = Counter()
c.increment()
c.increment()
print(c.count)
```

```output for=python-missing-self
0
```

`count = self.count + 1` makes a local variable `count` and drops it when the method returns. Python has "no shorthand for referencing data attributes"[^3]. Write `self.count = self.count + 1`.

### A value receiver in Go changes a copy

In Go, the receiver "behaves exactly as if it were an argument to the method"[^13]. A method with a value receiver gets a copy:

```go wrong id=go-value-receiver
package main

import "fmt"

type Counter struct {
	count int
}

func (c Counter) Increment() {
	c.count++
}

func main() {
	var c Counter
	c.Increment()
	c.Increment()
	fmt.Println(c.count)
}
```

```output for=go-value-receiver
0
```

The Go FAQ asks if the method needs to modify the receiver, and says: "If it does, the receiver must be a pointer."[^13] It also says that a value method "is called with a copy of the caller’s argument (that’s the definition of passing a value), so changes it makes will be invisible to the caller"[^13]. Write `func (c *Counter) Increment()`.

## Cost

A method call that the compiler resolves at compile time costs the same as a function call with one more argument. A call that chooses the method at run time from the type of the receiver costs one more lookup. [[dynamic-dispatch]] describes this cost.

## History

Simula 67 introduced classes with procedures in them. Dahl and Nygaard "presented their paper on class and subclass declarations at the IFIP Working Conference on simulation languages in Oslo, May 1967"[^14]. Smalltalk used the word method for the code that a class runs for a message. "Sending a message to an object at the most abstract involves fetching the class of the receiver (the object being sent the message) and looking up the message's selector in the class's method dictionary"[^15].

Go and Rust separate methods from classes. Go allows methods on any defined type of a package[^1], and Rust puts methods in `impl` blocks for structs and enums[^2].

## References
