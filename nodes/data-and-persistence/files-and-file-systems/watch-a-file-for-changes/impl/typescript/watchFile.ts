// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.
// Watch a file for changes: poll its size and modification time.
import * as fs from "node:fs";

const BASE_MTIME = 1_700_000_000; // seconds; the cases set the mtime to BASE_MTIME + step

// The size and the mtime in ns of the file, or null when it does not exist.
export function signature(path: string): string | null {
  let st: fs.BigIntStats;
  try {
    st = fs.statSync(path, { bigint: true });
  } catch (err) {
    if ((err as NodeJS.ErrnoException).code === "ENOENT") return null;
    throw err;
  }
  if (st.isDirectory()) {
    const err: NodeJS.ErrnoException = new Error(`EISDIR: illegal operation on a directory, watch '${path}'`);
    err.code = "EISDIR";
    throw err;
  }
  return `${st.size}:${st.mtimeNs}`;
}

export class FileWatcher {
  readonly path: string;
  private last: string | null;

  constructor(path: string) {
    this.path = path;
    this.last = signature(path);
  }

  // created, deleted, modified or none, for the time since the last poll.
  poll(): string {
    const now = signature(this.path);
    const before = this.last;
    this.last = now;
    if (before === null && now === null) return "none";
    if (before === null) return "created";
    if (now === null) return "deleted";
    return now !== before ? "modified" : "none";
  }
}

// Call onChange(path) once for each burst of changes, after the file is quiet.
// It returns a function that stops the watch.
export function watch(
  path: string,
  onChange: (path: string) => void,
  { interval = 500, quiet = 200 }: { interval?: number; quiet?: number } = {},
): () => void {
  const watcher = new FileWatcher(path);
  let pending: number | null = null;
  const timer = setInterval(() => {
    if (watcher.poll() !== "none") {
      pending = Date.now();
    } else if (pending !== null && Date.now() - pending >= quiet) {
      pending = null;
      onChange(path);
    }
  }, interval);
  return () => clearInterval(timer);
}

// Make the change of one step of a case (the entry of the cases uses it).
function applyStep(path: string, step: string, number: number): void {
  const colon = step.indexOf(":");
  const kind = colon < 0 ? step : step.slice(0, colon);
  const text = colon < 0 ? "" : step.slice(colon + 1);
  switch (kind) {
    case "create":
    case "write":
      fs.writeFileSync(path, text);
      break;
    case "append":
      fs.appendFileSync(path, text);
      break;
    case "replace":
      fs.writeFileSync(`${path}.tmp`, text);
      fs.renameSync(`${path}.tmp`, path);
      break;
    case "recreate":
      fs.unlinkSync(path);
      fs.writeFileSync(path, text);
      break;
    case "delete":
      fs.unlinkSync(path);
      break;
    case "mkdir":
      fs.mkdirSync(path);
      break;
    case "touch":
    case "none":
      break;
    default:
      throw new Error(`unknown step ${step}`);
  }
  if (kind !== "none" && fs.existsSync(path) && fs.statSync(path).isFile()) {
    const t = BASE_MTIME + number;
    fs.utimesSync(path, t, t);
  }
}

// The entry of the cases: poll after each step and return the results.
export function watchChanges(path: string, steps: string[]): string[] {
  const watcher = new FileWatcher(path);
  return steps.map((step, i) => {
    applyStep(path, step, i + 1);
    return watcher.poll();
  });
}
