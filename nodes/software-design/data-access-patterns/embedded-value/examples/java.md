```java run id=value-in-owner-columns
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // Value objects: records are immutable and compare by their fields.
    record DateRange(String start, String end) {
        @Override
        public String toString() {
            return start + " to " + (end == null ? "open" : end);
        }
    }

    record Money(int amount, String currency) {
        @Override
        public String toString() {
            return amount + " " + currency;
        }
    }

    // The row: the columns of the employment table.
    record Row(String person, String startDate, String endDate, int salaryAmount, String salaryCurrency) {}

    // A stand-in for the database: one table keyed by id.
    static final Map<Long, Row> EMPLOYMENT = new TreeMap<>();
    static long lastId = 0;

    static class Employment {
        Long id;
        String person;
        DateRange period;
        Money salary;

        Employment(String person, DateRange period, Money salary, Long id) {
            this.person = person;
            this.period = period;
            this.salary = salary;
            this.id = id;
        }
    }

    static Row toRow(Employment e) {
        return new Row(e.person, e.period.start(), e.period.end(), e.salary.amount(), e.salary.currency());
    }

    static void insert(Employment e) {
        EMPLOYMENT.put(++lastId, toRow(e));
        e.id = lastId;
    }

    static void update(Employment e) {
        EMPLOYMENT.put(e.id, toRow(e));
    }

    static Employment find(long id) {
        Row r = EMPLOYMENT.get(id);
        return new Employment(r.person(), new DateRange(r.startDate(), r.endDate()),
                new Money(r.salaryAmount(), r.salaryCurrency()), id);
    }

    static String show(Object v) {
        return v == null ? "null" : v.toString();
    }

    public static void main(String[] args) {
        insert(new Employment("Ann", new DateRange("2024-01-01", "2024-12-31"), new Money(5000, "EUR"), null));
        insert(new Employment("Bo", new DateRange("2025-03-01", null), new Money(4000, "EUR"), null));
        EMPLOYMENT.forEach((id, r) -> System.out.println("row: " + id + " " + r.person() + " " + r.startDate() + " "
                + show(r.endDate()) + " " + r.salaryAmount() + " " + r.salaryCurrency()));
        Employment ann = find(1);
        Employment bo = find(2);
        for (Employment e : List.of(ann, bo)) {
            System.out.println("loaded: " + e.person + " | " + e.period + " | " + e.salary);
        }
        String same = ann.salary.equals(new Money(5000, "EUR")) ? "yes" : "no";
        System.out.println("salary of Ann equals 5000 EUR: " + same);

        ann.salary = new Money(5500, "EUR"); // A new value replaces the old one.
        update(ann);
        System.out.println("after raise: Ann " + find(1).salary + " | Bo " + find(2).salary);

        List<String> above = new ArrayList<>();
        for (Row r : EMPLOYMENT.values()) {
            if (r.salaryCurrency().equals("EUR") && r.salaryAmount() > 4500) {
                above.add(r.person());
            }
        }
        System.out.println("EUR salaries above 4500: " + String.join(", ", above));
    }
}
```

```output
row: 1 Ann 2024-01-01 2024-12-31 5000 EUR
row: 2 Bo 2025-03-01 null 4000 EUR
loaded: Ann | 2024-01-01 to 2024-12-31 | 5000 EUR
loaded: Bo | 2025-03-01 to open | 4000 EUR
salary of Ann equals 5000 EUR: yes
after raise: Ann 5500 EUR | Bo 4000 EUR
EUR salaries above 4500: Ann
```

- Verdict: idiom. `DateRange` and `Money` are records, which are immutable and get `equals` and `hashCode` from their components.
- With JPA or Hibernate, an `@Embeddable` class and an `@Embedded` field give the same mapping, and `@AttributeOverride` renames the columns when two values have the same type.
