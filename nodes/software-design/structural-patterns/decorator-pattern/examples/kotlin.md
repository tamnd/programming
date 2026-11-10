```kotlin run timeout=30 id=repository
interface Repository {
    fun get(key: String): String
    fun name(): String
}

class Store : Repository {
    override fun get(key: String): String {
        println("store: load $key")
        return "value-$key"
    }

    override fun name(): String = "store"
}

class Logged(private val inner: Repository) : Repository by inner {
    override fun get(key: String): String {
        println("log: get $key")
        return inner.get(key)
    }
}

class Cached(private val inner: Repository) : Repository by inner {
    private val cache = mutableMapOf<String, String>()

    override fun get(key: String): String = cache.getOrPut(key) { inner.get(key) }
}

fun main() {
    val repo: Repository = Logged(Cached(Store()))
    for (key in listOf("a", "a", "b")) {
        println(repo.get(key))
    }
    println("name: ${repo.name()}")
}
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: native. `Repository by inner` makes the compiler write a forwarding method for each member of the interface. The decorators override only `get`, and `name` still works.
- The delegation is to the object passed to the constructor, so the chain is still built at run time.
- A member of the delegate that calls another member calls its own version, not the override in the decorator. The Kotlin documentation states this rule, and the node page shows the effect in Python.
