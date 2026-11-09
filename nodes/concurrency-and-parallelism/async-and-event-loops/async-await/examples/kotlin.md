```kotlin run id=load
import java.util.PriorityQueue
import kotlin.coroutines.Continuation
import kotlin.coroutines.EmptyCoroutineContext
import kotlin.coroutines.resume
import kotlin.coroutines.startCoroutine
import kotlin.coroutines.suspendCoroutine

// The event loop: suspended coroutines that wait for a time, earliest first.
class Timer(val due: Long, val continuation: Continuation<Unit>)

val timers = PriorityQueue<Timer>(compareBy { it.due })

fun nowMillis() = System.nanoTime() / 1_000_000

suspend fun sleep(ms: Long) = suspendCoroutine { continuation ->
    timers.add(Timer(nowMillis() + ms, continuation))
}

suspend fun load(name: String, ms: Long): String {
    println("start $name")
    sleep(ms)
    println("end $name")
    return name
}

fun main() {
    val results = arrayOfNulls<String>(2)
    val jobs = listOf("slow" to 200L, "fast" to 100L)
    for ((i, job) in jobs.withIndex()) {
        val done = Continuation<String>(EmptyCoroutineContext) { results[i] = it.getOrThrow() }
        suspend { load(job.first, job.second) }.startCoroutine(done)
    }
    while (timers.isNotEmpty()) {
        val next = timers.poll()
        Thread.sleep(maxOf(0, next.due - nowMillis()))
        next.continuation.resume(Unit)
    }
    println(results.joinToString(", "))
}
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. A `suspend fun` can suspend at a call of another suspending function. There is no `await` keyword: a call of a suspending function suspends when it must.
- The standard library gives only the primitives: `Continuation`, `startCoroutine` and `suspendCoroutine`. This example builds a timer queue on them as the event loop.
- Programs usually use the library `kotlinx.coroutines`. It gives `launch`, `async`, `await`, `delay`, dispatchers and structured concurrency with `coroutineScope`.
