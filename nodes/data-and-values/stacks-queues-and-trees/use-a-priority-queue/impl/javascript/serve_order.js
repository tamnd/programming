// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

// JavaScript has no priority queue type. Heap is a small binary heap: the
// item for which before(a, b) is true comes out first.
class Heap {
  constructor(before) {
    this.before = before;
    this.items = [];
  }

  get size() {
    return this.items.length;
  }

  push(x) {
    const a = this.items;
    a.push(x);
    let i = a.length - 1;
    while (i > 0) {
      const parent = (i - 1) >> 1;
      if (!this.before(a[i], a[parent])) break;
      [a[i], a[parent]] = [a[parent], a[i]];
      i = parent;
    }
  }

  pop() {
    const a = this.items;
    const top = a[0];
    const last = a.pop();
    if (a.length > 0) {
      a[0] = last;
      let i = 0;
      for (;;) {
        const l = 2 * i + 1;
        const r = l + 1;
        let best = i;
        if (l < a.length && this.before(a[l], a[best])) best = l;
        if (r < a.length && this.before(a[r], a[best])) best = r;
        if (best === i) break;
        [a[i], a[best]] = [a[best], a[i]];
        i = best;
      }
    }
    return top;
  }
}

// serveOrder returns the request numbers in serve order: the largest
// priority first, and for equal priorities, the smaller request number first.
export function serveOrder(priorities) {
  const heap = new Heap(
    (a, b) => priorities[a] > priorities[b] || (priorities[a] === priorities[b] && a < b),
  );
  priorities.forEach((_, i) => heap.push(i));
  const order = [];
  while (heap.size > 0) order.push(heap.pop());
  return order;
}
