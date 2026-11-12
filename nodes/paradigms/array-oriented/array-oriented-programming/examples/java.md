```java run id=arrays-and-streams
import java.util.Arrays;
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        int[] a = {3, 1, 4, 1, 5};
        int[] b = {2, 7, 1, 8, 2};

        int[] sum = new int[a.length];
        Arrays.setAll(sum, i -> a[i] + b[i]);               // element-wise
        int dot = IntStream.range(0, a.length).map(i -> a[i] * b[i]).sum();
        int[] scan = a.clone();
        Arrays.parallelPrefix(scan, Integer::sum);          // running totals
        int[] big = Arrays.stream(a).filter(x -> x > 2).toArray();

        System.out.println(Arrays.toString(sum) + " " + dot);
        System.out.println(Arrays.toString(scan) + " " + Arrays.toString(big));
    }
}
```

```output for=arrays-and-streams
[5, 8, 5, 9, 7] 35
[3, 4, 8, 9, 14] [3, 4, 5]
```

- Verdict: idiom. `Arrays.setAll` fills an array from a function of the index, `IntStream` gives reductions, and `Arrays.parallelPrefix` gives a scan that can use more than one core. The operators do not work on arrays. The Vector API, an incubator module of the JDK, gives explicit SIMD operations on short vectors.
