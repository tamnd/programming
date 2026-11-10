```kotlin run timeout=30 id=thermometer
import java.util.Locale

fun interface Thermometer {
    fun celsius(): Double
}

fun report(name: String, sensor: Thermometer) {
    println(String.format(Locale.ROOT, "%s: %.1f C", name, sensor.celsius()))
}

class RoomSensor(private val value: Double) : Thermometer {
    override fun celsius(): Double = value
}

// The vendor class. The program cannot change it.
class LegacyThermometer(private val tenths: Int) {
    fun fahrenheitTenths(): Int = tenths
}

fun LegacyThermometer.asThermometer(): Thermometer =
    Thermometer { (fahrenheitTenths() / 10.0 - 32) * 5 / 9 }

fun main() {
    report("lab", RoomSensor(21.5))
    report("patient", LegacyThermometer(986).asThermometer())
    report("hall", LegacyThermometer(704).asThermometer())
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. `Thermometer` is a `fun interface`, so a lambda converts to it with a SAM conversion. The extension function `asThermometer` returns that lambda as the adapter.
- The extension function adds no member to `LegacyThermometer`. It is a static function that the compiler lets the code call with the dot syntax.
