// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

const PAIRS = { ")": "(", "]": "[", "}": "{" };

// balanced returns true if the brackets of text match and nest correctly.
// An array is the stack: push adds to the end, and pop removes from the end.
export function balanced(text) {
  const stack = [];
  for (const c of text) {
    if (c === "(" || c === "[" || c === "{") {
      stack.push(c);
    } else if (c in PAIRS) {
      // pop returns undefined on an empty array, and undefined matches no
      // bracket, so an extra closing bracket gives false.
      if (stack.pop() !== PAIRS[c]) return false;
    }
  }
  return stack.length === 0;
}
