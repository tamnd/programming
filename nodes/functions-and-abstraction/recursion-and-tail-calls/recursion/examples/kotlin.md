```kotlin run timeout=30 id=tailrec
tailrec fun countDown(n: Long, steps: Long = 0): Long =
    if (n == 0L) steps else countDown(n - 1, steps + 1)

fun depth(n: Int): Int = if (n == 0) 0 else 1 + depth(n - 1)

fun main() {
    println(countDown(100_000_000))
    println(depth(1000))
    try {
        depth(100_000_000)
    } catch (error: StackOverflowError) {
        println("caught StackOverflowError")
    }
}
```

```output for=tailrec
100000000
1000
caught StackOverflowError
```

- Verdict: native. A function can call itself, and on the JVM each call takes a frame, as in Java.
- The modifier `tailrec` asks the compiler to turn self recursion into a loop. `countDown` counts to 100,000,000 with no deep stack.
- `tailrec` works only when the recursive call is the last operation. It does not apply to a call with more code after it, a call inside `try`, `catch` or `finally`, or an open function. `depth` is not a tail call, because the addition runs after the call returns.
