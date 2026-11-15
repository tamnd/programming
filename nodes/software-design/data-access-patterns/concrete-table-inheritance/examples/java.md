```java run id=table-per-concrete-class
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // One table for each concrete class, with all its fields, the inherited ones too.
    record FootballerRow(String name, String club) {}

    record CricketerRow(String name, int runs) {}

    record BowlerRow(String name, int runs, int wickets) {}

    static final Map<Long, FootballerRow> FOOTBALLER = new TreeMap<>();
    static final Map<Long, CricketerRow> CRICKETER = new TreeMap<>();
    static final Map<Long, BowlerRow> BOWLER = new TreeMap<>();
    static long nextId = 1; // One key source for all the tables.

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

    record Found(Player player, List<String> searched) {}

    record Summary(long id, String name, String table) {}

    static void insert(Player p) {
        p.id = nextId++;
        if (p instanceof Bowler b) {
            BOWLER.put(p.id, new BowlerRow(b.name, b.runs, b.wickets));
        } else if (p instanceof Cricketer c) {
            CRICKETER.put(p.id, new CricketerRow(c.name, c.runs));
        } else {
            Footballer f = (Footballer) p;
            FOOTBALLER.put(p.id, new FootballerRow(f.name, f.club));
        }
    }

    static Found find(long id) {
        // A key alone does not say which table holds the row, so the mapper tries each table.
        List<String> searched = new ArrayList<>();
        Player p = null;
        searched.add("footballer");
        if (FOOTBALLER.containsKey(id)) {
            FootballerRow r = FOOTBALLER.get(id);
            p = new Footballer(r.name(), r.club());
        } else {
            searched.add("cricketer");
            if (CRICKETER.containsKey(id)) {
                CricketerRow r = CRICKETER.get(id);
                p = new Cricketer(r.name(), r.runs());
            } else {
                searched.add("bowler");
                BowlerRow r = BOWLER.get(id);
                p = new Bowler(r.name(), r.runs(), r.wickets());
            }
        }
        p.id = id;
        return new Found(p, searched);
    }

    public static void main(String[] args) {
        for (Player p : List.of(new Footballer("Ann", "Lions"), new Cricketer("Bo", 412), new Bowler("Cy", 95, 31))) {
            insert(p);
        }
        List<String> parts = new ArrayList<>();
        FOOTBALLER.forEach((id, r) -> parts.add(id + " " + r.name() + " " + r.club()));
        System.out.println("footballer: " + String.join(" | ", parts));
        parts.clear();
        CRICKETER.forEach((id, r) -> parts.add(id + " " + r.name() + " " + r.runs()));
        System.out.println("cricketer: " + String.join(" | ", parts));
        parts.clear();
        BOWLER.forEach((id, r) -> parts.add(id + " " + r.name() + " " + r.runs() + " " + r.wickets()));
        System.out.println("bowler: " + String.join(" | ", parts));
        Found f = find(3);
        System.out.println("find 3: " + f.player().describe() + " (tables searched: " + String.join(", ", f.searched()) + ")");
        // The union of all the tables, as a polymorphic query does it.
        List<Summary> everyone = new ArrayList<>();
        FOOTBALLER.forEach((id, r) -> everyone.add(new Summary(id, r.name(), "footballer")));
        CRICKETER.forEach((id, r) -> everyone.add(new Summary(id, r.name(), "cricketer")));
        BOWLER.forEach((id, r) -> everyone.add(new Summary(id, r.name(), "bowler")));
        everyone.sort(Comparator.comparingLong(Summary::id));
        parts.clear();
        for (Summary s : everyone) {
            parts.add(s.id() + " " + s.name() + " " + s.table());
        }
        System.out.println("all players: " + String.join(" | ", parts));
        List<String> cricketers = new ArrayList<>();
        CRICKETER.values().forEach(r -> cricketers.add(r.name()));
        BOWLER.values().forEach(r -> cricketers.add(r.name()));
        Collections.sort(cricketers);
        System.out.println("cricketers: " + String.join(", ", cricketers));
        System.out.println("next key: " + nextId);
    }
}
```

```output
footballer: 1 Ann Lions
cricketer: 2 Bo 412
bowler: 3 Cy 95 31
find 3: Bowler Cy, runs 95, wickets 31 (tables searched: footballer, cricketer, bowler)
all players: 1 Ann footballer | 2 Bo cricketer | 3 Cy bowler
cricketers: Bo, Cy
next key: 4
```

- Verdict: idiom. Each table is a `TreeMap` of records with all the fields of the class, and `insert` uses pattern matching for `instanceof` to choose the table.
- With JPA or Hibernate, `@Inheritance(strategy = InheritanceType.TABLE_PER_CLASS)` on the root class gives the same tables, and a polymorphic query becomes a `UNION`.
