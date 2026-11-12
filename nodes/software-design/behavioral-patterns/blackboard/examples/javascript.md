```javascript run id=speech
const LEXICON = new Map([["hel lo", "hello"], ["wor ld", "world"]]);

const segment = (signal) => signal[0].split("-");
const lookup = (syl) => {
  const words = [];
  for (let i = 0; i < syl.length; i += 2) words.push(LEXICON.get(syl.slice(i, i + 2).join(" ")));
  return words;
};
const parse = (words) => {
  const s = words.join(" ");
  return [s[0].toUpperCase() + s.slice(1) + "."];
};

// Each knowledge source: name, input level, output level, function.
const SOURCES = [
  { name: "grammar", src: "words", dst: "sentence", run: parse },
  { name: "lexicon", src: "syllables", dst: "words", run: lookup },
  { name: "segmenter", src: "signal", dst: "syllables", run: segment },
];

const board = new Map([["signal", ["hel-lo-wor-ld"]]]);

// The control component selects a source that can contribute now.
while (!board.has("sentence")) {
  const ks = SOURCES.find((s) => board.has(s.src) && !board.has(s.dst));
  board.set(ks.dst, ks.run(board.get(ks.src)));
  console.log(`${ks.name} adds ${ks.dst}: ${board.get(ks.dst).join(" ")}`);
}
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A `Map` is the blackboard, and functions are the knowledge sources.
