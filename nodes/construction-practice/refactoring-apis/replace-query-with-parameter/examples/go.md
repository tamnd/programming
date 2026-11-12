```go run id=parameter-not-query
package main

import "fmt"

type Thermostat struct{ CurrentTemperature float64 }

type Plan struct{ Target, MaxStep float64 }

var thermostat = &Thermostat{CurrentTemperature: 19}

func targetTemperatureBefore(p Plan) float64 {
	current := thermostat.CurrentTemperature
	return min(p.Target, current+p.MaxStep)
}

// targetTemperature does not read the package-level thermostat.
func targetTemperature(p Plan, currentTemperature float64) float64 {
	return min(p.Target, currentTemperature+p.MaxStep)
}

func main() {
	plan := Plan{Target: 21, MaxStep: 1}
	if targetTemperature(plan, thermostat.CurrentTemperature) != targetTemperatureBefore(plan) ||
		targetTemperature(plan, 20.5) != 21 || targetTemperature(plan, 15) != 16 {
		panic("mismatch")
	}
	fmt.Println(targetTemperature(plan, thermostat.CurrentTemperature))
}
```

```output
20
```
