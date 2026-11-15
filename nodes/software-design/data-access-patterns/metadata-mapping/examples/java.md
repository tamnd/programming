```java run id=generic-mapper-from-metadata
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // A stand-in for the database: each table maps an id to a row of columns.
    static final Map<String, Map<Integer, Map<String, Object>>> DB = Map.of("player", new TreeMap<>(), "club", new TreeMap<>());

    public static class Player {
        public Integer id;
        public String name;
        public String country;

        public Player() {}

        Player(int id, String name, String country) {
            this.id = id;
            this.name = name;
            this.country = country;
        }

        @Override
        public String toString() {
            return "Player(id=" + id + ", name=" + name + ", country=" + country + ")";
        }
    }

    public static class Club {
        public Integer id;
        public String name;
        public String city;

        public Club() {}

        Club(int id, String name, String city) {
            this.id = id;
            this.name = name;
            this.city = city;
        }

        @Override
        public String toString() {
            return "Club(id=" + id + ", name=" + name + ", city=" + city + ")";
        }
    }

    record ColumnMap(String field, String column) {}

    // The metadata: the class, the table and the pairs of field and column.
    record DataMap(Class<?> cls, String table, List<ColumnMap> columns) {
        String describe() {
            List<String> pairs = new ArrayList<>();
            for (ColumnMap c : columns) pairs.add(c.field() + " -> " + c.column());
            return cls.getSimpleName() + " -> table " + table + ": " + String.join(", ", pairs);
        }
    }

    static final Map<Class<?>, DataMap> MAPS = new LinkedHashMap<>();

    static {
        MAPS.put(Player.class, new DataMap(Player.class, "player",
                List.of(new ColumnMap("id", "id"), new ColumnMap("name", "name"), new ColumnMap("country", "nation"))));
        MAPS.put(Club.class, new DataMap(Club.class, "club",
                List.of(new ColumnMap("id", "id"), new ColumnMap("name", "title"), new ColumnMap("city", "city"))));
    }

    // Generic code: it reads the metadata and uses reflection on the fields.
    static final class Mapper {
        static Object get(Object obj, String field) throws ReflectiveOperationException {
            Field f = obj.getClass().getField(field);
            return f.get(obj);
        }

        String insert(Object obj) throws ReflectiveOperationException {
            DataMap m = MAPS.get(obj.getClass());
            List<String> cols = new ArrayList<>();
            List<String> marks = new ArrayList<>();
            Map<String, Object> row = new LinkedHashMap<>();
            for (ColumnMap c : m.columns()) {
                cols.add(c.column());
                marks.add("?");
                row.put(c.column(), get(obj, c.field()));
            }
            DB.get(m.table()).put((Integer) get(obj, "id"), row);
            return "insert into " + m.table() + " (" + String.join(", ", cols) + ") values (" + String.join(", ", marks) + ")";
        }

        <T> T find(Class<T> cls, int id) throws ReflectiveOperationException {
            DataMap m = MAPS.get(cls);
            Map<String, Object> row = DB.get(m.table()).get(id);
            T obj = cls.getDeclaredConstructor().newInstance();
            for (ColumnMap c : m.columns()) {
                cls.getField(c.field()).set(obj, row.get(c.column()));
            }
            return obj;
        }

        String update(Object obj) throws ReflectiveOperationException {
            DataMap m = MAPS.get(obj.getClass());
            Map<String, Object> row = DB.get(m.table()).get((Integer) get(obj, "id"));
            List<String> sets = new ArrayList<>();
            for (ColumnMap c : m.columns()) {
                if (c.field().equals("id")) continue;
                row.put(c.column(), get(obj, c.field()));
                sets.add(c.column() + " = ?");
            }
            return "update " + m.table() + " set " + String.join(", ", sets) + " where id = ?";
        }
    }

    public static void main(String[] args) throws ReflectiveOperationException {
        Mapper mapper = new Mapper();
        for (DataMap m : MAPS.values()) System.out.println("map " + m.describe());
        System.out.println("insert: " + mapper.insert(new Player(1, "Ann", "NZ")));
        System.out.println("insert: " + mapper.insert(new Club(1, "Lions", "Leeds")));
        Player p = mapper.find(Player.class, 1);
        System.out.println("find player 1: " + p);
        System.out.println("find club 1: " + mapper.find(Club.class, 1));
        p.country = "AU";
        System.out.println("update: " + mapper.update(p));
        for (String table : List.of("player", "club")) {
            for (Map<String, Object> row : DB.get(table).values()) {
                List<String> vs = new ArrayList<>();
                for (Object v : row.values()) vs.add(String.valueOf(v));
                System.out.println("row in " + table + ": " + String.join(" ", vs));
            }
        }
    }
}
```

```output
map Player -> table player: id -> id, name -> name, country -> nation
map Club -> table club: id -> id, name -> title, city -> city
insert: insert into player (id, name, nation) values (?, ?, ?)
insert: insert into club (id, title, city) values (?, ?, ?)
find player 1: Player(id=1, name=Ann, country=NZ)
find club 1: Club(id=1, name=Lions, city=Leeds)
update: update player set name = ?, nation = ? where id = ?
row in player: 1 Ann AU
row in club: 1 Lions Leeds
```

- Verdict: idiom. `Class.getField` and `Field.get` and `Field.set` move the values, and `getDeclaredConstructor().newInstance()` creates the empty object. The mapped fields are public so that reflection can reach them without `setAccessible`.
- Jakarta Persistence puts the same metadata in annotations, such as `@Table` and `@Column`, and the provider reads them with reflection.
