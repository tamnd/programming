```java run id=collisions
interface Body {
    String name();
    String collide(Body other);
    String collideWithShip(Ship ship);
    String collideWithAsteroid(Asteroid asteroid);
}

class Ship implements Body {
    public String name() { return "ship"; }
    public String collide(Body other) { return other.collideWithShip(this); }
    public String collideWithShip(Ship ship) { return "ships bounce"; }
    public String collideWithAsteroid(Asteroid asteroid) { return "ship takes damage"; }
}

class Asteroid implements Body {
    public String name() { return "asteroid"; }
    public String collide(Body other) { return other.collideWithAsteroid(this); }
    public String collideWithShip(Ship ship) { return "ship takes damage"; }
    public String collideWithAsteroid(Asteroid asteroid) { return "asteroids break apart"; }
}

public class Main {
    public static void main(String[] args) {
        Body[][] pairs = {
            {new Ship(), new Asteroid()},
            {new Asteroid(), new Ship()},
            {new Asteroid(), new Asteroid()},
            {new Ship(), new Ship()},
        };
        for (Body[] p : pairs) {
            System.out.println(p[0].name() + " + " + p[1].name() + ": " + p[0].collide(p[1]));
        }
    }
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. The first call dispatches on the receiver, and the callback dispatches on the argument. The callbacks have different names, so no overload is chosen from a static type.
