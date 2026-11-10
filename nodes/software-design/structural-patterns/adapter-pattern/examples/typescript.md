```typescript run id=thermometer
interface Thermometer {
  celsius(): number;
}

function report(name: string, sensor: Thermometer): void {
  console.log(`${name}: ${sensor.celsius().toFixed(1)} C`);
}

class RoomSensor implements Thermometer {
  private readonly value: number;

  constructor(value: number) {
    this.value = value;
  }

  celsius(): number {
    return this.value;
  }
}

// The vendor class. The program cannot change it.
class LegacyThermometer {
  private readonly tenths: number;

  constructor(tenths: number) {
    this.tenths = tenths;
  }

  fahrenheitTenths(): number {
    return this.tenths;
  }
}

class LegacyAdapter implements Thermometer {
  private readonly legacy: LegacyThermometer;

  constructor(legacy: LegacyThermometer) {
    this.legacy = legacy;
  }

  celsius(): number {
    return ((this.legacy.fahrenheitTenths() / 10 - 32) * 5) / 9;
  }
}

report("lab", new RoomSensor(21.5));
report("patient", new LegacyAdapter(new LegacyThermometer(986)));

// Structural typing: an object literal with the right method is a Thermometer.
const hall = new LegacyThermometer(704);
report("hall", { celsius: () => ((hall.fahrenheitTenths() / 10 - 32) * 5) / 9 });
```

```output
lab: 21.5 C
patient: 37.0 C
hall: 21.3 C
```

- Verdict: idiom. The type checker compares members, not names, so `implements Thermometer` is a check and an object literal works as the third adapter.
- `report(new LegacyThermometer(704))` without an adapter is a compile error, because the class has no `celsius` member.
- The fields are assigned in the constructor body. Node runs this file by removing the types, and that mode does not accept parameter properties.
