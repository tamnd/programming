# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-a-markdown-text/.

"""Convert a text in a subset of CommonMark to HTML."""

import re
import unicodedata

HEADING = re.compile(r"(#{1,6})(?:[ \t]+(.*?))?[ \t]*$")
CLOSING = re.compile(r"(?:^|[ \t]+)#+[ \t]*$")
FENCE = re.compile(r"(`{3,})[ \t]*([^`]*?)[ \t]*$")
ITEM = re.compile(r"- (.*)$")
URL = re.compile(r"[A-Za-z0-9\-._~:/?#=&+%]+")
PUNCTUATION = set("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~")


def escape(text):
    return (text.replace("&", "&amp;").replace("<", "&lt;")
            .replace(">", "&gt;").replace('"', "&quot;"))


def markdown_to_html(text):
    lines = text.removesuffix("\n").split("\n") if text else []
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.strip(" \t") == "":
            i += 1
        elif FENCE.match(line):
            i = code_block(lines, i, out)
        elif HEADING.match(line):
            m = HEADING.match(line)
            level = len(m.group(1))
            content = CLOSING.sub("", m.group(2) or "")
            out.append(f"<h{level}>{inlines(content)}</h{level}>\n")
            i += 1
        elif ITEM.match(line):
            i = bullet_list(lines, i, out)
        else:
            i = paragraph(lines, i, out)
    return "".join(out)


def starts_block(line):
    return (line.strip(" \t") == "" or FENCE.match(line) is not None
            or HEADING.match(line) is not None or ITEM.match(line) is not None)


def code_block(lines, i, out):
    m = FENCE.match(lines[i])
    fence = m.group(1)
    info = m.group(2).split()
    attr = f' class="language-{escape(info[0])}"' if info else ""
    body = []
    i += 1
    while i < len(lines):
        close = lines[i].rstrip(" \t")
        if close.startswith(fence) and close.strip("`") == "":
            i += 1
            break
        body.append(escape(lines[i]) + "\n")
        i += 1
    out.append(f"<pre><code{attr}>{''.join(body)}</code></pre>\n")
    return i


def bullet_list(lines, i, out):
    items = []
    loose = False
    while i < len(lines):
        m = ITEM.match(lines[i])
        if m is None:
            break
        items.append(inlines(m.group(1).strip(" \t")))
        i += 1
        j = i
        while j < len(lines) and lines[j].strip(" \t") == "":
            j += 1
        if j > i and j < len(lines) and ITEM.match(lines[j]):
            loose = True
            i = j
    out.append("<ul>\n")
    for item in items:
        out.append(f"<li>\n<p>{item}</p>\n</li>\n" if loose else f"<li>{item}</li>\n")
    out.append("</ul>\n")
    return i


def paragraph(lines, i, out):
    start = i
    i += 1
    while i < len(lines) and not starts_block(lines[i]):
        i += 1
    text = "\n".join(lines[start:i]).rstrip(" \t")
    out.append(f"<p>{inlines(text)}</p>\n")
    return i


def is_space(c):
    return c in "\t\n\f\r" or unicodedata.category(c) == "Zs"


def is_punct(c):
    return unicodedata.category(c)[0] in "PS"


def inlines(text):
    nodes = []
    delims = []
    buf = []

    def flush():
        if buf:
            nodes.append("".join(buf))
            buf.clear()

    i = 0
    while i < len(text):
        c = text[i]
        if c == "\\" and i + 1 < len(text) and text[i + 1] in PUNCTUATION:
            buf.append(escape(text[i + 1]))
            i += 2
        elif c == "\\" and i + 1 < len(text) and text[i + 1] == "\n":
            buf.append("<br />\n")
            i += 2
        elif c == "\n":
            line = "".join(buf)
            stripped = line.rstrip(" ")
            buf[:] = [stripped, "<br />\n" if len(line) - len(stripped) >= 2 else "\n"]
            i += 1
        elif c == "`":
            j = i
            while j < len(text) and text[j] == "`":
                j += 1
            run = text[i:j]
            close = re.compile(r"(?<!`)" + run + r"(?!`)").search(text, j)
            if close is None:
                buf.append(run)
                i = j
            else:
                code = text[j:close.start()].replace("\n", " ")
                if len(code) >= 2 and code[0] == " " and code[-1] == " " and code.strip(" "):
                    code = code[1:-1]
                buf.append(f"<code>{escape(code)}</code>")
                i = close.end()
        elif c == "[":
            end = link_end(text, i)
            if end is None:
                buf.append("[")
                i += 1
            else:
                close, url_end = end
                label = inlines(text[i + 1:close])
                url = escape(text[close + 2:url_end])
                buf.append(f'<a href="{url}">{label}</a>')
                i = url_end + 1
        elif c == "*":
            j = i
            while j < len(text) and text[j] == "*":
                j += 1
            before = text[i - 1] if i > 0 else "\n"
            after = text[j] if j < len(text) else "\n"
            left = not is_space(after) and (
                not is_punct(after) or is_space(before) or is_punct(before))
            right = not is_space(before) and (
                not is_punct(before) or is_space(after) or is_punct(after))
            flush()
            delims.append({"node": len(nodes), "count": j - i, "length": j - i,
                           "open": left, "close": right, "active": True,
                           "opens": [], "closes": []})
            nodes.append(None)
            i = j
        else:
            buf.append(escape(c))
            i += 1
    flush()
    emphasis(delims)
    for d in delims:
        nodes[d["node"]] = ("".join(d["closes"]) + "*" * d["count"]
                            + "".join(reversed(d["opens"])))
    return "".join(nodes)


def link_end(text, i):
    close = text.find("]", i + 1)
    if close < 0 or "[" in text[i + 1:close] or not text.startswith("(", close + 1):
        return None
    m = URL.match(text, close + 2)
    if m is None or not text.startswith(")", m.end()):
        return None
    return close, m.end()


def emphasis(delims):
    k = 0
    while k < len(delims):
        closer = delims[k]
        if not closer["active"] or not closer["close"]:
            k += 1
            continue
        opener = None
        for j in range(k - 1, -1, -1):
            d = delims[j]
            if not d["active"] or not d["open"]:
                continue
            if (d["close"] or closer["open"]) and (d["length"] + closer["length"]) % 3 == 0 \
                    and not (d["length"] % 3 == 0 and closer["length"] % 3 == 0):
                continue
            opener = (j, d)
            break
        if opener is None:
            if not closer["open"]:
                closer["active"] = False
            k += 1
            continue
        j, d = opener
        use = 2 if d["count"] >= 2 and closer["count"] >= 2 else 1
        tag = "strong" if use == 2 else "em"
        d["count"] -= use
        closer["count"] -= use
        d["opens"].append(f"<{tag}>")
        closer["closes"].append(f"</{tag}>")
        for m in range(j + 1, k):
            delims[m]["active"] = False
        if d["count"] == 0:
            d["active"] = False
        if closer["count"] == 0:
            closer["active"] = False
            k += 1
