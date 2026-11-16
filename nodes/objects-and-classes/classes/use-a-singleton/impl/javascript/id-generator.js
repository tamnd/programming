// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

// A module is evaluated once, and every import gets the same bindings, so a
// module-level object is a singleton. The class is not exported, so other
// code cannot make a second instance.
let constructions = 0;

class IdGenerator {
  #next = 1;

  constructor() {
    constructions++;
  }

  nextId() {
    return this.#next++;
  }
}

export const idGenerator = new IdGenerator();

export function constructionCount() {
  return constructions;
}

export function singletonReport(calls) {
  const seen = new Set();
  const ids = [];
  for (let i = 0; i < calls; i++) {
    const g = idGenerator;
    seen.add(g);
    ids.push(g.nextId());
  }
  return [ids[ids.length - 1] - ids[0], constructions, seen.size];
}
