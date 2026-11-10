```kotlin run timeout=30 id=sums
import kotlin.concurrent.thread

fun main() {
    val results = LongArray(4)
    val workers = (0 until 4).map { i ->
        thread(name = "worker-$i") {
            results[i] = (i * 250L until (i + 1) * 250L).sum()
        }
    }
    workers.forEach { it.join() }
    println(results.toList())
    println("names: ${workers.map { it.name }}")
    println("daemon: ${workers[0].isDaemon}")

    val virtual = Thread.ofVirtual().start { results[0] = 0 }
    virtual.join()
    println("virtual: ${virtual.isVirtual}, first result now ${results[0]}")
}
```

```output for=sums
[31125, 93625, 156125, 218625]
names: [worker-0, worker-1, worker-2, worker-3]
daemon: false
virtual: true, first result now 0
```

- Verdict: stdlib. Kotlin on the JVM uses `java.lang.Thread`. The standard library function `kotlin.concurrent.thread` makes a thread and starts it by default.
- `thread` takes the name, the daemon flag and the priority as named arguments, with `isDaemon = false` by default.
- Virtual threads come from the JDK, through `Thread.ofVirtual()`, as in the Java example.
- The library `kotlinx.coroutines` adds coroutines, which are not threads (see [[async-await]]).
