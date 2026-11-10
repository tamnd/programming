```kotlin run id=queues timeout=30
import java.util.concurrent.ArrayBlockingQueue
import java.util.concurrent.SynchronousQueue
import kotlin.concurrent.thread

sealed interface Msg
data class Job(val value: Int) : Msg
data object Done : Msg // the end-of-stream marker

fun main() {
    val jobs = ArrayBlockingQueue<Msg>(2)
    val producers = (1..2).map { id ->
        thread {
            for (i in 1..3) jobs.put(Job(id * 10 * i))
            jobs.put(Done)
        }
    }
    var total = 0
    var finished = 0
    while (finished < producers.size) {
        when (val m = jobs.take()) {
            is Job -> total += m.value
            Done -> finished++
        }
    }
    producers.forEach { it.join() }
    println("total: $total")

    val handoff = SynchronousQueue<String>()
    val taker = thread { println("took: ${handoff.take()}") }
    handoff.put("ping") // waits for the taker: a rendezvous
    taker.join()
}
```

```output for=queues
total: 180
took: ping
```

- Verdict: library. The Kotlin standard library has no channel. The library `kotlinx.coroutines` gives `Channel`, with suspending `send` and `receive`, `close`, and iteration with `for` that ends after `close`. Its default capacity is `RENDEZVOUS`.
- With the standard library only, a Kotlin/JVM program uses the JDK queues, as here. A sealed type makes the end marker a separate case, so it cannot clash with a real value.
