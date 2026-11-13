```javascript run id=array-methods-and-iterator-helpers
const articles = [
  { title: "Intro to NoSQL", words: 1200, tags: ["nosql", "databases"] },
  { title: "Event sourcing", words: 800, tags: ["architecture"] },
  { title: "Graph stores", words: 1500, tags: ["nosql", "graphs"] },
  { title: "Refactoring loops", words: 600, tags: ["refactoring"] },
  { title: "Key-value stores", words: 900, tags: ["nosql"] },
];

// filter, sort, slice and map: each array method returns a new array.
const nosql = articles.filter((a) => a.tags.includes("nosql"));
console.log(nosql.toSorted((a, b) => b.words - a.words).slice(0, 2).map((a) => a.title));

// map and reduce.
console.log(nosql.map((a) => a.words).reduce((acc, n) => acc + n, 0));

// flatMap and Object.groupBy: the number of articles for each tag.
const groups = Object.groupBy(articles.flatMap((a) => a.tags), (t) => t);
console.log(Object.keys(groups).sort().map((t) => `${t}=${groups[t].length}`).join(" "));

let calls = 0;
const score = (a) => {
  calls++;
  return a.words;
};

// Array methods are eager: map computes every score before filter runs.
const eager = articles.map(score).filter((s) => s > 1000).slice(0, 1);
console.log("eager", eager, calls);

// Iterator helpers are lazy: take(1) stops the work after the first match.
calls = 0;
const lazy = articles.values().map(score).filter((s) => s > 1000).take(1).toArray();
console.log("lazy", lazy, calls);
```

```output
[ 'Graph stores', 'Intro to NoSQL' ]
3600
architecture=1 databases=1 graphs=1 nosql=3 refactoring=1
eager [ 1200 ] 5
lazy [ 1200 ] 1
```

- Verdict: native. The array methods filter, map, toSorted, slice, flatMap and reduce chain from left to right, and each one returns a new array, so the chain is eager and computes all 5 scores. Object.groupBy groups the tags. The iterator helpers on articles.values() are lazy, and take(1) stops the work after the first match, so score runs once.
