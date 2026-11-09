```kotlin run id=counter
fun makeCounter(): () -> Int {
    var count = 0
    return { ++count }
}

fun main() {
    val first = makeCounter()
    val second = makeCounter()
    println("${first()} ${first()} ${first()}")
    println(second())
}
```

```output for=counter
1 2 3
1
```

- Verdict: native. Lambda expressions, anonymous functions, local functions and object expressions can use the variables of their outer scope.
- Unlike Java, Kotlin lets the lambda change a captured `var`.
- The type `() -> Int` is a function type. The last expression of a lambda is its result.
