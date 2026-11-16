```java run id=switch-forms
public class Main {
    // The old form: labels with a colon, and break at the end of each group.
    static int daysIn(int month) {
        int days;
        switch (month) {
            case 2:
                days = 28;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                days = 30;
                break;
            default:
                days = 31;
                break;
        }
        return days;
    }

    // The new form: a switch expression with arrows and no fall-through.
    static String kind(String day) {
        return switch (day) {
            case "sat", "sun" -> "weekend";
            case "mon", "tue", "wed", "thu", "fri" -> "weekday";
            default -> "not a day";
        };
    }

    public static void main(String[] args) {
        System.out.println(daysIn(2) + " " + daysIn(4) + " " + daysIn(7));
        for (String day : new String[] {"sat", "wed", "xyz"}) {
            System.out.println(day + " " + kind(day));
        }
    }
}
```

```output for=switch-forms
28 30 31
sat weekend
wed weekday
xyz not a day
```

- Verdict: native. `daysIn` uses the statement form with `case ... :` labels. Four labels share the group for 30 days, and each group ends with `break`. `kind` uses a switch expression on a `String`. Each `case ... ->` lists several values, runs only its own code, and gives the value of the expression.
