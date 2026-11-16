// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

// finishOrder returns the job numbers in the order in which round-robin
// finishes them. JavaScript has no queue type. shift() on an array moves all
// of the other elements, so the queue is an array with a head index: push
// adds at the back, and the head index moves forward to dequeue.
export function finishOrder(work, slice) {
  const left = [...work];
  const queue = work.map((_, i) => i);
  let head = 0;
  const done = [];
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
