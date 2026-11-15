---
tag: HN1R5F
kind: concept
slug: method-concept
name: Method
aliases: [method, instance method, member function, receiver, bound method, unbound method, method value, method expression, self, this]
area: objects-and-classes/classes
level: intro
summary: >-
  A method is a function that belongs to a type and is called on a value of it, the receiver, which the method gets as a hidden or explicit first parameter.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1980
  people: [Alan Kay, Dan Ingalls, Adele Goldberg]
  note: Smalltalk classes hold method dictionaries that map message selectors to methods. C++ calls methods member functions.
see_also: [classes-and-instances, define-a-method-on-a-type, define-a-class, field-and-property, dynamic-dispatch, objects-and-messages, closures, higher-order-functions]
same_as:
  wikipedia: Method_(computer_programming)
sources:
  - url: https://docs.python.org/3/tutorial/classes.html
    title: 9. Classes, The Python Tutorial
    locator: 9.3.4 Method Objects
  - url: https://docs.python.org/3/howto/descriptor.html
    title: Descriptor Guide, Python HOWTOs
    locator: Functions and methods
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations; Method values; Method expressions
  - url: https://doc.rust-lang.org/book/ch05-03-method-syntax.html
    title: Methods, The Rust Programming Language
    locator: Method Syntax; Where's the -> Operator?
  - url: https://en.wikipedia.org/wiki/Smalltalk
    title: Smalltalk, Wikipedia
    locator: Reflection; Image-based persistence
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/this
    title: this, MDN Web Docs
    locator: Function context
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/bind
    title: Function.prototype.bind(), MDN Web Docs
    locator: Description
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-8.html
    title: Chapter 8, Classes, The Java Language Specification, Java SE 25 Edition
    locator: 8.4.3.2 static Methods
  - url: https://en.cppreference.com/cpp/language/member_functions
    title: Non-static member functions, cppreference.com
    locator: Introduction; Explicit object member functions
  - url: https://en.wikipedia.org/wiki/Method_(computer_programming)
    title: Method (computer programming), Wikipedia
    locator: Introduction; Member functions in C++
---

## Idea

A method is a function that belongs to a type and works on one value of that type, the receiver. The call `r.area()` does two things: it finds `area` through the type of `r`, and it calls the function with `r` as an extra argument. The function body reaches the receiver through a parameter, `self` in Python and Rust, a named receiver in Go, and the hidden `this` in Java, C#, C++ and JavaScript. So a method is an ordinary function plus a rule for lookup and a rule for passing the receiver, and most languages let a program see both parts:

```python run id=idea
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height


r = Rectangle(3, 4)
print(r.area())           # a method call
print(Rectangle.area(r))  # the same function, with the receiver passed by hand
f = r.area                # a bound method: the function and r together
print(f())
print(type(f).__name__, f.__self__ is r)
```

```output for=idea
12
12
12
method True
```

`r.area()` and `Rectangle.area(r)` call the same function with the same argument. `r.area` without the call makes a bound method: an object that holds the function and the receiver, and calls the function with the receiver when it is called.

## How it works

**The receiver is the first argument.** The Python tutorial states the rule: "the special thing about methods is that the instance object is passed as the first argument of the function."[^1] The Python descriptor guide says the same from the side of the implementation: "Methods only differ from regular functions in that the object instance is prepended to the other arguments."[^2] Go makes the receiver a declared parameter, written before the method name: "A method is a function with a receiver."[^3] Rust methods are "similar to functions",[^4] with `self`, `&self` or `&mut self` as the first parameter, and the form of that parameter says whether the method takes the value, reads it or changes it. Java, C# and C++ do not declare the receiver: the compiler adds it as the hidden parameter `this`, and a field name alone, such as `width`, means `this.width`.

