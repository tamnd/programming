// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

// Start one worker thread per name; each worker builds the greeting for its name.

import { Worker } from "node:worker_threads";

const source = `
const { parentPort, workerData } = require("node:worker_threads");
parentPort.postMessage("hello from " + workerData);
`;

function greetOnWorker(name: string): Promise<string> {
  return new Promise((resolve, reject) => {
    const worker = new Worker(source, { eval: true, workerData: name });
    worker.once("message", (message: string) => resolve(message));
    worker.once("error", reject);
  });
}

// Returns a promise of the greetings, in the order of names.
export function threadGreetings(names: readonly string[]): Promise<string[]> {
  // Each call starts a worker at once; Promise.all waits for all of them.
  return Promise.all(names.map(greetOnWorker));
}
