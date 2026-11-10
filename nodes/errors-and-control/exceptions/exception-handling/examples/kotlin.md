```kotlin run id=ports
class ConfigException(message: String, cause: Throwable? = null) : Exception(message, cause)

fun parsePort(text: String): Int {
    val port = try {
        text.toInt()
    } catch (e: NumberFormatException) {
        throw ConfigException("not a number: $text", e)
    }
    if (port !in 1..65535) throw ConfigException("out of range: $port")
    return port
}

fun main() {
    var checked = 0
    for (text in listOf("8080", "70000", "abc")) {
        try {
            println("port ${parsePort(text)}")
        } catch (e: ConfigException) {
            println("error: ${e.message} (cause: ${e.cause?.javaClass?.simpleName ?: "none"})")
        } finally {
            checked++
        }
    }
    println("checked $checked")
}
```

```output
port 8080
error: out of range: 70000 (cause: none)
error: not a number: abc (cause: NumberFormatException)
checked 3
```

- Verdict: native. The exception classes are those of Java, and `throw` and `try` with `catch` and `finally` work in the same way.
- Kotlin has no checked exceptions. `parsePort` throws a subclass of `Exception` but needs no declaration. `@Throws` adds the declaration for Java callers.
- `try` is an expression. Its value is the last expression of the `try` block or of the `catch` block that ran.
- `throw` is an expression of type `Nothing`, so it can stand where a value is expected.
- `runCatching` returns a `Result` in place of an exception, for code that prefers values.