**Lookup goes through the type.** A call `x.m()` finds `m` from the type of `x`, not from `x` itself. Smalltalk set the model: a class holds a method dictionary, and a message send looks up the selector in the class of the receiver, then in the superclass.[^5] Python, JavaScript and Ruby do the same lookup at run time, through the class or the prototype chain. Java, C#, C++, Go and Rust find the method at compile time from the static type, and a virtual method, an interface method or a trait object adds one indirect call at run time. See [dynamic dispatch](/dynamic-dispatch/) and [objects and messages](/objects-and-messages/).

**Bound and unbound forms.** A method can be used as a value in two forms. The unbound form is the function with the receiver as its first parameter: `Rectangle.area` in Python, `Rectangle::area` in Java and Rust, `Rectangle.Area` in Go, and `&Rectangle::area` in C++. The bound form fixes one receiver and takes the other arguments only. In Python, if the name found on the class is a function, "references to both the instance object and the function object are packed into a method object".[^1] Go calls `r.Area` a method value, and the spec states that the receiver "is evaluated and saved during the evaluation of the method value".[^3] Java writes `r::area`, and C# converts the method group `r.Area` to a delegate. C++ has no bound method type, and code uses a lambda that captures the object.

**JavaScript sets the receiver at each call.** A JavaScript method is a plain function stored on the prototype, and `this` comes from the form of the call, not from the place where the function is defined. In `r.area()` it is `r`. In a plain call, "this typically refers to the global object (in non-strict mode) or undefined (in strict mode)".[^6] `bind` fixes the receiver: it "creates a new function that, when called, calls this function with its this keyword set to the provided value".[^7] Arrow functions have no `this` of their own and take it from the code around them.

**Instance methods and static methods.** A static method belongs to the type and has no receiver. The JLS states the difference: "A class method is invoked relative to the class; an instance method is invoked with respect to some particular object that is an instance of a class."[^8] A Java or C# static method is not the same as a Python `classmethod`: the Python form receives the class as its first argument, so a subclass that calls it passes the subclass, and the method can create instances of it. Python's `staticmethod` is the form that receives nothing.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `def` in a class body; explicit `self`; bound method objects |
| JavaScript | native | functions on the prototype; `this` set by each call; `bind` |
| TypeScript | native | as JavaScript, with a `this` parameter type for checks |
| Java | native | instance and `static` methods; method references `r::m`, `T::m` |
| Kotlin | native | member functions; bound references `r::m`; extension functions |
| C# | native | instance and `static` methods; delegates from method groups |
| Go | native | functions with a receiver on any named type of the package; method values and expressions |
| Rust | native | functions in `impl` blocks with `self`, `&self` or `&mut self` |
| C | idiom | functions that take a pointer to a struct as the first argument |
| C++ | native | member functions; `this`; `const` and reference qualifiers; pointers to members |
| Bash | n/a | no types with functions |
| PowerShell | native | methods in `class` definitions; `$this` |
| SQL | idiom | methods of user-defined types in some databases |
| Regex | n/a | no functions |
| HTML | n/a | no functions; methods belong to the DOM classes in JavaScript |
| CSS | n/a | no functions on values |

The languages differ in how they pass the receiver and in what a method value holds. Python, Rust and Go name the receiver in the declaration, and the other languages add `this` without a declaration. Python, Java, C# and Go have a bound method value that holds the receiver, while JavaScript has none: reading a method gives the plain function, and the receiver is lost unless the code calls `bind`. Go copies the receiver into the method value when the method has a value receiver, while Python, Java and C# hold a reference to the object.

Each example file defines a rectangle with an `area` method and, apart from JavaScript, a `scale` method. It calls `area` in several forms: as a method call, as an unbound function that takes the receiver as an argument, and as a bound value that holds the receiver. The JavaScript file shows instead how a method loses its receiver and how `bind` fixes it.

## Variations and related ideas

