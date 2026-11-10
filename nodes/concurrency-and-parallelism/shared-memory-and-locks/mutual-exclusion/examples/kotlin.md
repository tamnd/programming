```kotlin run id=counter timeout=30
import java.util.concurrent.locks.ReentrantLock
import java.util.concurrent.locks.ReentrantReadWriteLock
import kotlin.concurrent.read
import kotlin.concurrent.thread
import kotlin.concurrent.withLock
import kotlin.concurrent.write

class Scoreboard {
    private val lock = ReentrantReadWriteLock()
    private val scores = mutableMapOf<String, Int>()

    fun add(player: String, points: Int) = lock.write {
        scores[player] = (scores[player] ?: 0) + points
    }

    fun total(): Int = lock.read { scores.values.sum() }
}

fun main() {
    val counterLock = ReentrantLock()
    var counter = 0
    val workers = List(4) {
        thread {
            repeat(10_000) {
                counterLock.withLock { counter++ }
            }
        }
    }
    workers.forEach { it.join() }
    println("total: $counter")

    val board = Scoreboard()
    val players = listOf("ada", "grace", "alan").map { name ->
        thread { repeat(100) { board.add(name, 2) } }
    }
    players.forEach { it.join() }
    println("points: ${board.total()}")
}
```

```output
total: 40000
points: 600
```

- Verdict: stdlib. Kotlin on the JVM uses the locks of `java.util.concurrent.locks`. The package `kotlin.concurrent` adds `withLock`, `read`, `write` and `thread`. The function `thread` starts a thread at once and returns it, so `join` can wait for it.
- `withLock` is an inline function. It locks, runs the lambda and unlocks, also when the lambda throws.
- `read` and `write` do the same for the two halves of a `ReentrantReadWriteLock`. Many `total` calls can run at once, but `add` runs alone.
- The lambda changes the local `counter`. The compiler puts the variable in a shared object, so the lock must protect it. The Java `synchronized` modifier has the Kotlin form `@Synchronized`.
