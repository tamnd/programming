```java run id=car
interface StarterPart { String crank(); }

class Starter implements StarterPart {
    public String crank() { return "crank the engine"; }
}

class NewStarter implements StarterPart {
    public String crank() { return "crank the engine with the new starter"; }
}

class Car {
    // The parts are private; the Engine and Wheels classes are private too.
    private static final class Engine { String fire() { return "fire the cylinders"; } }
    private static final class Wheels { String turn() { return "turn the wheels"; } }

    private StarterPart starter = new Starter();
    private final Engine engine = new Engine();
    private final Wheels wheels = new Wheels();

    String drive() {
        return String.join(", ", starter.crank(), engine.fire(), wheels.turn());
    }

    void replaceStarter(StarterPart starter) { this.starter = starter; }
}

public class Main {
    public static void main(String[] args) {
        Car car = new Car();
        System.out.println("drive: " + car.drive());
        car.replaceStarter(new NewStarter());
        System.out.println("drive: " + car.drive());
    }
}
```

```output
drive: crank the engine, fire the cylinders, turn the wheels
drive: crank the engine with the new starter, fire the cylinders, turn the wheels
```

- Verdict: idiom. Private nested classes make the parts invisible outside the whole, as `ThreadPoolExecutor` does with its `Worker` class.
