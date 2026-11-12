```javascript run id=wrappers
class Sensor {
  reads = 0;
  read() {
    this.reads += 1;
    return 20;
  }
}

class FahrenheitAdapter {
  constructor(sensor) {
    this.sensor = sensor;
  }
  fahrenheit() {
    return Math.floor((this.sensor.read() * 9) / 5) + 32;
  }
}

class Calibrated {
  constructor(sensor, offset) {
    this.sensor = sensor;
    this.offset = offset;
  }
  read() {
    return this.sensor.read() + this.offset;
  }
}

// The proxy: a Proxy object intercepts the property access "read".
function cachingProxy(sensor) {
  let value;
  return new Proxy(sensor, {
    get(target, prop, receiver) {
      if (prop === "read") {
        return () => (value ??= target.read());
      }
      return Reflect.get(target, prop, receiver);
    },
  });
}

console.log(`adapter: ${new FahrenheitAdapter(new Sensor()).fahrenheit()} F`);
console.log(`decorator: ${new Calibrated(new Sensor(), 1).read()}`);
const real = new Sensor();
const proxy = cachingProxy(real);
console.log(`proxy: ${proxy.read()} ${proxy.read()}, real reads: ${proxy.reads}`);
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: native. The `Proxy` object intercepts operations on another object, so a proxy needs code only for the operations that it changes. `Reflect.get` forwards the rest, here `reads`.
