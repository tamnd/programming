```javascript run id=thermometer
function report(name, sensor) {
  console.log(`${name}: ${sensor.celsius().toFixed(1)} C`);
}

class RoomSensor {
  #value;

  constructor(value) {
    this.#value = value;
  }

  celsius() {
    return this.#value;
  }
}

// The vendor class. The program cannot change it.
class LegacyThermometer {
  #tenths;

  constructor(tenths) {
    this.#tenths = tenths;
  }

  fahrenheitTenths() {
    return this.#tenths;
  }
}

class LegacyAdapter {
  #legacy;

  constructor(legacy) {
    this.#legacy = legacy;
  }

  celsius() {
    return ((this.#legacy.fahrenheitTenths() / 10 - 32) * 5) / 9;
  }
}

report("lab", new RoomSensor(21.5));
report("patient", new LegacyAdapter(new LegacyThermometer(986)));
report("hall", new LegacyAdapter(new LegacyThermometer(704)));
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. The client calls `celsius()` on any object, so the adapter is any object with that method.
- An object literal `{ celsius: () => convert(legacy) }` is also an adapter, with no class.
- Node `util.promisify` adapts a function with an error-first callback to a function that returns a promise.
