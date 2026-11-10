```kotlin run timeout=30 id=countdown
class Countdown(private val start: Int) : Iterable<Int> {
    override fun iterator(): Iterator<Int> = object : Iterator<Int> {
        private var current = start

        override fun hasNext() = current > 0

        override fun next(): Int {
            if (current <= 0) throw NoSuchElementException()
            return current--
        }
    }
}

fun main() {
    for (n in Countdown(3)) {
        println(n)
    }
    val tens = Countdown(5).asSequence()
        .filter { it % 2 == 1 }
        .map { it * 10 }
        .toList()
    println(tens)
}
```

```output for=countdown
3
2
1
[50, 30, 10]
```

- Verdict: native. A `for` loop works on any value with an `operator fun iterator()` whose result has `next()` and `hasNext()`. Implementing `Iterable<T>` is the usual way.
- The compiler turns a `for` loop over a range or an array into an index loop, with no iterator object.
- Operations on an `Iterable` are eager and make a list after each step. `asSequence()` makes the chain lazy, so each element goes through `filter` and `map` before the next one starts.
- An iterator cannot go back to the start. To iterate again, ask the collection for a fresh iterator.
