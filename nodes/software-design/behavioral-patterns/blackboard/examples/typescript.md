```typescript run id=speech
type Level = "signal" | "syllables" | "words" | "sentence";
type Board = Map<Level, string[]>;

interface KnowledgeSource {
  name: string;
  src: Level;
  dst: Level;
  run(input: string[]): string[];
}

const LEXICON = new Map<string, string>([["hel lo", "hello"], ["wor ld", "world"]]);

const SOURCES: KnowledgeSource[] = [
  {
    name: "grammar", src: "words", dst: "sentence",
    run: (w) => {
      const s = w.join(" ");
      return [s[0].toUpperCase() + s.slice(1) + "."];
    },
  },
  {
    name: "lexicon", src: "syllables", dst: "words",
    run: (syl) => {
      const words: string[] = [];
      for (let i = 0; i < syl.length; i += 2) words.push(LEXICON.get(syl.slice(i, i + 2).join(" "))!);
      return words;
    },
  },
  { name: "segmenter", src: "signal", dst: "syllables", run: (sig) => sig[0].split("-") },
];

const board: Board = new Map([["signal", ["hel-lo-wor-ld"]]]);

// The control component selects a source that can contribute now.
while (!board.has("sentence")) {
  const ks = SOURCES.find((s) => board.has(s.src) && !board.has(s.dst))!;
  board.set(ks.dst, ks.run(board.get(ks.src)!));
  console.log(`${ks.name} adds ${ks.dst}: ${board.get(ks.dst)!.join(" ")}`);
}
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. An interface describes a knowledge source, and a union type names the levels of the blackboard.
