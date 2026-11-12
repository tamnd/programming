```java run id=wrappers
interface Thermometer { int read(); }

class Sensor implements Thermometer {
    int reads;
    public int read() { reads++; return 20; }
}

class FahrenheitAdapter {
    private final Thermometer sensor;
    FahrenheitAdapter(Thermometer sensor) { this.sensor = sensor; }
    int fahrenheit() { return sensor.read() * 9 / 5 + 32; }
}

class Calibrated implements Thermometer {
    private final Thermometer sensor;
    private final int offset;
    Calibrated(Thermometer sensor, int offset) { this.sensor = sensor; this.offset = offset; }
    public int read() { return sensor.read() + offset; }
}

class CachingProxy implements Thermometer {
    private final Thermometer sensor;
    private Integer value;
    CachingProxy(Thermometer sensor) { this.sensor = sensor; }
    public int read() {
        if (value == null) value = sensor.read();
        return value;
    }
}

public class Main {
    public static void main(String[] args) {
        System.out.println("adapter: " + new FahrenheitAdapter(new Sensor()).fahrenheit() + " F");
        System.out.println("decorator: " + new Calibrated(new Sensor(), 1).read());
        Sensor real = new Sensor();
        Thermometer proxy = new CachingProxy(real);
        System.out.println("proxy: " + proxy.read() + " " + proxy.read() + ", real reads: " + real.reads);
    }
}
```

```output
adapter: 68 F
decorator: 21
proxy: 20 20, real reads: 1
```

- Verdict: idiom. Each wrapper is a class that holds the sensor; the decorator and the proxy implement the same interface. `java.lang.reflect.Proxy` can generate a proxy class at run time for a list of interfaces.
