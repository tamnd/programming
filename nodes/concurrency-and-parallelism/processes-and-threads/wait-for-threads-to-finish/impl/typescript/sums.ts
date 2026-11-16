// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

// Sum each chunk on its own worker thread, wait for all of them and collect the sums.

import { Worker } from "node:worker_threads";

const source = `
const { parentPort, workerData } = require("node:worker_threads");
let sum = 0;
for (const n of workerData) sum += n;
parentPort.postMessage(sum);
`;

function sumOnWorker(chunk: readonly number[]): Promise<number> {
  return new Promise((resolve, reject) => {
    const worker = new Worker(source, { eval: true, workerData: chunk });
    worker.once("message", (sum: number) => resolve(sum));
    worker.once("error", reject);
  });
}

// Returns a promise of the sums, in the order of chunks.
export function parallelSums(chunks: readonly (readonly number[])[]): Promise<number[]> {
  // Promise.all settles after every worker has sent its sum, and keeps the order of the input.
  return Promise.all(chunks.map(sumOnWorker));
}
