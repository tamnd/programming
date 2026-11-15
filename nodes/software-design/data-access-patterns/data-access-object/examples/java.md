```java run id=two-daos-one-interface
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    // The transfer object: plain data that goes in and out of the DAO.
    static class Player {
        final int id;
        final String name;
        String country;

        Player(int id, String name, String country) {
            this.id = id;
            this.name = name;
            this.country = country;
        }
    }

    // The interface that the business code knows. It names no storage.
    interface PlayerDao {
        void insert(Player p);
        Player find(int id);
        List<Player> findByCountry(String country);
        List<Player> findAll();
        void update(Player p);
        void delete(int id);
    }

    // The data source is a map from key to record.
    static class MemoryPlayerDao implements PlayerDao {
        private final Map<Integer, String[]> rows = new TreeMap<>();

        public void insert(Player p) { rows.put(p.id, new String[] {p.name, p.country}); }

        public Player find(int id) {
            String[] r = rows.get(id);
            return new Player(id, r[0], r[1]);
        }

        public List<Player> findByCountry(String country) {
            return findAll().stream().filter(p -> p.country.equals(country)).toList();
        }

        public List<Player> findAll() {
            return rows.keySet().stream().map(this::find).toList();
        }

        public void update(Player p) { rows.put(p.id, new String[] {p.name, p.country}); }

        public void delete(int id) { rows.remove(id); }
    }

    // The data source is a flat file: one line "id;name;country" for each player.
    static class TextPlayerDao implements PlayerDao {
        final List<String> lines = new ArrayList<>();

        private Player parse(String line) {
            String[] f = line.split(";");
            return new Player(Integer.parseInt(f[0]), f[1], f[2]);
        }

        private int index(int id) {
            for (int i = 0; i < lines.size(); i++) {
                if (parse(lines.get(i)).id == id) return i;
            }
            throw new IllegalArgumentException("no player " + id);
        }

        private static String format(Player p) { return p.id + ";" + p.name + ";" + p.country; }

        public void insert(Player p) { lines.add(format(p)); }

        public Player find(int id) { return parse(lines.get(index(id))); }

        public List<Player> findByCountry(String country) {
            return findAll().stream().filter(p -> p.country.equals(country)).toList();
        }

        public List<Player> findAll() {
            return lines.stream().map(this::parse).sorted((a, b) -> Integer.compare(a.id, b.id)).toList();
        }

        public void update(Player p) { lines.set(index(p.id), format(p)); }

        public void delete(int id) { lines.remove(index(id)); }
    }

    // The one place that knows which storage the program uses.
    static PlayerDao daoFactory(String kind) {
        return kind.equals("memory") ? new MemoryPlayerDao() : new TextPlayerDao();
    }

    // The business object. It uses only the DAO interface.
    static class PlayerService {
        private final PlayerDao dao;

        PlayerService(PlayerDao dao) { this.dao = dao; }

        void transfer(int id, String country) {
            Player p = dao.find(id);
            p.country = country;
            dao.update(p);
        }
    }

    static String names(List<Player> players) {
        return players.stream().map(p -> p.name + " " + p.country).collect(Collectors.joining(" | "));
    }

    public static void main(String[] args) {
        Map<String, PlayerDao> daos = new TreeMap<>();
        for (String kind : List.of("memory", "text")) {
            PlayerDao dao = daoFactory(kind);
            daos.put(kind, dao);
            PlayerService service = new PlayerService(dao);
            System.out.println("dao: " + dao.getClass().getSimpleName());
            for (Player p : List.of(new Player(1, "Ann", "NZ"), new Player(2, "Bo", "NZ"), new Player(3, "Cy", "GB"))) {
                dao.insert(p);
            }
            System.out.println("all: " + names(dao.findAll()));
            System.out.println("find 1: " + names(List.of(dao.find(1))));
            service.transfer(1, "AU");
            System.out.println("from NZ: " + names(dao.findByCountry("NZ")));
            System.out.println("from AU: " + names(dao.findByCountry("AU")));
            dao.delete(3);
            System.out.println("all: " + names(dao.findAll()));
        }

        // The business code never saw this format; only TextPlayerDao knows it.
        TextPlayerDao text = (TextPlayerDao) daos.get("text");
        System.out.println("lines in the text store: " + String.join(" | ", text.lines));
    }
}
```

```output
dao: MemoryPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
dao: TextPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
lines in the text store: 1;Ann;AU | 2;Bo;NZ
```

- Verdict: idiom. `PlayerDao` is a Java interface, as in the Core J2EE pattern, and the business object holds a reference of the interface type.
- In real code, the transfer object is often a record. Here `Player` is a class with a mutable `country`, because `PlayerService` changes it before the update.
- Spring and Jakarta EE applications usually get the DAO by dependency injection and not from a factory method.
