```java run id=speech
import java.util.*;
import java.util.function.Function;

public class Main {
    record KnowledgeSource(String name, String src, String dst, Function<List<String>, List<String>> run) {}

    static final Map<String, String> LEXICON = Map.of("hel lo", "hello", "wor ld", "world");

    public static void main(String[] args) {
        List<KnowledgeSource> sources = List.of(
            new KnowledgeSource("grammar", "words", "sentence", w -> {
                String s = String.join(" ", w);
                return List.of(Character.toUpperCase(s.charAt(0)) + s.substring(1) + ".");
            }),
            new KnowledgeSource("lexicon", "syllables", "words", syl -> {
                List<String> words = new ArrayList<>();
                for (int i = 0; i < syl.size(); i += 2) {
                    words.add(LEXICON.get(String.join(" ", syl.subList(i, i + 2))));
                }
                return words;
            }),
            new KnowledgeSource("segmenter", "signal", "syllables",
                sig -> List.of(sig.get(0).split("-"))));

        Map<String, List<String>> board = new HashMap<>();
        board.put("signal", List.of("hel-lo-wor-ld"));

        // The control component selects a source that can contribute now.
        while (!board.containsKey("sentence")) {
            KnowledgeSource ks = sources.stream()
                .filter(s -> board.containsKey(s.src()) && !board.containsKey(s.dst()))
                .findFirst().orElseThrow();
            board.put(ks.dst(), ks.run().apply(board.get(ks.src())));
            System.out.println(ks.name() + " adds " + ks.dst() + ": " + String.join(" ", board.get(ks.dst())));
        }
    }
}
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A map is the blackboard, and records with functions are the knowledge sources.
