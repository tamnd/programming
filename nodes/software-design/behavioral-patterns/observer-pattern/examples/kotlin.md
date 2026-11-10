```kotlin run timeout=30 id=sensor
import kotlin.properties.Delegates

class Sensor {
    private val observers = mutableListOf<(Double) -> Unit>()

    var value: Double by Delegates.observable(0.0) { _, _, new ->
        for (observer in observers.toList()) {
            observer(new)
        }
    }

    fun subscribe(observer: (Double) -> Unit): () -> Unit {
        observers += observer
        return { observers -= observer }
    }
}

fun main() {
    val sensor = Sensor()
    val removeDisplay = sensor.subscribe { println("display: %.1f".format(it)) }
    sensor.subscribe { if (it > 30) println("alarm: %.1f is above 30".format(it)) }
    sensor.value = 21.5
    sensor.value = 30.5
    removeDisplay()
    sensor.value = 31.0
}
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: stdlib. `Delegates.observable` in `kotlin.properties` calls a function after each assignment to a property, with the property, the old value and the assigned value.
- The delegate has one callback. The list of observers is a list of function values, as in Python.
- `subscribe` returns a function that removes the observer, and the loop runs over a copy of the list.
- `SharedFlow` and `StateFlow` from the kotlinx.coroutines library are the asynchronous form. They are not part of the Kotlin standard library.
