```kotlin run timeout=30 id=hof
fun twice(f: (Int) -> Int, x: Int): Int = f(f(x))

fun <A, B, C> compose(f: (B) -> C, g: (A) -> B): (A) -> C = { x -> f(g(x)) }

fun main() {
    val words = listOf("pear", "fig", "banana")
    println(twice({ n -> n * 3 }, 2))
    println(words.map(String::length))
    println(words.filter { it.length > 3 })
    println(listOf(1, 2, 3, 4).fold(0) { total, n -> total + n })
    println(words.sortedBy { it.length })
    val addThenDouble = compose<Int, Int, Int>({ it * 2 }, { it + 1 })
    println(addThenDouble(5))
}
```

```output for=hof
18
[4, 3, 6]
[pear, banana]
10
[fig, pear, banana]
12
```

- Verdict: native. A function type such as `(Int) -> Int` is a type. Lambda expressions, anonymous functions and callable references such as `String::length` are values of function types.
- When the last parameter is a function, the lambda can go outside the parentheses, as in `filter { ... }`. A lambda with one parameter can call it `it`.
- The collection functions `map`, `filter` and `fold` are `inline`, so the compiler copies the lambda into the call site and makes no function object.
- On the JVM, a function value that is not inlined is an object, and a call of it is a virtual call.
