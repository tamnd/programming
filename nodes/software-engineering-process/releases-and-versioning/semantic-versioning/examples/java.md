```java run id=semver4j deps=deps/java/pom.xml
import java.util.List;
import org.semver4j.Semver;

public class Main {
    public static void main(String[] args) {
        List<Semver> versions = List.of("1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2", "1.0.0-alpha.beta", "1.0.0-alpha")
                .stream()
                .map(Semver::parse)
                .sorted()
                .toList();
        System.out.println(versions);

        Semver release = Semver.parse("1.4.2+build.7");
        System.out.println(release.getMajor() + " " + release.getMinor() + " " + release.getPatch() + " " + release.getBuild());
        System.out.println("isEqualTo: " + release.isEqualTo("1.4.2+build.8"));
        System.out.println("isEquivalentTo: " + release.isEquivalentTo("1.4.2+build.8"));

        for (String text : List.of("v1.2.3", "1.2", "01.2.3")) {
            System.out.println(text + " valid: " + Semver.isValid(text) + ", parse: " + Semver.parse(text) + ", coerce: " + Semver.coerce(text));
        }

        String[] candidates = {"0.2.9", "0.3.0", "1.2.9", "1.3.0", "2.0.0-rc.1", "2.0.0"};
        for (String range : List.of("^1.2.3", "^0.2.3", "~1.2.3", "1.x", "1.2.3", ">=1.2.0 <1.3.0 || >=2.0.0-rc.1")) {
            StringBuilder line = new StringBuilder(range + " ->");
            for (String candidate : candidates) {
                if (Semver.parse(candidate).satisfies(range)) {
                    line.append(" ").append(candidate);
                }
            }
            System.out.println(line);
        }
    }
}
```

```output for=semver4j
[1.0.0-alpha, 1.0.0-alpha.beta, 1.0.0-beta.2, 1.0.0-beta.11, 1.0.0-rc.1, 1.0.0]
1 4 2 [build, 7]
isEqualTo: false
isEquivalentTo: true
v1.2.3 valid: true, parse: 1.2.3, coerce: 1.2.3
1.2 valid: false, parse: null, coerce: 1.2.0
01.2.3 valid: false, parse: null, coerce: 1.2.3
^1.2.3 -> 1.2.9 1.3.0
^0.2.3 -> 0.2.9
~1.2.3 -> 1.2.9
1.x -> 1.2.9 1.3.0
1.2.3 ->
>=1.2.0 <1.3.0 || >=2.0.0-rc.1 -> 1.2.9 2.0.0-rc.1 2.0.0
```

- Verdict: library. Java SE has no SemVer type. The example uses semver4j 6.0.0.
- `Semver` implements `Comparable` with SemVer precedence, so `sorted()` gives the order of item 11.
- `isEqualTo` compares the build metadata too. `isEquivalentTo` ignores it.
- `Semver.isValid` accepts "v1.2.3", although the specification says it is not a semantic version. `Semver.parse` returns `null` for text that is not valid. `Semver.coerce` turns loose text into a version, so it accepts input that the specification rejects.
- `satisfies` reads npm-style ranges, with caret, tilde, X-ranges and `||`. A bare version such as `1.2.3` is an exact match, as in npm, so it matches none of the candidates.
