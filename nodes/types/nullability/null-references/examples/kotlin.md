```kotlin run timeout=30 id=nullable
fun domain(email: String?): String {
    if (email == null) return "none"
    return email.substringAfter('@')
}

fun main() {
    val emails: List<String?> = listOf("ada@example.org", null)
    for (e in emails) {
        println(domain(e))
    }

    val name: String? = emails[1]
    println(name?.length)
    println(name?.length ?: 0)
    println(emails.filterNotNull())
}
```

```output
example.org
none
null
0
[ada@example.org]
```

- Verdict: native. `String` cannot hold `null`, and `String?` can. The compiler rejects `null` for a non-nullable type, and it rejects a member access such as `email.length` on a `String?`.
- After `email == null` returns, the compiler smart casts `email` to `String`, so `substringAfter` needs no operator.
- `?.` returns `null` when the receiver is `null`. The Elvis operator `?:` gives a default value. `!!` converts a value to a non-nullable type and throws `NullPointerException` when it is `null` (see Pitfalls on the concept page).
- `filterNotNull` turns a `List<String?>` into a `List<String>`.
- Java types are platform types, so a value from Java can still be `null` at run time.
