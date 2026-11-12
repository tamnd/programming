```java run id=breeds
import java.util.List;

public class Main {
    record Breed(String name, int health, String attack) {
        Monster newMonster() {
            return new Monster(this);
        }
    }

    static final class Monster {
        private final Breed breed;
        private int health; // each monster has its own health

        Monster(Breed breed) {
            this.breed = breed;
            this.health = breed.health();
        }

        String describe() {
            return breed.name() + ": " + health + " health, attacks with " + breed.attack();
        }
    }

    public static void main(String[] args) {
        // New kinds are data, for example from a file.
        List<Breed> breeds = List.of(
            new Breed("goblin", 20, "a dagger"),
            new Breed("dragon", 200, "fire"),
            new Breed("wyvern", 120, "its tail"));
        for (Breed b : breeds) {
            System.out.println(b.newMonster().describe());
        }
    }
}
```

```output for=breeds
goblin: 20 health, attacks with a dagger
dragon: 200 health, attacks with fire
wyvern: 120 health, attacks with its tail
```

- Verdict: idiom. A record with one instance for each kind.
