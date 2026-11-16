// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

/**
 * Returns the port number in text, or throws an Error.
 * @param {string} text
 * @returns {number}
 */
export function parsePort(text) {
  if (!/^[0-9]+$/.test(text)) {
    throw new Error(`not a number: '${text}'`);
  }
  const port = Number(text);
  if (port < 1 || port > 65535) {
    throw new Error(`out of range: ${text}`);
  }
  return port;
}

/**
 * Puts value in double quotes and escapes the characters that could end the line or the value.
 * @param {string} value
 * @returns {string}
 */
export function quote(value) {
  const escapes = { "\\": "\\\\", '"': '\\"', "\n": "\\n", "\r": "\\r", "\t": "\\t" };
  return `"${value.replace(/[\\"\n\r\t]/g, (c) => escapes[c])}"`;
}

/**
 * Returns the good ports. Writes a log line for each bad text, then a summary.
 * The default log writer is console.error, which writes to the standard error stream.
 * @param {string[]} texts
 * @param {(line: string) => void} [log]
 * @returns {number[]}
 */
export function readPorts(
  texts,
  log = (line) => console.error(line),
) {
  const ports = [];
  texts.forEach((text, i) => {
    try {
      ports.push(parsePort(text));
    } catch (e) {
      const message = e instanceof Error ? e.message : String(e);
      log(`level=ERROR msg="read port failed" line=${i + 1} err=${quote(message)}`);
    }
  });
  log(`level=INFO msg="ports read" ok=${ports.length} total=${texts.length}`);
  return ports;
}
