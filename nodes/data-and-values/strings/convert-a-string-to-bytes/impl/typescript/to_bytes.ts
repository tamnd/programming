// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

// toBytes returns the UTF-8 bytes of s. TextEncoder always encodes to UTF-8.
export function toBytes(s: string): Uint8Array {
  return new TextEncoder().encode(s);
}
