```go run id=wrappers
package main

import "fmt"

type Thermometer interface{ Read() int }

type Sensor struct{ reads int }

func (s *Sensor) Read() int { s.reads++; return 20 }
func (s *Sensor) Unit() string { return "C" }

type FahrenheitAdapter struct{ sensor Thermometer }

func (a FahrenheitAdapter) Fahrenheit() int { return a.sensor.Read()*9/5 + 32 }

// Calibrated embeds *Sensor: Unit is promoted, and Read is replaced.
type Calibrated struct {
	*Sensor
	offset int
}

func (c Calibrated) Read() int { return c.Sensor.Read() + c.offset }

type CachingProxy struct {
	sensor Thermometer
	value  *int
}

func (p *CachingProxy) Read() int {
	if p.value == nil {
		v := p.sensor.Read()
		p.value = &v
	}
	return *p.value
}

func main() {
	fmt.Printf("adapter: %d F\n", FahrenheitAdapter{&Sensor{}}.Fahrenheit())
	c := Calibrated{&Sensor{}, 1}
	_ = c.Unit() // promoted from the embedded *Sensor
	fmt.Printf("decorator: %d\n", c.Read())
	real := &Sensor{}
	var proxy Thermometer = &CachingProxy{sensor: real}
	first, second := proxy.Read(), proxy.Read()
	fmt.Printf("proxy: %d %d, real reads: %d\n", first, second, real.reads)
}
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: native. Struct embedding promotes the methods of the embedded field, so the decorator declares only the method that it changes. The interface `Thermometer` is satisfied without a declaration.
