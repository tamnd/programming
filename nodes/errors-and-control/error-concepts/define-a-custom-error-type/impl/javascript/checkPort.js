// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

/** The text is not a valid TCP port number. */
export class PortError extends Error {
  constructor(text, reason) {
    super(`invalid port '${text}': ${reason}`);
    // Without this line, the name is "Error" in messages and stack traces.
    this.name = "PortError";
    this.text = text;
    this.reason = reason;
  }
}

/**
 * Returns the port number in text, or throws a PortError.
 * @param {string} text
 * @returns {number}
 */
export function parsePort(text) {
  if (text === "") {
    throw new PortError(text, "empty");
  }
  if (!/^[0-9]+$/.test(text)) {
    throw new PortError(text, "not a number");
  }
  const port = Number(text);
  if (port < 1 || port > 65535) {
    throw new PortError(text, "out of range");
  }
  return port;
}

/**
 * Returns "port N", or the message of the PortError.
 * @param {string} text
 * @returns {string}
 */
export function checkPort(text) {
  try {
    return `port ${parsePort(text)}`;
  } catch (e) {
    // A catch clause takes each exception, so test the type and throw the rest again.
    if (e instanceof PortError) {
      return e.message;
    }
    throw e;
  }
}
