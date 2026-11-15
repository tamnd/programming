```java run id=mapper-for-each-class
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // A stand-in for the database: one player table with a type column.
    static final Map<Long, Map<String, Object>> PLAYER = new TreeMap<>();
    static long nextId = 1;

    abstract static class Player {
        Long id;
        String name;

        abstract String describe();
    }

    static class Footballer extends Player {
        String club;

        String describe() {
            return "Footballer " + name + ", club " + club;
        }
    }

    static class Cricketer extends Player {
        int runs;

        String describe() {
            return "Cricketer " + name + ", runs " + runs;
        }
    }

    static class Bowler extends Cricketer {
        int wickets;

        @Override
        String describe() {
            return "Bowler " + name + ", runs " + runs + ", wickets " + wickets;
        }
    }

    // Each mapper class saves and loads the fields that its domain class declares,
    // and calls the mapper of the superclass for the inherited fields.
    abstract static class AbstractPlayerMapper {
        abstract String typeCode();

        abstract Player create();

        void save(Player p, Map<String, Object> row, List<String> log) {
            row.put("name", p.name);
            log.add("name (Player)");
        }

        void load(Player p, Map<String, Object> row) {
            p.name = (String) row.get("name");
        }

        List<String> insert(Player p) {
            Map<String, Object> row = new HashMap<>();
            row.put("type", typeCode());
            List<String> log = new ArrayList<>();
            save(p, row, log);
            p.id = nextId++;
            PLAYER.put(p.id, row);
            return log;
        }

        Player find(long id, Map<String, Object> row) {
            Player p = create();
            p.id = id;
            load(p, row);
            return p;
        }
    }

    static class FootballerMapper extends AbstractPlayerMapper {
        String typeCode() {
            return "footballer";
        }

        Player create() {
            return new Footballer();
        }

        @Override
        void save(Player p, Map<String, Object> row, List<String> log) {
            super.save(p, row, log);
            row.put("club", ((Footballer) p).club);
            log.add("club (Footballer)");
        }

        @Override
        void load(Player p, Map<String, Object> row) {
            super.load(p, row);
            ((Footballer) p).club = (String) row.get("club");
        }
    }

    static class CricketerMapper extends AbstractPlayerMapper {
        String typeCode() {
            return "cricketer";
        }

        Player create() {
            return new Cricketer();
        }

        @Override
        void save(Player p, Map<String, Object> row, List<String> log) {
            super.save(p, row, log);
            row.put("runs", ((Cricketer) p).runs);
            log.add("runs (Cricketer)");
        }

        @Override
        void load(Player p, Map<String, Object> row) {
            super.load(p, row);
            ((Cricketer) p).runs = (Integer) row.get("runs");
        }
    }

    static class BowlerMapper extends CricketerMapper {
        @Override
        String typeCode() {
            return "bowler";
        }

        @Override
        Player create() {
            return new Bowler();
        }

        @Override
        void save(Player p, Map<String, Object> row, List<String> log) {
            super.save(p, row, log);
            row.put("wickets", ((Bowler) p).wickets);
            log.add("wickets (Bowler)");
        }

        @Override
        void load(Player p, Map<String, Object> row) {
            super.load(p, row);
            ((Bowler) p).wickets = (Integer) row.get("wickets");
        }
    }

    record Saved(AbstractPlayerMapper mapper, List<String> log) {}

    record Found(AbstractPlayerMapper mapper, Player player) {}

    // The mapper for the whole hierarchy: it chooses the concrete mapper and delegates to it.
    static class PlayerMapper {
        final Map<String, AbstractPlayerMapper> byType = new LinkedHashMap<>();
        final Map<Class<?>, AbstractPlayerMapper> byClass = new HashMap<>();

        PlayerMapper() {
            for (AbstractPlayerMapper m : List.of(new FootballerMapper(), new CricketerMapper(), new BowlerMapper())) {
                byType.put(m.typeCode(), m);
            }
            byClass.put(Footballer.class, byType.get("footballer"));
            byClass.put(Cricketer.class, byType.get("cricketer"));
            byClass.put(Bowler.class, byType.get("bowler"));
        }

        Saved insert(Player p) {
            AbstractPlayerMapper m = byClass.get(p.getClass());
            return new Saved(m, m.insert(p));
        }

        Found find(long id) {
            Map<String, Object> row = PLAYER.get(id);
            AbstractPlayerMapper m = byType.get((String) row.get("type"));
            return new Found(m, m.find(id, row));
        }

        List<Player> findAll() {
            List<Player> out = new ArrayList<>();
            PLAYER.forEach((id, row) -> out.add(byType.get((String) row.get("type")).find(id, row)));
            return out;
        }
    }

    static String chain(Object mapper) {
        List<String> names = new ArrayList<>();
        for (Class<?> c = mapper.getClass(); c != Object.class; c = c.getSuperclass()) {
            names.add(c.getSimpleName());
        }
        return String.join(", ", names);
    }


    public static void main(String[] args) {
        PlayerMapper players = new PlayerMapper();
        players.byType.forEach((t, m) -> System.out.println("mapper chain for " + t + ": " + chain(m)));
        Footballer ann = new Footballer();
        ann.name = "Ann";
        ann.club = "Lions";
        Cricketer bo = new Cricketer();
        bo.name = "Bo";
        bo.runs = 412;
        Bowler cy = new Bowler();
        cy.name = "Cy";
        cy.runs = 95;
        cy.wickets = 31;
        for (Player p : List.of(ann, bo, cy)) {
            Saved s = players.insert(p);
            System.out.println("insert " + p.name + ": " + s.mapper().getClass().getSimpleName() + " saved " + String.join(", ", s.log()));
        }
        Found f = players.find(3);
        System.out.println("find 3: " + f.player().describe() + " (mapper " + f.mapper().getClass().getSimpleName() + ")");
        List<String> all = new ArrayList<>();
        for (Player p : players.findAll()) {
            all.add(p.describe());
        }
        System.out.println("find all: " + String.join(" | ", all));
    }
}
```

```output
mapper chain for footballer: FootballerMapper, AbstractPlayerMapper
mapper chain for cricketer: CricketerMapper, AbstractPlayerMapper
mapper chain for bowler: BowlerMapper, CricketerMapper, AbstractPlayerMapper
insert Ann: FootballerMapper saved name (Player), club (Footballer)
insert Bo: CricketerMapper saved name (Player), runs (Cricketer)
insert Cy: BowlerMapper saved name (Player), runs (Cricketer), wickets (Bowler)
find 3: Bowler Cy, runs 95, wickets 31 (mapper BowlerMapper)
find all: Footballer Ann, club Lions | Cricketer Bo, runs 412 | Bowler Cy, runs 95, wickets 31
```

- Verdict: idiom. `AbstractPlayerMapper` is an abstract class with the steps of `insert` and `find`, and each concrete mapper overrides `save` and `load` and calls `super` first. A row is a `Map<String, Object>`, so each mapper casts its own fields.
- `PlayerMapper` maps `p.getClass()` to the concrete mapper. In Hibernate, the persister classes of the three table designs extend `AbstractEntityPersister` in the same way.
