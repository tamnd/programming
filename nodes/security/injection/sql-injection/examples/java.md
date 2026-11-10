```java wrong id=statement
import java.lang.reflect.Proxy;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class Main {
    // A fake connection that prints what JDBC code sends to a driver.
    // The JDK has no JDBC driver, so the example records the calls.
    static Connection recorder() {
        return (Connection) Proxy.newProxyInstance(
                Main.class.getClassLoader(),
                new Class<?>[] {Connection.class},
                (proxy, method, args) -> switch (method.getName()) {
                    case "createStatement" -> statement(null);
                    case "prepareStatement" -> statement((String) args[0]);
                    default -> null;
                });
    }

    static Object statement(String prepared) {
        List<Object> params = new ArrayList<>();
        Class<?> type = prepared == null ? Statement.class : PreparedStatement.class;
        return Proxy.newProxyInstance(
                Main.class.getClassLoader(),
                new Class<?>[] {type},
                (proxy, method, args) -> {
                    switch (method.getName()) {
                        case "setString" -> params.add(args[1]);
                        case "executeQuery" -> {
                            String sql = prepared == null ? (String) args[0] : prepared;
                            System.out.println("query: " + sql);
                            System.out.println("args:  " + params);
                        }
                        default -> {}
                    }
                    return null;
                });
    }

    public static void main(String[] args) throws SQLException {
        Connection db = recorder();
        String customer = "x' OR '1'='1";

        Statement statement = db.createStatement();
        statement.executeQuery("SELECT id FROM orders WHERE customer = '" + customer + "'");
    }
}
```

```output for=statement
query: SELECT id FROM orders WHERE customer = 'x' OR '1'='1'
args:  []
```

```java run id=prepared
import java.lang.reflect.Proxy;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class Main {
    // A fake connection that prints what JDBC code sends to a driver.
    // The JDK has no JDBC driver, so the example records the calls.
    static Connection recorder() {
        return (Connection) Proxy.newProxyInstance(
                Main.class.getClassLoader(),
                new Class<?>[] {Connection.class},
                (proxy, method, args) -> switch (method.getName()) {
                    case "createStatement" -> statement(null);
                    case "prepareStatement" -> statement((String) args[0]);
                    default -> null;
                });
    }

    static Object statement(String prepared) {
        List<Object> params = new ArrayList<>();
        Class<?> type = prepared == null ? Statement.class : PreparedStatement.class;
        return Proxy.newProxyInstance(
                Main.class.getClassLoader(),
                new Class<?>[] {type},
                (proxy, method, args) -> {
                    switch (method.getName()) {
                        case "setString" -> params.add(args[1]);
                        case "executeQuery" -> {
                            String sql = prepared == null ? (String) args[0] : prepared;
                            System.out.println("query: " + sql);
                            System.out.println("args:  " + params);
                        }
                        default -> {}
                    }
                    return null;
                });
    }

    public static void main(String[] args) throws SQLException {
        Connection db = recorder();
        String customer = "x' OR '1'='1";

        PreparedStatement statement = db.prepareStatement("SELECT id FROM orders WHERE customer = ?");
        statement.setString(1, customer);
        statement.executeQuery();
    }
}
```

```output for=prepared
query: SELECT id FROM orders WHERE customer = ?
args:  [x' OR '1'='1]
```

- Verdict: stdlib. JDBC (`java.sql`) is part of Java SE. `PreparedStatement` takes SQL text with `?` placeholders, and `setString`, `setLong` and the other setters bind the values by position. `Statement` takes complete SQL text and has no parameters.
- The JDK has no JDBC driver, so the examples use a fake connection, made with `java.lang.reflect.Proxy`, that prints what it receives.
- The driver decides how it sends the values. A PostgreSQL driver uses the extended query protocol, where the values travel in their own message. MySQL Connector/J prepares statements on the client by default (`useServerPrepStmts=false`) and escapes the values in the driver. Both keep the value out of the SQL grammar.
- JPA and Hibernate have the same weakness when a JPQL or HQL query string is built with `+`. Use `setParameter` with named parameters such as `:customer`. CWE-564 covers SQL injection through Hibernate.
- `CallableStatement` binds the arguments of a stored procedure. A procedure that builds dynamic SQL from its arguments is still vulnerable.
