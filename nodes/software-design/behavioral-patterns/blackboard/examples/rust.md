```rust run id=speech
use std::collections::HashMap;

type Data = Vec<String>;

struct KnowledgeSource {
    name: &'static str,
    src: &'static str,
    dst: &'static str,
    run: fn(&Data) -> Data,
}

fn segment(sig: &Data) -> Data {
    sig[0].split('-').map(String::from).collect()
}

fn lookup(syl: &Data) -> Data {
    let lexicon: HashMap<&str, &str> = [("hel lo", "hello"), ("wor ld", "world")].into();
    syl.chunks(2).map(|p| lexicon[p.join(" ").as_str()].to_string()).collect()
}

fn parse(words: &Data) -> Data {
    let s = words.join(" ");
    let mut chars = s.chars();
    let first = chars.next().unwrap().to_uppercase().collect::<String>();
    vec![format!("{}{}.", first, chars.as_str())]
}

fn main() {
    let sources = [
        KnowledgeSource { name: "grammar", src: "words", dst: "sentence", run: parse },
        KnowledgeSource { name: "lexicon", src: "syllables", dst: "words", run: lookup },
        KnowledgeSource { name: "segmenter", src: "signal", dst: "syllables", run: segment },
    ];
    let mut board: HashMap<&str, Data> = HashMap::new();
    board.insert("signal", vec!["hel-lo-wor-ld".to_string()]);

    // The control component selects a source that can contribute now.
    while !board.contains_key("sentence") {
        let ks = sources
            .iter()
            .find(|s| board.contains_key(s.src) && !board.contains_key(s.dst))
            .unwrap();
        let out = (ks.run)(&board[ks.src]);
        println!("{} adds {}: {}", ks.name, ks.dst, out.join(" "));
        board.insert(ks.dst, out);
    }
}
```

```output for=speech
segmenter adds syllables: hel lo wor ld
lexicon adds words: hello world
grammar adds sentence: Hello world.
```

- Verdict: idiom. A `HashMap` is the blackboard, and function pointers are the knowledge sources.
