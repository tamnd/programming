```java run id=table-per-class
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // One table for each class, all keyed by the same id.
    static final Map<Long, String> PLAYER = new TreeMap<>();
    static final Map<Long, String> FOOTBALLER = new TreeMap<>();
    static final Map<Long, Integer> CRICKETER = new TreeMap<>();
    static final Map<Long, Integer> BOWLER = new TreeMap<>();
    static long lastId = 0;

    abstract static class Player {
        Long id;
        final String name;

        Player(String name) {
            this.name = name;
        }

        abstract String describe();
    }

    static class Footballer extends Player {
        final String club;

        Footballer(String name, String club) {
            super(name);
            this.club = club;
        }

        String describe() {
            return "Footballer " + name + ", club " + club;
        }
    }

    static class Cricketer extends Player {
        final int runs;

        Cricketer(String name, int runs) {
            super(name);
            this.runs = runs;
        }

        String describe() {
            return "Cricketer " + name + ", runs " + runs;
        }
    }

    static class Bowler extends Cricketer {
        final int wickets;

        Bowler(String name, int runs, int wickets) {
            super(name, runs);
            this.wickets = wickets;
        }

        @Override
        String describe() {
            return "Bowler " + name + ", runs " + runs + ", wickets " + wickets;
        }
    }

    record Loaded(Player player, List<String> tables) {}

    static void insert(Player p) {
        // One row in the table of each class on the path from the root, all with the same key.
        p.id = ++lastId;
        PLAYER.put(p.id, p.name);
        if (p instanceof Footballer f) {
            FOOTBALLER.put(p.id, f.club);
        }
        if (p instanceof Cricketer c) {
            CRICKETER.put(p.id, c.runs);
        }
        if (p instanceof Bowler b) {
            BOWLER.put(p.id, b.wickets);
        }
    }

    static Loaded find(long id) {
        // The root row has no type, so the mapper looks for the key in the tables of the subclasses.
        String name = PLAYER.get(id);
        Player p;
        List<String> used;
        if (FOOTBALLER.containsKey(id)) {
            p = new Footballer(name, FOOTBALLER.get(id));
            used = List.of("player", "footballer");
        } else if (BOWLER.containsKey(id)) {
            p = new Bowler(name, CRICKETER.get(id), BOWLER.get(id));
            used = List.of("player", "cricketer", "bowler");
        } else {
            p = new Cricketer(name, CRICKETER.get(id));
            used = List.of("player", "cricketer");
        }
        p.id = id;
        return new Loaded(p, used);
    }

    static int delete(Player p) {
        int count = 0;
        for (Map<Long, ?> table : List.of(BOWLER, CRICKETER, FOOTBALLER, PLAYER)) { // Children first.
            if (table.remove(p.id) != null) {
                count++;
            }
        }
        return count;
    }

    static String show(Map<Long, ?> table) {
        List<String> parts = new ArrayList<>();
        for (var e : table.entrySet()) {
            parts.add(e.getKey() + " " + e.getValue());
        }
        return String.join(" | ", parts);
    }

    public static void main(String[] args) {
        Player ann = new Footballer("Ann", "Lions");
        Player bo = new Cricketer("Bo", 412);
        Player cy = new Bowler("Cy", 95, 31);
        for (Player p : List.of(ann, bo, cy)) {
            insert(p);
        }
        System.out.println("player: " + show(PLAYER));
        System.out.println("footballer: " + show(FOOTBALLER));
        System.out.println("cricketer: " + show(CRICKETER));
        System.out.println("bowler: " + show(BOWLER));
        for (long id = 1; id <= 3; id++) {
            Loaded l = find(id);
            System.out.println("loaded: " + l.player().describe() + " (tables: " + String.join(", ", l.tables()) + ")");
        }
        List<String> names = new ArrayList<>();
        for (long id : CRICKETER.keySet()) {
            names.add(PLAYER.get(id));
        }
        System.out.println("cricketers: " + String.join(", ", names));
        System.out.println("delete Cy: rows deleted " + delete(cy));
    }
}
```

```output
player: 1 Ann | 2 Bo | 3 Cy
footballer: 1 Lions
cricketer: 2 412 | 3 95
bowler: 3 31
loaded: Footballer Ann, club Lions (tables: player, footballer)
loaded: Cricketer Bo, runs 412 (tables: player, cricketer)
loaded: Bowler Cy, runs 95, wickets 31 (tables: player, cricketer, bowler)
cricketers: Bo, Cy
delete Cy: rows deleted 3
```

- Verdict: idiom. Each table is a `TreeMap` keyed by the shared id, and `insert` uses pattern matching for `instanceof`, so a `Bowler` also gets a cricketer row.
- With JPA or Hibernate, `@Inheritance(strategy = InheritanceType.JOINED)` on the root class gives the same tables, and the provider writes and joins the rows.
