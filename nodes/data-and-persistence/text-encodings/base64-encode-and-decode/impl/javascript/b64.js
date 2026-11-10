// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

import { Buffer } from "node:buffer";

// The Buffer encoding names: "base64" pads with "=", and "base64url"
// adds no padding.
function bufferEncoding(alphabet) {
  if (alphabet === "standard") return "base64";
  if (alphabet === "url") return "base64url";
  throw new RangeError(`unknown alphabet: ${alphabet}`);
}

/** Encodes the bytes of a Uint8Array as Base64 text. */
export function encode(data, alphabet = "standard") {
  const buf = Buffer.from(data.buffer, data.byteOffset, data.byteLength);
  return buf.toString(bufferEncoding(alphabet));
}

/** Decodes Base64 text strictly, and throws a SyntaxError on bad input. */
export function decode(text, alphabet = "standard") {
  const encoding = bufferEncoding(alphabet);
  // Buffer.from skips characters outside the alphabet, accepts both
  // alphabets and does not need the padding. Only the canonical text
  // encodes back to itself, so this check rejects everything else.
  const data = Buffer.from(text, encoding);
  if (data.toString(encoding) !== text) {
    throw new SyntaxError(`invalid base64 text: ${JSON.stringify(text)}`);
  }
  return new Uint8Array(data);
}
