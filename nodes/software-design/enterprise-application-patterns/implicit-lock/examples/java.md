```java run id=locking-mapper
import java.util.HashMap;
import java.util.Map;

public class Main {
    static class LockException extends Exception {
        LockException(String message) {
            super(message);
        }
    }

    // Exclusive locks for business transactions, kept until the transaction ends.
    static class LockManager {
        final Map<String, String> held = new HashMap<>();

        void acquire(String resource, String owner) throws LockException {
            String holder = held.putIfAbsent(resource, owner);
            if (holder != null && !holder.equals(owner)) {
                throw new LockException(resource + " is locked by " + holder);
            }
        }

        void releaseAll(String owner) {
            held.values().removeIf(o -> o.equals(owner));
        }
    }

    static class Customer {
        final int id;
        String name;
        int credit;

        Customer(int id, String name, int credit) {
            this.id = id;
            this.name = name;
            this.credit = credit;
        }
    }

    interface Mapper {
        Customer find(int id) throws LockException;

        void update(Customer customer);
    }

    static class CustomerMapper implements Mapper {
        final Map<Integer, Customer> rows = new HashMap<>();

        public Customer find(int id) {
            Customer row = rows.get(id);
            return new Customer(id, row.name, row.credit);
        }

        public void update(Customer c) {
            rows.put(c.id, new Customer(c.id, c.name, c.credit));
        }
    }

    // Wraps a mapper. Each find takes the lock first, so the application scripts never call the lock manager.
    static class LockingMapper implements Mapper {
        static int taken = 0;
        final Mapper mapper;
        final LockManager locks;
        final String owner;

        LockingMapper(Mapper mapper, LockManager locks, String owner) {
            this.mapper = mapper;
            this.locks = locks;
            this.owner = owner;
        }

        public Customer find(int id) throws LockException {
            locks.acquire("customer " + id, owner);
            taken++;
            return mapper.find(id);
        }

        public void update(Customer customer) {
            mapper.update(customer);
        }
    }

    static final LockManager locks = new LockManager();
    static final CustomerMapper plain = new CustomerMapper();

    // The framework gives each business transaction a locking mapper and releases its locks at the end.
    static class BusinessTransaction {
        final String owner;
        final Mapper customers;

        BusinessTransaction(String owner) {
            this.owner = owner;
            this.customers = new LockingMapper(plain, locks, owner);
        }

        void end() {
            locks.releaseAll(owner);
        }
    }

    // The application scripts. Not one of them calls the lock manager.
    static Customer openForRename(BusinessTransaction tx, int id) throws LockException {
        return tx.customers.find(id);
    }

    static void saveRename(BusinessTransaction tx, Customer customer, String name) {
        customer.name = name;
        tx.customers.update(customer);
        tx.end();
    }

    static int raiseCredit(BusinessTransaction tx, int id, int amount) throws LockException {
        Customer customer = tx.customers.find(id);
        customer.credit += amount;
        tx.customers.update(customer);
        tx.end();
        return customer.credit;
    }

    static void runRaiseCredit(String who, BusinessTransaction tx) {
        try {
            int credit = raiseCredit(tx, 1, 100);
            System.out.println(who + " raises the credit of customer 1: ok, credit " + credit);
        } catch (LockException e) {
            tx.end();
            System.out.println(who + " raises the credit of customer 1: refused: " + e.getMessage());
        }
    }

    public static void main(String[] args) throws LockException {
        plain.rows.put(1, new Customer(1, "Acme Ltd", 500));
        BusinessTransaction ann = new BusinessTransaction("ann");
        BusinessTransaction bob = new BusinessTransaction("bob");
        Customer customer = openForRename(ann, 1);
        System.out.println("ann opens customer 1 to rename it: " + customer.name);
        runRaiseCredit("bob", bob);
        saveRename(ann, customer, "Acme Group");
        System.out.println("ann saves the new name: Acme Group");
        runRaiseCredit("bob", bob);
        Customer last = plain.find(1);
        System.out.println("final: " + last.name + " with credit " + last.credit);
        System.out.println("locks taken by the mapper: " + LockingMapper.taken + " locks held now: " + locks.held.size());
    }
}
```

```output
ann opens customer 1 to rename it: Acme Ltd
bob raises the credit of customer 1: refused: customer 1 is locked by ann
ann saves the new name: Acme Group
bob raises the credit of customer 1: ok, credit 600
final: Acme Group with credit 600
locks taken by the mapper: 2 locks held now: 0
```

- Verdict: idiom. `Mapper` is the interface, and `LockingMapper` is a decorator that implements it. The scripts get a `Mapper` from the business transaction, so they cannot call the plain mapper by mistake.
- `putIfAbsent` checks and takes the lock in one call, and `releaseAll` removes the locks of one owner with `removeIf`.
