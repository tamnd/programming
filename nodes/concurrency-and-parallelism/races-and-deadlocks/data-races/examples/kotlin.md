```kotlin run id=atomic timeout=30
import java.util.concurrent.atomic.AtomicInteger
import kotlin.concurrent.thread

fun main() {
    val count = AtomicInteger()
    val workers = List(2) {
        thread {
            repeat(100_000) { count.incrementAndGet() }
        }
    }
    workers.forEach { it.join() }
    println(count.get())
}
```

```output for=atomic
200000
```

- Verdict: stdlib. On the JVM, Kotlin uses the JDK class `java.util.concurrent.atomic.AtomicInteger`, so the rules of the Java memory model apply.
- `kotlin.concurrent.thread` creates and starts a JVM thread. `join` orders the work of each thread before the final read.
- A plain `var count` with `count++` in both threads has a data race and can lose updates, as in Java. The Kotlin compiler does not report it.
