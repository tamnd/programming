```java run id=central-policy
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.Set;

public class Main {
    enum Permission { INVOICE_READ, USER_DELETE }

    record Invoice(int id, String owner, int total) {}

    record Response(int status, String body) {}

    static final Map<String, String> SESSIONS = Map.of("session-alice", "alice", "session-carol", "carol");
    static final Map<String, String> ROLES = Map.of("alice", "customer", "bob", "customer", "carol", "admin");
    // The policy lists what each role may do. Anything not listed is denied.
    static final Map<String, Set<Permission>> POLICY = Map.of(
            "customer", Set.of(Permission.INVOICE_READ),
            "admin", Set.of(Permission.INVOICE_READ, Permission.USER_DELETE));
    static final List<Invoice> INVOICES = List.of(new Invoice(1, "alice", 120), new Invoice(2, "bob", 75));

    interface Handler { Response handle(String user, String arg); }

    // A route cannot exist without a permission.
    record Route(Permission permission, Handler handler) {
        Route {
            Objects.requireNonNull(permission, "permission");
        }
    }

    // In SQL: SELECT ... FROM invoices WHERE id = ? AND owner = ?
    static Optional<Invoice> findByIdAndOwner(int id, String owner) {
        return INVOICES.stream().filter(i -> i.id() == id && i.owner().equals(owner)).findFirst();
    }

    static final Map<String, Route> ROUTES = Map.of(
            "getInvoice", new Route(Permission.INVOICE_READ, (user, arg) -> findByIdAndOwner(Integer.parseInt(arg), user)
                    .map(i -> new Response(200, i.toString()))
                    .orElse(new Response(404, "not found"))),
            "deleteUser", new Route(Permission.USER_DELETE, (user, arg) -> new Response(200, "deleted " + arg)));

    // Every request passes through this one check.
    static Response dispatch(String name, String session, String arg) {
        Route route = ROUTES.get(name);
        if (route == null) return new Response(404, "not found");
        String user = SESSIONS.get(session);
        if (user == null) return new Response(401, "log in first");
        Set<Permission> granted = POLICY.getOrDefault(ROLES.get(user), Set.of());
        if (!granted.contains(route.permission())) return new Response(403, "forbidden");
        return route.handler().handle(user, arg);
    }

    public static void main(String[] args) {
        System.out.println(dispatch("getInvoice", "session-alice", "1"));
        System.out.println(dispatch("getInvoice", "session-alice", "2"));
        System.out.println(dispatch("deleteUser", "session-alice", "bob"));
        System.out.println(dispatch("deleteUser", "session-carol", "bob"));
        System.out.println(dispatch("getInvoice", "none", "1"));
        try {
            new Route(null, (user, arg) -> new Response(200, "all invoices"));
        } catch (NullPointerException e) {
            System.out.println("refused: " + e.getMessage());
        }
    }
}
```

```output for=central-policy
Response[status=200, body=Invoice[id=1, owner=alice, total=120]]
Response[status=404, body=not found]
Response[status=403, body=forbidden]
Response[status=200, body=deleted bob]
Response[status=401, body=log in first]
refused: permission
```

The compact constructor of `Route` refuses a null permission, so a route cannot exist without one. `findByIdAndOwner` takes the owner as a parameter, so a handler cannot look up an invoice by id alone.

- Verdict: stdlib. Java SE has no web authorization. Jakarta EE and Spring add annotations and filters for the check.
- An `enum` for permissions makes a typing mistake a compile error. A string permission with a typing mistake fails closed here, because no role holds it.
- Name repository methods after their scope, such as `findByIdAndOwner`, so that a review sees a lookup by id alone.
