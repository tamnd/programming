```java run id=locale-argument
import java.text.NumberFormat;
import java.util.Locale;

public class Main {
    public static void main(String[] args) {
        double x = 1234567.891;
        System.out.println(String.format(Locale.US, "%,.2f", x));
        System.out.println(String.format(Locale.GERMANY, "%,.2f", x));
        System.out.println(String.format(Locale.ROOT, "%.2f", x));
        System.out.println(String.format(Locale.FRANCE, "e = %+10.4f", Math.E));

        String fr = String.format(Locale.FRANCE, "%,.0f", x);
        System.out.printf("U+%04X%n", (int) fr.charAt(1));

        NumberFormat usd = NumberFormat.getCurrencyInstance(Locale.US);
        System.out.println(usd.format(x));
        NumberFormat de = NumberFormat.getInstance(Locale.GERMANY);
        System.out.println(de.format(x));
    }
}
```

```output for=locale-argument
1,234,567.89
1.234.567,89
1234567.89
e =    +2,7183
U+202F
$1,234,567.89
1.234.567,891
```

- Verdict: stdlib. `String.format`, `Formatter` and `NumberFormat` take a `Locale`, and the flag `,` asks for the group separator of that locale. Without a locale argument, `String.format` uses the default locale of the computer, so a program passes `Locale.ROOT` for machine text. Java reads the CLDR data by default.
