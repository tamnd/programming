```java run id=near-its-data
public class Main {
    record Parcel(double weightKg, int l, int w, int h) {
        // Moved here from Order: it uses only the parcel's data.
        long shippingCents() {
            double volumetricKg = (double) l * w * h / 5000;
            return 400 + (long) Math.ceil(Math.max(weightKg, volumetricKg)) * 150;
        }
    }

    record Order(int id, Parcel parcel) {
        long shippingCents() { return parcel.shippingCents(); } // delegates for a time
    }

    public static void main(String[] args) {
        if (new Order(1, new Parcel(0.8, 10, 10, 5)).shippingCents() != 550) throw new AssertionError("light");
        if (new Order(2, new Parcel(1.2, 60, 40, 40)).shippingCents() != 3400) throw new AssertionError("bulky");
        System.out.println("move function: tests pass");
    }
}
```

```output
move function: tests pass
```
