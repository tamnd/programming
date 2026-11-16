---
tag: W941G8
kind: feature
owner: python
slug: lambda-expressions
name: Lambda expressions
aliases: [lambda, anonymous function, lambda function, lambda x, inline function, key=lambda, lambda in a loop]
area: languages/python/functions
level: intro
summary: >-
  lambda parameters: expression makes an unnamed function whose body is one expression. It is a short form of a def, and it is often used as a small function argument, such as a sort key.
grammar: "lambda_expr: 'lambda' [parameter_list] ':' expression (Language Reference, 6.14)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#lambda", locator: "6.14. Lambdas"}
see_also: [python/defining-functions, python/closures-and-nonlocal, python/function-objects, python/functools, python/conditional-expressions]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#lambda
    title: The Python Language Reference, Python 3.15
    locator: 6.14. Lambdas
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#lambda-expressions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.6. Lambda Expressions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-do-lambdas-defined-in-a-loop-with-different-values-all-return-the-same-result
    title: Programming FAQ, Python 3.15
    locator: Why do lambdas defined in a loop with different values all return the same result?
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/functional.html#small-functions-and-the-lambda-expression
    title: Functional Programming HOWTO, Python 3.15
    locator: Small functions and the lambda expression
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_literals
    title: The Go Programming Language Specification
    locator: Function literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=sort-key
pairs = [(1, "one"), (2, "two"), (3, "three"), (4, "four")]
pairs.sort(key=lambda pair: pair[1])
print(pairs)
```

```output for=sort-key
[(4, 'four'), (1, 'one'), (3, 'three'), (2, 'two')]
```

This is the example of the tutorial. The lambda gives the second item of each pair, so `sort` orders the pairs by their names.

## Semantics

**r1.** "Lambda expressions (sometimes called lambda forms) are used to create anonymous functions. The expression lambda parameters: expression yields a function object."[^1]

**r2.** "The unnamed object behaves like a function object defined with:" `def <lambda>(parameters): return expression`.[^1] Example: [`name`](#name).

**r3.** "functions created with lambda expressions cannot contain statements or annotations."[^1] Example: [`statement`](#statement).

**r4.** "They are syntactically restricted to a single expression. Semantically, they are just syntactic sugar for a normal function definition. Like nested function definitions, lambda functions can reference variables from the containing scope".[^2] Example: [`incrementor`](#incrementor).

**r5.** For the parameters of a lambda, the reference says: "See section Function definitions for the syntax of parameter lists."[^1] So a lambda can have defaults, `*args` and keyword-only parameters. Example: [`parameters`](#parameters).

## Examples

```python run id=incrementor
def make_incrementor(n):
    return lambda x: x + n

f = make_incrementor(42)
print(f(0), f(1))
```

```output for=incrementor
42 43
```

The lambda uses `n` from the function that contains it (rule r4).

```python run id=name
square = lambda x: x * x

def square_def(x):
    return x * x

print(square(4), square.__name__, square_def.__name__)
```

```output for=name
16 <lambda> square_def
```

The function object of a lambda has the name `<lambda>` (rule r2).

```python run id=parameters
greet = lambda name, *, punct="!": "Hi " + name + punct
print(greet("ann"), greet("bob", punct="?"))
print((lambda *args: sum(args))(1, 2, 3))
```

```output for=parameters
Hi ann! Hi bob?
6
```

## Pitfalls

A lambda body is one expression, so it cannot hold a statement such as return:

```python fails id=statement expect=exception:SyntaxError
f = lambda x: return x
```

Lambdas made in a loop use the loop variable when they are called, not when they are made:

```python wrong id=loop
squares = []
for x in range(5):
    squares.append(lambda: x ** 2)

print(squares[2](), squares[4]())
```

```output for=loop
16 16
```

The FAQ says: "This happens because x is not local to the lambdas, but is defined in the outer scope, and it is accessed when the lambda is called".[^3] Its fix is a default value, `lambda n=x: n ** 2`, because "n=x creates a new variable n local to the lambda and computed when the lambda is defined".[^3]

A long lambda is hard to read. The Functional Programming HOWTO says: "If you try to do too much in a lambda statement, you’ll end up with an overly complicated expression that’s hard to read."[^4] One of the rules that it gives is: "Convert the lambda to a def statement, using that name."[^4]

## Comparison

Go has function literals: "A function literal represents an anonymous function."[^5] A Go function literal has a full body with statements, and "Function literals are closures: they may refer to variables declared in a surrounding function."[^5] A Python lambda is also a closure, but its body is only one expression (rule r4).