- **Class methods and static methods.** Python has `classmethod`, which receives the class, and `staticmethod`, which receives nothing. Java, C# and C++ have only the static form. Kotlin uses a companion object for members of the class.
- **Extension methods.** C# extension methods and Kotlin extension functions are static functions that the call syntax lets a program call like methods, without changing the type. Go and Rust allow methods only in the package or crate that owns the type, and Rust adds methods to types of other crates through local traits.
- **Explicit receivers in C++.** C++23 lets a member function declare its object parameter with `this`, as in `int area(this const Rectangle& self)`, which is close to the Python and Rust form.[^9]
- **Const and mutating methods.** C++ marks a method that does not change the object with `const`, Rust with `&self`, and Swift requires `mutating` on a method that changes a struct.
- **Methods without classes.** C code writes a method as a function whose first parameter is a pointer to a struct, such as `rect_area(const struct rect *r)`. Go and Rust put methods on structs and on other named types. See [define a method on a type](/define-a-method-on-a-type/) and [define a class](/define-a-class/).
- **Multimethods.** Common Lisp and Julia choose a method from the types of all the arguments, not only from the first one. See [dynamic dispatch](/dynamic-dispatch/).
- **Functions as values.** A bound method is a closure over its receiver. See [closures](/closures/) and [higher-order functions](/higher-order-functions/).

## Pitfalls

- **A lost receiver in JavaScript.** Passing `r.area` as a callback passes the function only, and the call gets `this` as `undefined` in strict mode, which throws a `TypeError` at the first field access. The example file shows the error and the two fixes, `bind` and an arrow function.
- **A missing `self` in Python.** A method defined without the `self` parameter still gets the instance as its first argument when it is called on an instance, so the call fails:

```python run id=missing-self
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area():  # the self parameter is missing
        return 0


r = Rectangle(3, 4)
try:
    r.area()
except TypeError as e:
    print(type(e).__name__)

print(r.area is r.area)  # each lookup makes another bound method
print(r.area == r.area)  # they compare equal: same function, same instance
```

```output for=missing-self
TypeError
False
True
```

The program also shows that each read of `r.area` makes another bound method object, so `is` gives `False` for two reads, while `==` gives `True`.

- **A Go method value copies the receiver.** For a method with a value receiver, `v := r.Area` copies `r` at that point, and later changes to `r` do not reach `v()`. The example file shows `value()` printing the area from before the scale. A method with a pointer receiver saves the pointer, so it sees the changes.
- **A Java static method in place of a Python `classmethod`.** Code ported from Python that turns a `classmethod` factory into a Java `static` method loses the class argument, so a subclass gets instances of the base class. Java code passes a factory or overrides the method in the subclass.
- **Calling an instance method from a static context.** A static method has no receiver, so a call to an instance method or a read of an instance field without an object is a compile error in Java and C#.
- **Receivers in Go `defer` and `go` statements.** In Go, `defer r.Close()` and `go r.Run()` evaluate the receiver when the statement runs, not when the call happens.

## Cost

A method call that the compiler resolves at compile time costs the same as a function call with one more argument, and the compiler can inline it. A call through a virtual function, an interface or a trait object adds one indirect call, which blocks inlining unless the compiler can prove the type. Python and JavaScript look up the method at run time, and their implementations cache the result of the lookup at each call site. A bound method value costs an allocation in Python, in Go when it escapes, and for a Java method reference or a C# delegate that captures a receiver.

## History

Simula 67 attached procedures to classes, and Smalltalk, built at Xerox PARC by Alan Kay, Dan Ingalls, Adele Goldberg and others in the 1970s, used the word method for the code that answers a message: an object receives a message, and its class maps the selector of the message to a method.[^5] Wikipedia describes a method as "a procedure associated with an object, and generally also a message".[^10] Bjarne Stroustrup's C++ used the term member function, and the C++ standard still uses it.[^9] Python, released in 1991, made the receiver an explicit parameter, and Go in 2009 and Rust in 2015 kept the receiver as a declared parameter but dropped classes.

## References
