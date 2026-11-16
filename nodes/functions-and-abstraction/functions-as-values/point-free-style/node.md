---
tag: KWFBHJ
kind: technique
slug: point-free-style
name: Point-free style
aliases: [tacit programming, pointfree style, point-free programming, pointless style, eta reduction]
area: functions-and-abstraction/functions-as-values
level: advanced
summary: >-
  Define a function as a composition of other functions, with partial application and function names in place of lambdas, so that the definition never names the argument that it works on.
see_also: [function-composition, combinator, currying, higher-order-functions, anonymous-function, map-filter-reduce, first-class-function, collection-pipeline]
sources:
- url: https://en.wikipedia.org/wiki/Tacit_programming
  title: Tacit programming, Wikipedia
  locator: the introduction; Python; Functional programming; Unix pipeline
- url: https://wiki.haskell.org/Pointfree
  title: Pointfree, HaskellWiki
  locator: the introduction; But pointfree has more points; Tool support
- url: https://en.wikipedia.org/wiki/Lambda_calculus
  title: Lambda calculus, Wikipedia
  locator: Reduction
- url: https://doc.rust-lang.org/book/ch20-04-advanced-functions-and-closures.html
  title: Advanced Functions and Closures, The Rust Programming Language
  locator: Function Pointers, Listing 20-30
- url: https://docs.python.org/3/library/functools.html#functools.partial
  title: functools, The Python Standard Library
  locator: functools.partial
- url: https://docs.python.org/3/library/operator.html#operator.itemgetter
  title: operator, The Python Standard Library
  locator: operator.itemgetter; operator.methodcaller
- url: https://docs.oracle.com/javase/tutorial/java/javaOO/methodreferences.html
  title: Method References, The Java Tutorials
  locator: the introduction; Kinds of Method References
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: Array.prototype.map(), MDN Web Docs
  locator: Using parseInt() with map()
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/bind
  title: Function.prototype.bind(), MDN Web Docs
  locator: the introduction; Creating a bound function
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/function/Function.html
  title: Interface Function, Java SE 25 API
  locator: andThen
- url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/delegates/delegates-with-named-vs-anonymous-methods
  title: Delegates with Named vs. Anonymous Methods, C# Programming Guide
  locator: the introduction
- url: https://go.dev/ref/spec#Method_expressions
  title: The Go Programming Language Specification
  locator: Method expressions
- url: https://en.cppreference.com/w/cpp/utility/functional/bind_front.html
  title: std::bind_front, std::bind_back, C++ reference, cppreference.com
  locator: the description
- url: https://learn.microsoft.com/en-us/dotnet/fsharp/language-reference/functions/
  title: Functions, F# language reference
  locator: Function Composition and Pipelining
- url: https://www.gnu.org/software/bash/manual/html_node/Pipelines.html
  title: Pipelines, Bash Reference Manual
  locator: 3.2.3 Pipelines
---

## Purpose

In point-free style, a definition names the functions that make up a function, but not the value that the function gets. Wikipedia describes it as a paradigm "in which function definitions do not identify the arguments"[^1] on which they operate. "Instead the definitions merely compose other functions, among which are combinators that manipulate the arguments."[^1]

The HaskellWiki compares two definitions of the same function:

- `sum = foldr (+) 0` is point-free.
- ``sum' xs = foldr (+) 0 xs`` names the argument `xs`.

"These functions perform the same operation, however, the former is more compact, and is considered cleaner."[^2] The style "helps the writer (and reader) think about composing functions (high level), rather than shuffling data (low level)."[^2]

The points are the values, not the dot of composition. "The term originated in topology, a branch of mathematics which works with spaces composed of points, and functions between those spaces."[^2] "In Haskell, our 'space' is some type, and 'points' are values."[^2]

## Steps

1. **Remove a wrapper.** When a function only passes its argument to one other function, use that function. In the lambda calculus, η-conversion "converts between λx.fx and f whenever x does not appear free in f."[^3] So `lambda s: str.upper(s)` becomes `str.upper`, and `|i| i.to_string()` becomes `ToString::to_string`[^4].
2. **Fix the first arguments.** Give a function some of its arguments now and the rest later. `foldr (+) 0` gives `foldr` the function `(+)` and the start value `0`, and the list comes later[^2]. In Python, `functools.partial` returns an object that "when called will behave like func called with the positional arguments args and keyword arguments keywords."[^5]
3. **Join the steps with composition.** The HaskellWiki says that "it is clearer to write let fn = f . g . h than to write let fn x = f (g (h x))."[^2] See [[function-composition]].
4. **Use a library function in place of a small lambda.** Python `operator.itemgetter` will "Return a callable object that fetches item from its operand using the operand’s __getitem__() method."[^6] Java method references "are compact, easy-to-read lambda expressions for methods that already have a name."[^7]
5. **Stop when the next step only moves arguments.** When a definition needs `flip` or other [[combinator|combinators]] to put the arguments in order, a named argument is clearer.

