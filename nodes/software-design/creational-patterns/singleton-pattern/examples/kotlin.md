```kotlin run timeout=30 id=settings
import kotlin.concurrent.thread

object Settings {
    init {
        println("creating the settings")
    }

    @Volatile
    var level = "info"
}

fun main() {
    val first = Settings
    println("main: ${first.level}")
    var seen: Settings? = null
    thread {
        val settings = Settings
        println("worker sets debug")
        settings.level = "debug"
        seen = settings
    }.join()
    println("main: ${Settings.level}")
    println("same instance: ${seen === first}")
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: native. An `object` declaration is a class with exactly one instance. Its initialization is thread-safe and runs on first access.
- The name `Settings` is the instance, so code uses it with no accessor call.
- An `object` can implement interfaces. Code that takes the interface as a parameter can get a fake in tests, which is the usual way to soften the global access.
- `@Volatile` makes the change from the worker thread visible to `main`.
