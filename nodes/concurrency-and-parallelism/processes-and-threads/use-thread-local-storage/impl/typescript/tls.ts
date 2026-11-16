// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-thread-local-storage/.

// Count on a per-thread counter in one worker thread for each count, with no lock and no shared counter.

import { isMainThread, parentPort, Worker, workerData } from "node:worker_threads";

// Each thread loads its own copy of this module, so each thread has its own counter.
let counter = 0;

export function bump(): number {
  counter += 1;
  return counter;
}

export function current(): number {
  return counter;
}

if (!isMainThread) {
  // This is a worker: count, then send the counter of this thread back.
  for (let k = 0; k < (workerData as number); k++) bump();
  parentPort!.postMessage(current());
}

function runWorker(count: number): Promise<number> {
  return new Promise((resolve, reject) => {
    const worker = new Worker(new URL(import.meta.url), { workerData: count });
    worker.once("message", resolve);
    worker.once("error", reject);
  });
}

// Returns a promise of the final counter of each worker, in the order of counts.
export async function countPerThread(counts: number[]): Promise<number[]> {
  return Promise.all(counts.map(runWorker));
}
