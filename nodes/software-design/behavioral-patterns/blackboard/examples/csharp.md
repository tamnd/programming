```csharp run id=speech
using System;
using System.Collections.Generic;
using System.Linq;

var lexicon = new Dictionary<string, string> { ["hel lo"] = "hello", ["wor ld"] = "world" };

var sources = new List<KnowledgeSource>
{
    new("grammar", "words", "sentence", w =>
    {
        var s = string.Join(" ", w);
        return new List<string> { char.ToUpper(s[0]) + s[1..] + "." };
    }),
    new("lexicon", "syllables", "words", syl =>
        Enumerable.Range(0, syl.Count / 2)
            .Select(i => lexicon[syl[2 * i] + " " + syl[2 * i + 1]]).ToList()),
    new("segmenter", "signal", "syllables", sig => sig[0].Split('-').ToList()),
};

var board = new Dictionary<string, List<string>> { ["signal"] = new() { "hel-lo-wor-ld" } };

// The control component selects a source that can contribute now.
while (!board.ContainsKey("sentence"))
{
    var ks = sources.First(s => board.ContainsKey(s.Src) && !board.ContainsKey(s.Dst));
    board[ks.Dst] = ks.Run(board[ks.Src]);
    Console.WriteLine($"{ks.Name} adds {ks.Dst}: {string.Join(" ", board[ks.Dst])}");
}

record KnowledgeSource(string Name, string Src, string Dst, Func<List<string>, List<string>> Run);
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A dictionary is the blackboard, and records with delegates are the knowledge sources.
