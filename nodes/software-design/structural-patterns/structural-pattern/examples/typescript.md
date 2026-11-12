```typescript run id=wrappers
interface Thermometer {
  read(): number;
}

class Sensor implements Thermometer {
  reads = 0;
  read(): number {
    this.reads += 1;
    return 20;
  }
}

class FahrenheitAdapter {
  private readonly sensor: Thermometer;
  constructor(sensor: Thermometer) {
    this.sensor = sensor;
  }
  fahrenheit(): number {
    return Math.floor((this.sensor.read() * 9) / 5) + 32;
  }
}

class Calibrated implements Thermometer {
  private readonly sensor: Thermometer;
  private readonly offset: number;
  constructor(sensor: Thermometer, offset: number) {
    this.sensor = sensor;
    this.offset = offset;
  }
  read(): number {
    return this.sensor.read() + this.offset;
  }
}

class CachingProxy implements Thermometer {
  private readonly sensor: Thermometer;
  private value: number | undefined;
  constructor(sensor: Thermometer) {
    this.sensor = sensor;
  }
  read(): number {
    this.value ??= this.sensor.read();
    return this.value;
  }
}

function show(t: Thermometer): number {
  return t.read(); // the client knows only the interface
}

console.log(`adapter: ${new FahrenheitAdapter(new Sensor()).fahrenheit()} F`);
console.log(`decorator: ${show(new Calibrated(new Sensor(), 1))}`);
const real = new Sensor();
const proxy = new CachingProxy(real);
console.log(`proxy: ${show(proxy)} ${show(proxy)}, real reads: ${real.reads}`);
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. An interface names what the decorator and the proxy share with the sensor, and the compiler checks that each wrapper implements it.
