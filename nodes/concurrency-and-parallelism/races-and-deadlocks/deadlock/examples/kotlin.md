```kotlin run id=backoff timeout=30
import java.util.concurrent.TimeUnit
import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.thread
import kotlin.random.Random

class Account(var balance: Int) {
    val lock = ReentrantLock()
}

fun transfer(from: Account, to: Account, amount: Int) {
    while (true) {
        from.lock.lock()
        try {
            if (to.lock.tryLock(1, TimeUnit.MILLISECONDS)) {
                try {
                    from.balance -= amount
                    to.balance += amount
                    return
                } finally {
                    to.lock.unlock()
                }
            }
        } finally {
            from.lock.unlock()
        }
        Thread.sleep(Random.nextLong(0, 2))
    }
}

fun main() {
    val a = Account(1000)
    val b = Account(1000)
    val workers = listOf(
        thread { repeat(200) { transfer(a, b, 1) } },
        thread { repeat(200) { transfer(b, a, 2) } },
    )
    workers.forEach { it.join() }
    println("a: ${a.balance}, b: ${b.balance}, total: ${a.balance + b.balance}")
}
```

```output for=backoff
a: 1200, b: 800, total: 2000
```

- Verdict: stdlib. Kotlin on the JVM uses the JDK locks and threads. `kotlin.concurrent.thread` starts a thread.
- The two threads lock in opposite orders, so a deadlock is possible. When `tryLock` fails, the thread releases its first lock, sleeps for a random time, and starts again. This denies the no preemption condition.
- The random sleep breaks the rhythm of the retries. Two threads that retry at the same time again and again would livelock.
