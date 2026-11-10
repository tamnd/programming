```kotlin run id=types timeout=30
fun describe(value: Any): String = when (value) {
    is Int -> "Int ${value + 1}"
    is String -> "String of length ${value.length}"
    else -> value.javaClass.simpleName
}

fun main() {
    val count = 41
    val values: List<Any> = listOf(count, "abc", 2.5)
    for (value in values) {
        println(describe(value))
    }
    val text: String? = values[0] as? String
    println(text)
    try {
        val s = values[0] as String
        println(s)
    } catch (e: ClassCastException) {
        println("ClassCastException")
    }
}
```

```output
Int 42
String of length 3
Double
null
ClassCastException
```

- Verdict: native. Kotlin is statically typed. `val count = 41` infers `Int`.
- After a test with `is`, the compiler treats the value as the tested type in that branch. This is a smart cast.
- `as?` gives `null` when the cast fails. `as` throws `ClassCastException`.
