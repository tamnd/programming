```javascript run id=class-in-a-closure
class Playlist {
  static Cursor = class { // a class stored in a static field: a nested class by convention
    #owner;
    #i = 0;
    constructor(owner) {
      this.#owner = owner;
    }
    next() {
      const songs = this.#owner.songs;
      return this.#i < songs.length
        ? { done: false, value: `${this.#owner.name}: ${songs[this.#i++]}` }
        : { done: true, value: undefined };
    }
  };

  constructor(name) {
    this.name = name;
    this.songs = [];
  }

  [Symbol.iterator]() {
    return new Playlist.Cursor(this);
  }
}

function makeCounter(start) {
  // A class declared in a function closes over the function's variables.
  class Counter {
    next() {
      return start++;
    }
  }
  return new Counter();
}

const p = new Playlist("road");
p.songs.push("one", "two");
console.log([...p]);
const c = makeCounter(5);
console.log(c.next(), c.next());
```

```output for=class-in-a-closure
[ 'road: one', 'road: two' ]
5 6
```

- Verdict: idiom. JavaScript has no nested class declarations. A class expression stored in a static field, `Playlist.Cursor`, gives the same name structure, and the code passes the outer object by hand.
- A class declared inside a function is a closure: its methods can read and change the function's local variables, here `start`. This plays the part of a Java local class, without the rule that the variable must not change.
