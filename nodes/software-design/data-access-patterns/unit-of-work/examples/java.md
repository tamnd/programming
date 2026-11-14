```java run id=transfer-and-close
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    static final class Account {
        final int id;
        String owner;
        int balance;

        Account(int id, String owner, int balance) {
            this.id = id;
            this.owner = owner;
            this.balance = balance;
        }
    }

    record Entry(int accountId, int amount) {}

    record AccountRow(String owner, int balance) {}

    // A stand-in for the database, because the JDK has JDBC but no database driver.
    // transaction() keeps a copy of the tables and puts it back when a statement fails.
    static final class Database {
        TreeMap<Integer, AccountRow> accounts = new TreeMap<>(Map.of(1, new AccountRow("ann", 100), 2, new AccountRow("bob", 0)));
        List<Entry> entries = new ArrayList<>();

        void transaction(Runnable work) {
            var savedAccounts = new TreeMap<>(accounts);
            var savedEntries = new ArrayList<>(entries);
            try {
                work.run();
            } catch (IllegalStateException e) {
                accounts = savedAccounts;
                entries = savedEntries;
                throw e;
            }
        }

        void updateAccount(int id, String owner, int balance) {
            if (balance < 0) throw new IllegalStateException("CHECK constraint failed: balance >= 0");
            accounts.put(id, new AccountRow(owner, balance));
        }
    }

    interface Mapper {
        default void insert(Database db, Object obj) { throw new UnsupportedOperationException(); }
        default void update(Database db, Object obj) { throw new UnsupportedOperationException(); }
        default void delete(Database db, Object obj) { throw new UnsupportedOperationException(); }
    }

    static final class AccountMapper implements Mapper {
        Account find(Database db, int id) {
            var row = db.accounts.get(id);
            return new Account(id, row.owner(), row.balance());
        }

        public void update(Database db, Object obj) {
            var a = (Account) obj;
            System.out.println("  update accounts " + a.id + " " + a.owner + " " + a.balance);
            db.updateAccount(a.id, a.owner, a.balance);
        }

        public void delete(Database db, Object obj) {
            var a = (Account) obj;
            System.out.println("  delete accounts " + a.id);
            db.accounts.remove(a.id);
        }
    }

    static final class EntryMapper implements Mapper {
        public void insert(Database db, Object obj) {
            var e = (Entry) obj;
            System.out.println("  insert entries (" + e.accountId() + ", " + e.amount() + ")");
            db.entries.add(e);
        }
    }

    static final class UnitOfWork {
        private final Database db;
        private final Map<Class<?>, Mapper> mappers;
        private List<Object> created = new ArrayList<>(), dirty = new ArrayList<>(), removed = new ArrayList<>();

        UnitOfWork(Database db, Map<Class<?>, Mapper> mappers) {
            this.db = db;
            this.mappers = mappers;
        }

        // Identity, not equals: two equal records are still two objects to write.
        private static boolean has(List<Object> objs, Object obj) {
            return objs.stream().anyMatch(o -> o == obj);
        }

        String pending() {
            return created.size() + " new, " + dirty.size() + " dirty";
        }

        void registerNew(Object obj) {
            created.add(obj);
        }

        void registerDirty(Object obj) {
            if (!has(created, obj) && !has(dirty, obj)) dirty.add(obj);
        }

        void registerRemoved(Object obj) {
            if (has(created, obj)) { // never written, nothing to delete
                created.removeIf(o -> o == obj);
                return;
            }
            dirty.removeIf(o -> o == obj); // no update before the delete
            removed.add(obj);
        }

        void commit() {
            try {
                db.transaction(() -> {
                    for (var obj : created) mappers.get(obj.getClass()).insert(db, obj);
                    for (var obj : dirty) mappers.get(obj.getClass()).update(db, obj);
                    for (var obj : removed) mappers.get(obj.getClass()).delete(db, obj);
                });
                System.out.println("  commit");
            } catch (IllegalStateException e) {
                System.out.println("  rollback: " + e.getMessage());
            } finally {
                created = new ArrayList<>();
                dirty = new ArrayList<>();
                removed = new ArrayList<>();
            }
        }
    }

    static void transfer(UnitOfWork uow, Account source, Account target, int amount) {
        source.balance -= amount;
        target.balance += amount;
        uow.registerDirty(source);
        uow.registerDirty(target);
        uow.registerNew(new Entry(source.id, -amount));
        uow.registerNew(new Entry(target.id, amount));
    }

    static void show(Database db) {
        System.out.println("accounts: " + db.accounts.entrySet().stream()
                .map(e -> e.getKey() + " " + e.getValue().owner() + " " + e.getValue().balance())
                .collect(Collectors.joining(" | ")));
        System.out.println("entries: " + db.entries.stream()
                .map(e -> e.accountId() + " " + e.amount())
                .collect(Collectors.joining(" | ")));
    }

    public static void main(String[] args) {
        var db = new Database();
        var accounts = new AccountMapper();
        Map<Class<?>, Mapper> mappers = new HashMap<>(Map.of(Account.class, accounts, Entry.class, new EntryMapper()));

        var uow = new UnitOfWork(db, mappers);
        transfer(uow, accounts.find(db, 1), accounts.find(db, 2), 30);
        System.out.println("pending: " + uow.pending() + "; entries in the table: " + db.entries.size());
        System.out.println("commit 1");
        uow.commit();

        uow = new UnitOfWork(db, mappers);
        transfer(uow, accounts.find(db, 2), accounts.find(db, 1), 50); // bob has only 30
        System.out.println("commit 2");
        uow.commit();
        show(db);

        uow = new UnitOfWork(db, mappers);
        var ann = accounts.find(db, 1);
        var bob = accounts.find(db, 2);
        transfer(uow, bob, ann, 30); // close bob's account: move the money, then remove it
        uow.registerRemoved(bob);
        System.out.println("commit 3");
        uow.commit();
        show(db);
    }
}
```

```output
pending: 2 new, 2 dirty; entries in the table: 0
commit 1
  insert entries (1, -30)
  insert entries (2, 30)
  update accounts 1 ann 70
  update accounts 2 bob 30
  commit
commit 2
  insert entries (2, -50)
  insert entries (1, 50)
  update accounts 2 bob -20
  rollback: CHECK constraint failed: balance >= 0
accounts: 1 ann 70 | 2 bob 30
entries: 1 -30 | 2 30
commit 3
  insert entries (2, -30)
  insert entries (1, 30)
  update accounts 1 ann 100
  delete accounts 2
  commit
accounts: 1 ann 100
entries: 1 -30 | 2 30 | 2 -30 | 1 30
```

- Verdict: idiom. The lists compare with `==`, not with `equals`, because two `Entry` records with the same fields are equal but are two rows.
- `Database.transaction` takes a `Runnable`, keeps copies of the tables and puts them back when a statement throws, as a database rolls back a transaction.
