// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

const PAIRS: Readonly<Record<string, string>> = { ")": "(", "]": "[", "}": "{" };

// balanced returns true if the brackets of text match and nest correctly.
export function balanced(text: string): boolean {
  const stack: string[] = [];
  for (const c of text) {
    if (c === "(" || c === "[" || c === "{") {
      stack.push(c);
    } else if (c in PAIRS) {
      // The type of pop() is string | undefined: the stack can be empty.
      const top: string | undefined = stack.pop();
      if (top !== PAIRS[c]) return false;
    }
  }
  return stack.length === 0;
}
