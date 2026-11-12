```java run id=twin-ball
abstract class Item {
    final String name;
    Item(String name) { this.name = name; }
    abstract int position();
    void show() { System.out.println(name + " at " + position()); }
}

abstract class Ticker {
    int count;
    abstract void onTick();
    void tick() { count++; onTick(); }
}

class BallItem extends Item {
    int x;
    final BallTicker twin;
    BallItem(String name) { super(name); twin = new BallTicker(this); }
    int position() { return x; }
    void move() { x += 2; }
}

class BallTicker extends Ticker {
    final BallItem twin;
    BallTicker(BallItem twin) { this.twin = twin; }
    void onTick() { twin.move(); }
}

public class Main {
    public static void main(String[] args) {
        BallItem ball = new BallItem("ball");
        ball.show();
        for (int i = 0; i < 2; i++) {
            ball.twin.tick();
            ball.show();
        }
        System.out.println("ticks: " + ball.twin.count);
    }
}
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. The `extends` clause names one direct superclass, so Twin is the form that Mössenböck gives for Java. The two classes share a package, so they can use each other's package-private fields.
