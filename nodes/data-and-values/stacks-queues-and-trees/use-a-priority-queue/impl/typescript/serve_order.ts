// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

// TypeScript has no priority queue type. Heap is a small binary heap: the
// item for which before(a, b) is true comes out first.
class Heap<T> {
  private readonly items: T[] = [];
  private readonly before: (a: T, b: T) => boolean;

  constructor(before: (a: T, b: T) => boolean) {
    this.before = before;
  }

  get size(): number {
    return this.items.length;
  }

  push(x: T): void {
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

  pop(): T | undefined {
    const a = this.items;
    const top = a[0];
    const last = a.pop();
    if (a.length > 0 && last !== undefined) {
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
export function serveOrder(priorities: readonly number[]): number[] {
  const heap = new Heap<number>(
    (a, b) => priorities[a] > priorities[b] || (priorities[a] === priorities[b] && a < b),
  );
  priorities.forEach((_, i) => heap.push(i));
  const order: number[] = [];
  for (let i = heap.pop(); i !== undefined; i = heap.pop()) order.push(i);
  return order;
}
