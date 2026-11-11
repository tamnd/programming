```java run id=self-invocation
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;

public class Main {
    interface Account {
        void deposit(int amount);
        void depositTwice(int amount);
        int balance();
    }

    static class SimpleAccount implements Account {
        private int balance;
        public void deposit(int amount) { balance += amount; }
        public void depositTwice(int amount) { deposit(amount); deposit(amount); }
        public int balance() { return balance; }
    }

    public static void main(String[] args) {
        Account target = new SimpleAccount();
        InvocationHandler audit = (proxy, method, params) -> {
            if (method.getName().startsWith("deposit")) {
                System.out.println("before " + method.getName() + " " + params[0]);
            }
            return method.invoke(target, params);
        };
        Account account = (Account) Proxy.newProxyInstance(
                Account.class.getClassLoader(), new Class<?>[] {Account.class}, audit);

        account.deposit(10);
        account.depositTwice(5);
        System.out.println("balance " + account.balance());
    }
}
```

```output for=self-invocation
before deposit 10
before depositTwice 5
balance 20
```

- Verdict: library. AspectJ adds aspects, pointcuts and advice to Java and weaves them into the bytecode at compile time or at class load time. Spring AOP gives a subset with proxies. The JDK gives the base of the proxy approach: `java.lang.reflect.Proxy` makes an object that sends each call on an interface to an `InvocationHandler`.
- The output shows a pitfall of all proxy-based AOP. `depositTwice` calls `deposit` twice, but these calls go to `this`, the real object, and not through the proxy. Only the outer call is advised. AspectJ's weaving changes the bytecode of the class, so it advises internal calls too.
