```kotlin run id=cycle
import java.lang.ref.WeakReference

class Item {
    var peer: Item? = null
}

fun makeCycle(): WeakReference<Item> {
    val a = Item()
    val b = Item()
    a.peer = b
    b.peer = a
    val probe = WeakReference(a)
    println("alive while referenced: ${probe.get() != null}")
    return probe
}

fun main() {
    val probe = makeCycle()
    System.gc()
    println("collected after gc: ${probe.get() == null}")
}
```

```output
alive while referenced: true
collected after gc: true
```

- Verdict: native. Kotlin/JVM uses the collector of the JVM, so the rules of the Java example apply.
- The cycle is made in a separate function. Its local variables are gone when `System.gc()` runs, so no stack slot keeps `a` alive.
- Kotlin/Native has a collector of its own: a concurrent mark and sweep collector without generations.
