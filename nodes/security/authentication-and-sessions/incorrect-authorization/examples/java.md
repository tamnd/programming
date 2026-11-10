```java wrong id=contains
import java.util.Map;

public class Main {
    // The role of each user, as the identity provider sends it.
    static final Map<String, String> ROLES = Map.of(
        "ana", "ADMIN",
        "bao", "ADMIN_READONLY",
        "chi", "SUPPORT_ADMIN_TRAINEE");

    static String refund(String user, int cents) {
        String role = ROLES.get(user);
        // Meant to allow ADMIN only.
        if (role == null || !role.contains("ADMIN")) {
            return "403 not allowed";
        }
        return "200 refunded " + cents + " cents";
    }

    public static void main(String[] args) {
        for (String user : new String[] {"ana", "bao", "chi"}) {
            System.out.println(user + " " + refund(user, 5000));
        }
    }
}
```

```output for=contains
ana 200 refunded 5000 cents
bao 200 refunded 5000 cents
chi 200 refunded 5000 cents
```

The check is meant to allow the role `ADMIN` only. `contains("ADMIN")` also matches `ADMIN_READONLY` and `SUPPORT_ADMIN_TRAINEE`, so all three users get the refund.

```java run id=enum-permissions
import java.util.EnumSet;
import java.util.Map;
import java.util.Set;

public class Main {
    enum Permission { READ_ORDERS, REFUND }

    enum Role {
        ADMIN(EnumSet.of(Permission.READ_ORDERS, Permission.REFUND)),
        ADMIN_READONLY(EnumSet.of(Permission.READ_ORDERS)),
        SUPPORT(EnumSet.of(Permission.READ_ORDERS));

        final Set<Permission> permissions;

        Role(Set<Permission> permissions) {
            this.permissions = permissions;
        }
    }

    // The role of each user, as the identity provider sends it.
    static final Map<String, String> ROLES = Map.of(
        "ana", "ADMIN",
        "bao", "ADMIN_READONLY",
        "chi", "SUPPORT_ADMIN_TRAINEE");

    // An exact match. A role name that is not a constant of Role has no permissions.
    static boolean allowed(String user, Permission permission) {
        String name = ROLES.get(user);
        for (Role role : Role.values()) {
            if (role.name().equals(name)) {
                return role.permissions.contains(permission);
            }
        }
        return false;
    }

    static String refund(String user, int cents) {
        if (!allowed(user, Permission.REFUND)) {
            return "403 not allowed";
        }
        return "200 refunded " + cents + " cents";
    }

    public static void main(String[] args) {
        for (String user : new String[] {"ana", "bao", "chi"}) {
            System.out.println(user + " " + refund(user, 5000));
        }
    }
}
```

```output for=enum-permissions
ana 200 refunded 5000 cents
bao 403 not allowed
chi 403 not allowed
```

Each `Role` constant has an `EnumSet` of permissions. The check finds the role by its exact name and asks for one exact permission. A role name that is not a constant matches nothing, so it has no permissions.

- Verdict: idiom. Java SE has enums and `EnumSet`, which fit a fixed set of roles and permissions. It has no authorization framework for web requests; Jakarta EE and Spring Security add one.
- `Role.valueOf(name)` also does an exact match, but it throws `IllegalArgumentException` for an unknown name. If the code uses it, the handler must turn the exception into "deny".
- Keep the permission in the check, not the role. A check for `Permission.REFUND` stays correct when a new role gets that permission.
