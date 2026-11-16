---
tag: 5AW8P8
kind: feature
owner: python
slug: method-objects
name: Method Objects
aliases: [bound method, method object, self, instance method, __self__, __func__, call a method through the class]
area: languages/python/user-types
level: core
summary: >-
  x.f gives a bound method: the instance and the function packed together. A call to it passes the instance as the first argument, so x.f() is the same as MyClass.f(x).
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#instance-methods", locator: "3.2.8.2. Instance methods"}
see_also: [python/classes, python/instance-objects, python/function-objects, python/class-objects]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#method-objects
    title: The Python Tutorial, 9. Classes
    locator: 9.3.4. Method Objects; 9.4. Random Remarks
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#instance-methods
    title: The Python Language Reference, 3. Data model
    locator: 3.2.8.2. Instance methods
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/descriptor.html#functions-and-methods
    title: Descriptor Guide
    locator: Functions and methods
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Method_values
    title: The Go Programming Language Specification
    locator: Method expressions; Method values
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class MyClass:
    def f(self):
        return 'hello world'

x = MyClass()
xf = x.f
print(xf())
print(x.f() == MyClass.f(x))
```

```output for=basics
hello world
True
```

`x.f` is a method object. The program can keep it and call it later. `MyClass.f(x)` passes the instance by hand.

## Semantics

**r1.** "Usually, a method is called right after it is bound".[^1] But "x.f is a method object, and can be stored away and called at a later time."[^1] Example: [`stored`](#stored).

**r2.** "the special thing about methods is that the instance object is passed as the first argument of the function. In our example, the call x.f() is exactly equivalent to MyClass.f(x)."[^1]

**r3.** "When a non-data attribute of an instance is referenced, the instance’s class is searched. If the name denotes a valid class attribute that is a function object, references to both the instance object and the function object are packed into a method object."[^1] "When the method object is called with an argument list, a new argument list is constructed from the instance object and the argument list, and the function object is called with this new argument list."[^1]

**r4.** When a method object is made from a function through an instance, "its __self__ attribute is the instance, and the method object is said to be bound. The new method’s __func__ attribute is the original function object."[^2] Example: [`attrs`](#attrs).

**r5.** "Any function object that is a class attribute defines a method for instances of that class. It is not necessary that the function definition is textually enclosed in the class definition".[^1] Example: [`outside`](#outside).

**r6.** The mechanism is the descriptor protocol: "functions include the __get__() method for binding methods during attribute access. This means that functions are non-data descriptors that return bound methods during dotted lookup from an instance."[^3]

## Examples

```python run id=stored
class Greeter:
    def __init__(self, name):
        self.name = name

    def greet(self, greeting):
        return f"{greeting}, {self.name}"

hello_ada = Greeter("Ada").greet
hello_bob = Greeter("Bob").greet
print(hello_ada("Hi"), "|", hello_bob("Hello"))
print(list(map(Greeter("Cy").greet, ["Hi", "Bye"])))
```

```output for=stored
Hi, Ada | Hello, Bob
['Hi, Cy', 'Bye, Cy']
```

Each bound method keeps its own instance (rule r1). A bound method can go to map() as a function of one argument.

```python run id=attrs
class C:
    def f(self, n):
        return n * 2

x = C()
m = x.f
print(m.__self__ is x, m.__func__ is C.f, m(21), C.f(x, 21))
print(C.__dict__["f"] is C.f, C.f.__get__(x)(21))
```

```output for=attrs
True True 42 42
True 42
```

The function in the class dictionary has a `__get__` method. A call to `__get__` with an instance gives a bound method (rules r4 and r6).

```python run id=outside
def f1(self, x, y):
    return min(x, x + y)

class C:
    f = f1

    def g(self):
        return 'hello world'

    h = g

c = C()
print(c.f(3, -1), c.g(), c.h())
```

```output for=outside
2 hello world hello world
```

`f`, `g` and `h` are all class attributes that refer to functions, so they are all methods (rule r5).

## Pitfalls

A function stored on the instance is not bound. The lookup finds it in the instance dictionary, not in the class:

```python fails id=instance-func expect=exception:TypeError
class C:
    pass

def show(self):
    return "shown"

c = C()
c.show = show
c.show()
```

Only a function that is a class attribute makes a method (rule r3). Put the function on the class, or call it as `c.show(c)`.

## Comparison

Go has the same two forms. "The method value x.M is a function value that is callable with the same arguments as a method call of x.M."[^4] "The expression x is evaluated and saved during the evaluation of the method value".[^4] A method expression `T.M` "is a function that is callable as a regular function with the same arguments as M prefixed by an additional argument that is the receiver of the method."[^4] So Python `x.f` is like a Go method value, and Python `MyClass.f(x)` is like a Go method expression (rule r2).
