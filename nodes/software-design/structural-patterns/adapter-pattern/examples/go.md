```go run id=thermometer
package main

import "fmt"

type Thermometer interface {
	Celsius() float64
}

func report(name string, sensor Thermometer) {
	fmt.Printf("%s: %.1f C\n", name, sensor.Celsius())
}

type RoomSensor float64

func (r RoomSensor) Celsius() float64 { return float64(r) }

// LegacyThermometer is the vendor type. The program cannot change it.
type LegacyThermometer struct{ tenths int }

func (l LegacyThermometer) FahrenheitTenths() int { return l.tenths }

// CelsiusFunc adapts an ordinary function to Thermometer, as http.HandlerFunc does for handlers.
type CelsiusFunc func() float64

func (f CelsiusFunc) Celsius() float64 { return f() }

func adaptLegacy(l LegacyThermometer) Thermometer {
	return CelsiusFunc(func() float64 {
		return (float64(l.FahrenheitTenths())/10 - 32) * 5 / 9
	})
}

func main() {
	report("lab", RoomSensor(21.5))
	report("patient", adaptLegacy(LegacyThermometer{986}))
	report("hall", adaptLegacy(LegacyThermometer{704}))
}
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. A function type with a method is the adapter. Any function with the right signature becomes a `Thermometer` with a conversion, `CelsiusFunc(f)`.
- `net/http` uses this form. `HandlerFunc(f)` is a `Handler` whose `ServeHTTP` calls `f`.
- Go interfaces are satisfied implicitly. A type from another package that already has a `Celsius() float64` method needs no adapter.
