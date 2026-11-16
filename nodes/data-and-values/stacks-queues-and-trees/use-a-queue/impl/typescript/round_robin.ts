// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

// finishOrder returns the job numbers in the order in which round-robin
// finishes them. The queue is an array with a head index, because shift()
// moves all of the other elements.
export function finishOrder(work: readonly number[], slice: number): number[] {
  const left = [...work];
  const queue: number[] = work.map((_, i) => i);
  let head = 0;
  const done: number[] = [];
  while (head < queue.length) {
    const job = queue[head++];
    left[job] -= Math.min(slice, left[job]);
    if (left[job] === 0) {
      done.push(job);
    } else {
      queue.push(job);
    }
  }
  return done;
}
