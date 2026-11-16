// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

// Submit one job for each number to a fixed pool of worker threads and collect the results in order.

import { Worker } from "node:worker_threads";

// Each worker waits for jobs, runs them one at a time and sends back each result.
const source = `
const { parentPort } = require("node:worker_threads");
parentPort.on("message", ({ index, n }) => {
  let steps = 0;
  while (n !== 1) {
    n = n % 2 === 0 ? n / 2 : 3 * n + 1;
    steps++;
  }
  parentPort.postMessage({ index, steps });
});
`;

// Returns a promise of the step counts, in the order of numbers.
export async function stepsOnPool(workers: number, numbers: readonly number[]): Promise<number[]> {
  const results: number[] = new Array<number>(numbers.length).fill(0);
  const size = Math.min(workers, numbers.length);
  const pool = Array.from({ length: size }, () => new Worker(source, { eval: true }));
  let next = 0;
  try {
    await Promise.all(
      pool.map(
        (worker) =>
          new Promise<void>((resolve, reject) => {
            // Give the worker the next job, or finish when no job is left.
            const giveNext = () => {
              if (next === numbers.length) {
                resolve();
                return;
              }
              const index = next++;
              worker.postMessage({ index, n: numbers[index] });
            };
            worker.on("message", ({ index, steps }: { index: number; steps: number }) => {
              results[index] = steps;
              giveNext();
            });
            worker.once("error", reject);
            giveNext();
          }),
      ),
    );
  } finally {
    // A worker with a message listener keeps running, so stop every worker.
    await Promise.all(pool.map((worker) => worker.terminate()));
  }
  return results;
}
