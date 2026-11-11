```java run id=copy
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> tracks = new ArrayList<>(List.of("intro", "theme"));
        List<String> view = Collections.unmodifiableList(tracks);
        List<String> copy = List.copyOf(tracks);

        tracks.add("outro");
        System.out.println(view);
        System.out.println(copy);
    }
}
```

```output for=copy
[intro, theme, outro]
[intro, theme]
```

- Verdict: absent. The garbage collector frees objects, and every reference can read and write the object. A class that must not share mutable state keeps a defensive copy.
- An unmodifiable view blocks writes through the view, but the owner can still change the data. `List.copyOf` makes an immutable copy that no other code can change.
