```kotlin run id=largest timeout=30
fun <T : Comparable<T>> largest(items: List<T>): T {
    var result = items[0]
    for (item in items) {
        if (item > result) result = item
    }
    return result
}

inline fun <reified T> allOf(items: List<*>): Boolean = items.all { it is T }

fun main() {
    println(largest(listOf(3, 9, 4)))
    println(largest(listOf("pear", "apple", "plum")))
    println(allOf<String>(listOf("a", "b")))
    println(allOf<String>(listOf(1, 2)))
    val numbers: List<Number> = listOf<Int>(1, 2)
    println(numbers.size)
}
```

```output for=largest
9
plum
true
false
2
```

- Verdict: native. Classes, interfaces and functions take type parameters. `T : Comparable<T>` is an upper bound.
- `List<out E>` is covariant at its declaration, so a `List<Int>` is a `List<Number>`. A `MutableList<E>` is invariant.
- On the JVM, the compiler erases type arguments, so `value is T` is an error in a normal function. An `inline` function with a `reified` type parameter can make the check, because the compiler copies its body into each call with the real type.
