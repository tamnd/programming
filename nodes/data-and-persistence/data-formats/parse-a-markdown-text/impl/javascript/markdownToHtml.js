// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

// Convert a text in a subset of CommonMark to HTML.

const HEADING = /^(#{1,6})(?:[ \t]+(.*?))?[ \t]*$/;
const CLOSING = /(?:^|[ \t]+)#+[ \t]*$/;
const FENCE = /^(`{3,})[ \t]*([^`]*?)[ \t]*$/;
const ITEM = /^- (.*)$/;
const URL = /^[A-Za-z0-9\-._~:/?#=&+%]+/;
const PUNCTUATION = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

const escape = (text) =>
  text.replaceAll("&", "&amp;").replaceAll("<", "&lt;").replaceAll(">", "&gt;").replaceAll('"', "&quot;");

const isBlank = (line) => line.replace(/[ \t]/g, "") === "";

export function markdownToHtml(text) {
  const lines = text === "" ? [] : text.replace(/\n$/, "").split("\n");
  const out = [];
  let i = 0;
  while (i < lines.length) {
    const line = lines[i];
    let m;
    if (isBlank(line)) {
      i += 1;
    } else if (FENCE.test(line)) {
      i = codeBlock(lines, i, out);
    } else if ((m = HEADING.exec(line))) {
      const level = m[1].length;
      const content = (m[2] ?? "").replace(CLOSING, "");
      out.push(`<h${level}>${inlines(content)}</h${level}>\n`);
      i += 1;
    } else if (ITEM.test(line)) {
      i = bulletList(lines, i, out);
    } else {
      i = paragraph(lines, i, out);
    }
  }
  return out.join("");
}

const startsBlock = (line) => isBlank(line) || FENCE.test(line) || HEADING.test(line) || ITEM.test(line);

function codeBlock(lines, i, out) {
  const m = FENCE.exec(lines[i]);
  const fence = m[1];
  const info = m[2].split(/[ \t]+/).filter((w) => w !== "");
  const attr = info.length > 0 ? ` class="language-${escape(info[0])}"` : "";
  const body = [];
  i += 1;
  while (i < lines.length) {
    const close = lines[i].replace(/[ \t]+$/, "");
    if (close.startsWith(fence) && close.replaceAll("`", "") === "") {
      i += 1;
      break;
    }
    body.push(escape(lines[i]) + "\n");
    i += 1;
  }
  out.push(`<pre><code${attr}>${body.join("")}</code></pre>\n`);
  return i;
}

function bulletList(lines, i, out) {
  const items = [];
  let loose = false;
  while (i < lines.length) {
    const m = ITEM.exec(lines[i]);
    if (m === null) break;
    items.push(inlines(m[1].replace(/^[ \t]+|[ \t]+$/g, "")));
    i += 1;
    let j = i;
    while (j < lines.length && isBlank(lines[j])) j += 1;
    if (j > i && j < lines.length && ITEM.test(lines[j])) {
      loose = true;
      i = j;
    }
  }
  out.push("<ul>\n");
  for (const item of items) {
    out.push(loose ? `<li>\n<p>${item}</p>\n</li>\n` : `<li>${item}</li>\n`);
  }
  out.push("</ul>\n");
  return i;
}

function paragraph(lines, i, out) {
  const start = i;
  i += 1;
  while (i < lines.length && !startsBlock(lines[i])) i += 1;
  const text = lines.slice(start, i).join("\n").replace(/[ \t]+$/, "");
  out.push(`<p>${inlines(text)}</p>\n`);
  return i;
}

const isSpace = (c) => /^[\t\n\f\r\p{Zs}]$/u.test(c);
const isPunct = (c) => /^[\p{P}\p{S}]$/u.test(c);

// cs is an array of code points, so that a character outside the BMP is one element.
function inlines(text) {
  const cs = Array.from(text);
  const nodes = [];
  const delims = [];
  let buf = [];
  const flush = () => {
    if (buf.length > 0) {
      nodes.push(buf.join(""));
      buf = [];
    }
  };
  let i = 0;
  while (i < cs.length) {
    const c = cs[i];
    if (c === "\\" && i + 1 < cs.length && PUNCTUATION.includes(cs[i + 1])) {
      buf.push(escape(cs[i + 1]));
      i += 2;
    } else if (c === "\\" && cs[i + 1] === "\n") {
      buf.push("<br />\n");
      i += 2;
    } else if (c === "\n") {
      const line = buf.join("");
      const stripped = line.replace(/ +$/, "");
      buf = [stripped, line.length - stripped.length >= 2 ? "<br />\n" : "\n"];
      i += 1;
    } else if (c === "`") {
      let j = i;
      while (j < cs.length && cs[j] === "`") j += 1;
      const close = closingRun(cs, j, j - i);
      if (close < 0) {
        buf.push("`".repeat(j - i));
        i = j;
      } else {
        let code = cs.slice(j, close).join("").replaceAll("\n", " ");
        if (code.length >= 2 && code.startsWith(" ") && code.endsWith(" ") && code.trim() !== "") {
          code = code.slice(1, -1);
        }
        buf.push(`<code>${escape(code)}</code>`);
        i = close + (j - i);
      }
    } else if (c === "[") {
      const end = linkEnd(cs, i);
      if (end === null) {
        buf.push("[");
        i += 1;
      } else {
        const [close, urlEnd] = end;
        const label = inlines(cs.slice(i + 1, close).join(""));
        const url = escape(cs.slice(close + 2, urlEnd).join(""));
        buf.push(`<a href="${url}">${label}</a>`);
        i = urlEnd + 1;
      }
    } else if (c === "*") {
      let j = i;
      while (j < cs.length && cs[j] === "*") j += 1;
      const before = i > 0 ? cs[i - 1] : "\n";
      const after = j < cs.length ? cs[j] : "\n";
      const left = !isSpace(after) && (!isPunct(after) || isSpace(before) || isPunct(before));
      const right = !isSpace(before) && (!isPunct(before) || isSpace(after) || isPunct(after));
      flush();
      delims.push({ node: nodes.length, count: j - i, length: j - i, open: left, close: right, active: true, opens: [], closes: [] });
      nodes.push(null);
      i = j;
    } else {
      buf.push(escape(c));
      i += 1;
    }
  }
  flush();
  emphasis(delims);
  for (const d of delims) {
    nodes[d.node] = d.closes.join("") + "*".repeat(d.count) + d.opens.toReversed().join("");
  }
  return nodes.join("");
}

// closingRun returns the index of the next run of exactly n backticks at or after j, or -1.
function closingRun(cs, j, n) {
  while (j < cs.length) {
    if (cs[j] !== "`") {
      j += 1;
      continue;
    }
    let k = j;
    while (k < cs.length && cs[k] === "`") k += 1;
    if (k - j === n) return j;
    j = k;
  }
  return -1;
}

function linkEnd(cs, i) {
  const close = cs.indexOf("]", i + 1);
  if (close < 0 || cs.slice(i + 1, close).includes("[") || cs[close + 1] !== "(") return null;
  const m = URL.exec(cs.slice(close + 2).join(""));
  if (m === null) return null;
  const urlEnd = close + 2 + Array.from(m[0]).length;
  return cs[urlEnd] === ")" ? [close, urlEnd] : null;
}

function emphasis(delims) {
  let k = 0;
  while (k < delims.length) {
    const closer = delims[k];
    if (!closer.active || !closer.close) {
      k += 1;
      continue;
    }
    let j = k - 1;
    for (; j >= 0; j -= 1) {
      const d = delims[j];
      if (!d.active || !d.open) continue;
      if ((d.close || closer.open) && (d.length + closer.length) % 3 === 0 && !(d.length % 3 === 0 && closer.length % 3 === 0)) continue;
      break;
    }
    if (j < 0) {
      if (!closer.open) closer.active = false;
      k += 1;
      continue;
    }
    const d = delims[j];
    const use = d.count >= 2 && closer.count >= 2 ? 2 : 1;
    const tag = use === 2 ? "strong" : "em";
    d.count -= use;
    closer.count -= use;
    d.opens.push(`<${tag}>`);
    closer.closes.push(`</${tag}>`);
    for (let m = j + 1; m < k; m += 1) delims[m].active = false;
    if (d.count === 0) d.active = false;
    if (closer.count === 0) {
      closer.active = false;
      k += 1;
    }
  }
}
