// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

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
 * Returns the port number, or throws an Error whose cause is the error of parsePort.
 * @param {string} where
 * @param {string} text
 * @returns {number}
 */
export function readPort(where, text) {
  try {
    return parsePort(text);
  } catch (e) {
    throw new Error(`${where}: read port`, { cause: e });
  }
}

/**
 * Joins the message of e and the messages of its causes.
 * @param {unknown} e
 * @returns {string}
 */
function chain(e) {
  const parts = [];
  let cur = e;
  while (cur instanceof Error) {
    parts.push(cur.message);
    cur = cur.cause;
  }
  return parts.join(": ");
}

/**
 * Returns "port N", or "error: " and the chain of messages.
 * @param {string} where
 * @param {string} text
 * @returns {string}
 */
export function checkSetting(where, text) {
  try {
    return `port ${readPort(where, text)}`;
  } catch (e) {
    return `error: ${chain(e)}`;
  }
}