## Example

The function `total_price` adds the prices of a list of items. The first version names the list. The second version is point-free. It uses the `compose` of the Wikipedia article[^1], which applies the functions from left to right.

```python run id=python-total-price
from functools import partial, reduce
from operator import itemgetter


def compose(*functions):
    return partial(reduce, lambda x, f: f(x), functions)


def total_price_pointful(items):
    return sum(map(lambda item: item[1], items))


total_price = compose(partial(map, itemgetter(1)), sum)

items = [("pen", 3), ("book", 12), ("bag", 25)]
print(total_price_pointful(items))
print(total_price(items))
print(list(map(str.upper, ["red", "green"])))
```

```output for=python-total-price
40
40
['RED', 'GREEN']
```

`partial(map, itemgetter(1))` is step 2 and step 4: it fixes the first argument of `map`, and `itemgetter(1)` replaces the lambda. `compose` is step 3. `str.upper` in the last line is step 1.

## When to use

Use the style for short pipelines of named steps, such as a sort key, a `map` over a list or a chain of transformations. A function name in place of a lambda is often the clearest form. Unix pipes use the same idea: "Unix scripting uses the paradigm with pipes."[^1]

Do not use it when the definition needs [[combinator|combinators]] only to move arguments, or when a name for the argument tells the reader what the value is. "The lack of argument naming gives point-free style a reputation of being needlessly obscure"[^1].

## Pitfalls

- **Extra arguments.** A function in place of a lambda gets all the arguments that the caller gives. JavaScript `map` gives three. MDN shows a call that maps `parseInt` over three strings of digits: "the actual result is [1, NaN, NaN]", because "Array.prototype.map passes 3 arguments: the element, the index, and the array."[^8] `parseInt` reads the index as the radix:

```javascript wrong id=javascript-map-parseint
console.log(["1", "2", "3"].map(parseInt));
console.log(["1", "2", "3"].map((s) => parseInt(s, 10)));
```

```output for=javascript-map-parseint
[ 1, NaN, NaN ]
[ 1, 2, 3 ]
```

- **A method without its object.** In JavaScript, `obj.method` used as a value does not keep `obj`. MDN says: "A common mistake for new JavaScript programmers is to extract a method from an object, then to later call that function and expect it to use the original object as its this"[^9]. Use `bind`, which "creates a new function that, when called, calls this function with its this keyword set to the provided value"[^9]:

```javascript wrong id=javascript-detached-method
class Counter {
  constructor() {
    this.count = 0;
  }
  add() {
    this.count += 1;
  }
}

const counter = new Counter();
try {
  [1, 2, 3].forEach(counter.add);
} catch (e) {
  console.log(e.name);
}
[1, 2, 3].forEach(counter.add.bind(counter));
console.log(counter.count);
```

```output for=javascript-detached-method
TypeError
3
```

- **Definitions that nobody can read.** The tool that makes Haskell code point-free shows the limit. The HaskellWiki says: "It can, of course, get out of hand"[^2]. For `\a b c -> a*b+2+c`, the tool gives `((+) .) . flip flip 2 . ((+) .) . (*)`[^2].
- **Composition order.** `f . g . h` in Haskell runs `h` first, because it is the same as `f (g (h x))`[^2]. The `compose` above runs the functions from left to right. Java has both orders: `andThen` "Returns a composed function that first applies this function to its input, and then applies the after function to the result."[^10] Check the order of each helper.

## Tools that do it

| Language | A function by name | Fix the first arguments | Compose |
|---|---|---|---|
| Python | `str.upper`, `operator.itemgetter` | `functools.partial` | a helper such as `compose` |
| JavaScript, TypeScript | a function name, `method.bind(obj)` | `bind` with arguments | a helper or a chain of calls |
| Java | method references, `String::length` | a lambda | `Function.andThen`, `Function.compose` |
| C# | a method group, as in `obj.DoWork` | a lambda | a lambda |
| C++ | a function name | `std::bind_front` | a lambda |
| Rust | a path, as in `ToString::to_string` | a closure | a closure |
| Go | a method expression `T.M` or a method value `x.M` | a function literal | a function literal |
| F# | a function name | partial application | `>>` and `<<` |
| Bash | a command name | the arguments of the command | a pipeline |

Notes on the table:

- In C#, "When you instantiate a delegate by using a named method, the method is passed as a parameter"[^11].
- In Go, "T.M is a function that is callable as a regular function with the same arguments as M prefixed by an additional argument that is the receiver of the method."[^12]
- In C++, `std::bind_front` makes a wrapper with the first arguments of the target "bound to args"[^13].
- In F#, "The composition operator >> takes two functions and returns a function"[^14].
- In Bash, "The output of each command in the pipeline is connected via a pipe to the input of the next command."[^15]
- For Haskell, a tool converts expressions to point-free form. "A stand-alone command-line version is available at HackageDB (package pointfree)."[^2]

## References
