// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

import assert from "node:assert/strict";
import { Buffer } from "node:buffer";
import { test } from "node:test";

import { decode, encode, type Alphabet } from "./b64.ts";

// The bytes are written in hexadecimal.
const encodeCases: [string, string, Alphabet, string][] = [
  ["rfc-f", "66", "standard", "Zg=="],
  ["rfc-fo", "666f", "standard", "Zm8="],
  ["rfc-foo", "666f6f", "standard", "Zm9v"],
  ["rfc-foob", "666f6f62", "standard", "Zm9vYg=="],
  ["rfc-fooba", "666f6f6261", "standard", "Zm9vYmE="],
  ["rfc-foobar", "666f6f626172", "standard", "Zm9vYmFy"],
  ["hello-standard", "48656c6c6f2c20776f726c6421", "standard", "SGVsbG8sIHdvcmxkIQ=="],
  ["hello-url", "48656c6c6f2c20776f726c6421", "url", "SGVsbG8sIHdvcmxkIQ"],
  ["empty-standard", "", "standard", ""],
  ["empty-url", "", "url", ""],
  ["two-pad-chars", "00", "standard", "AA=="],
  ["one-pad-char", "0000", "standard", "AAA="],
  ["zero-bytes", "000000", "standard", "AAAA"],
  ["plus-and-slash", "fbff", "standard", "+/8="],
  ["minus-and-underscore", "fbff", "url", "-_8"],
  ["all-62-standard", "fbefbe", "standard", "++++"],
  ["all-62-url", "fbefbe", "url", "----"],
  ["all-63-standard", "ffffff", "standard", "////"],
  ["all-63-url", "ffffff", "url", "____"],
  ["url-one-byte", "66", "url", "Zg"],
  ["url-two-bytes", "666f", "url", "Zm8"],
  ["non-ascii-standard", "c3a9e282acf09f9982", "standard", "w6nigqzwn5mC"],
  ["non-ascii-url", "c3a9e282acf09f9982", "url", "w6nigqzwn5mC"],
];

const decodeCases: [string, string, Alphabet, string][] = [
  ["rfc-empty", "", "standard", ""],
  ["rfc-f", "Zg==", "standard", "66"],
  ["rfc-fo", "Zm8=", "standard", "666f"],
  ["rfc-foo", "Zm9v", "standard", "666f6f"],
  ["rfc-foob", "Zm9vYg==", "standard", "666f6f62"],
  ["rfc-fooba", "Zm9vYmE=", "standard", "666f6f6261"],
  ["rfc-foobar", "Zm9vYmFy", "standard", "666f6f626172"],
  ["empty-url", "", "url", ""],
  ["url-foob", "Zm9vYg", "url", "666f6f62"],
  ["url-fooba", "Zm9vYmE", "url", "666f6f6261"],
  ["url-foobar", "Zm9vYmFy", "url", "666f6f626172"],
  ["plus-and-slash", "+/8=", "standard", "fbff"],
  ["minus-and-underscore", "-_8", "url", "fbff"],
  ["all-63-url", "____", "url", "ffffff"],
  ["zero-bytes", "AAAA", "standard", "000000"],
  ["non-ascii-standard", "w6nigqzwn5mC", "standard", "c3a9e282acf09f9982"],
  ["non-ascii-url", "w6nigqzwn5mC", "url", "c3a9e282acf09f9982"],
];

const errorCases: [string, string, Alphabet][] = [
  ["plus-in-url", "+/8", "url"],
  ["minus-in-standard", "-_8=", "standard"],
  ["bang", "Zm9v!", "standard"],
  ["non-ascii-char", "Zm9vé===", "standard"],
  ["space", "Zm9v YmFy", "standard"],
  ["line-feed", "Zm9v\nYmFy", "standard"],
  ["trailing-crlf", "Zm9vYmFy\r\n", "standard"],
  ["line-feed-url", "Zm9v\nYmFy", "url"],
  ["missing-padding", "Zm8", "standard"],
  ["missing-one-pad", "Zg=", "standard"],
  ["excess-padding", "Zm9v=", "standard"],
  ["padding-in-middle", "Zm=9", "standard"],
  ["three-pads", "Z===", "standard"],
  ["only-padding", "====", "standard"],
  ["padding-then-data", "Zg==Zm8=", "standard"],
  ["padding-in-url", "Zm8=", "url"],
  ["length-1-mod-4-standard", "Zm9vY", "standard"],
  ["length-1-mod-4-url", "Zm9vY", "url"],
  ["one-char-url", "Z", "url"],
  ["pad-bits-one-byte", "QR==", "standard"],
  ["pad-bits-two-bytes", "Zm9=", "standard"],
  ["pad-bits-url", "Zm9", "url"],
];

const fromHex = (hex: string) => new Uint8Array(Buffer.from(hex, "hex"));
const toHex = (data: Uint8Array) => Buffer.from(data).toString("hex");

for (const [name, data, alphabet, want] of encodeCases) {
  test(`encode ${name}`, () => {
    assert.equal(encode(fromHex(data), alphabet), want);
  });
  test(`round trip ${name}`, () => {
    assert.equal(toHex(decode(encode(fromHex(data), alphabet), alphabet)), data);
  });
}

for (const [name, text, alphabet, want] of decodeCases) {
  test(`decode ${name}`, () => {
    assert.equal(toHex(decode(text, alphabet)), want);
  });
}

for (const [name, text, alphabet] of errorCases) {
  test(`invalid ${name}`, () => {
    assert.throws(() => decode(text, alphabet), SyntaxError);
  });
}
