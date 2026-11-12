// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

// fatal: true makes decode throw a TypeError for bytes that are not well-formed
// UTF-8. ignoreBOM: true keeps a leading U+FEFF in the result.
const decoder = new TextDecoder("utf-8", { fatal: true, ignoreBOM: true });

// encodeUtf8 returns the UTF-8 bytes of the code point cp.
export function encodeUtf8(cp: number): number[] {
  return Array.from(new TextEncoder().encode(String.fromCodePoint(cp)));
}

// decodeUtf8 returns the code point of the one UTF-8 sequence in b, or -1.
export function decodeUtf8(b: number[]): number {
  let s;
  try {
    s = decoder.decode(new Uint8Array(b));
  } catch {
    return -1;
  }
  const points = Array.from(s);
  return points.length === 1 ? points[0].codePointAt(0)! : -1;
}
