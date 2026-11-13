// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-a-markdown-text/.

// Package markdowntohtml converts a text in a subset of CommonMark to HTML.
package markdowntohtml

import (
	"regexp"
	"strings"
	"unicode"
)

var (
	heading = regexp.MustCompile(`^(#{1,6})(?:[ \t]+(.*?))?[ \t]*$`)
	closing = regexp.MustCompile(`(?:^|[ \t]+)#+[ \t]*$`)
	fence   = regexp.MustCompile("^(`{3,})[ \t]*([^`]*?)[ \t]*$")
	item    = regexp.MustCompile(`^- (.*)$`)
	url     = regexp.MustCompile(`^[A-Za-z0-9\-._~:/?#=&+%]+`)
)

const punctuation = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

var escaper = strings.NewReplacer("&", "&amp;", "<", "&lt;", ">", "&gt;", `"`, "&quot;")

func escape(text string) string { return escaper.Replace(text) }

func isBlank(line string) bool { return strings.Trim(line, " \t") == "" }

func startsBlock(line string) bool {
	return isBlank(line) || fence.MatchString(line) || heading.MatchString(line) || item.MatchString(line)
}

// MarkdownToHTML returns the HTML for text.
func MarkdownToHTML(text string) string {
	var lines []string
	if text != "" {
		lines = strings.Split(strings.TrimSuffix(text, "\n"), "\n")
	}
	var out strings.Builder
	for i := 0; i < len(lines); {
		line := lines[i]
		switch {
		case isBlank(line):
			i++
		case fence.MatchString(line):
			i = codeBlock(lines, i, &out)
		case heading.MatchString(line):
			m := heading.FindStringSubmatch(line)
			level := string(rune('0' + len(m[1])))
			content := closing.ReplaceAllString(m[2], "")
			out.WriteString("<h" + level + ">" + inlines(content) + "</h" + level + ">\n")
			i++
		case item.MatchString(line):
			i = bulletList(lines, i, &out)
		default:
			i = paragraph(lines, i, &out)
		}
	}
	return out.String()
}

func codeBlock(lines []string, i int, out *strings.Builder) int {
	m := fence.FindStringSubmatch(lines[i])
	attr := ""
	if info := strings.Fields(m[2]); len(info) > 0 {
		attr = ` class="language-` + escape(info[0]) + `"`
	}
	out.WriteString("<pre><code" + attr + ">")
	for i++; i < len(lines); i++ {
		end := strings.TrimRight(lines[i], " \t")
		if strings.HasPrefix(end, m[1]) && strings.Trim(end, "`") == "" {
			i++
			break
		}
		out.WriteString(escape(lines[i]) + "\n")
	}
	out.WriteString("</code></pre>\n")
	return i
}

func bulletList(lines []string, i int, out *strings.Builder) int {
	var items []string
	loose := false
	for i < len(lines) {
		m := item.FindStringSubmatch(lines[i])
		if m == nil {
			break
		}
		items = append(items, inlines(strings.Trim(m[1], " \t")))
		i++
		j := i
		for j < len(lines) && isBlank(lines[j]) {
			j++
		}
		if j > i && j < len(lines) && item.MatchString(lines[j]) {
			loose = true
			i = j
		}
	}
	out.WriteString("<ul>\n")
	for _, it := range items {
		if loose {
			out.WriteString("<li>\n<p>" + it + "</p>\n</li>\n")
		} else {
			out.WriteString("<li>" + it + "</li>\n")
		}
	}
	out.WriteString("</ul>\n")
	return i
}

func paragraph(lines []string, i int, out *strings.Builder) int {
	start := i
	for i++; i < len(lines) && !startsBlock(lines[i]); i++ {
	}
	text := strings.TrimRight(strings.Join(lines[start:i], "\n"), " \t")
	out.WriteString("<p>" + inlines(text) + "</p>\n")
	return i
}

func isSpace(r rune) bool {
	return r == '\t' || r == '\n' || r == '\f' || r == '\r' || unicode.Is(unicode.Zs, r)
}

func isPunct(r rune) bool { return unicode.IsPunct(r) || unicode.IsSymbol(r) }

type delimiter struct {
	node, count, length int
	open, close, active bool
	opens, closes       []string
}

// inlines works on runes, so that a character outside the BMP is one element.
func inlines(text string) string {
	cs := []rune(text)
	var nodes []string
	var delims []*delimiter
	var buf strings.Builder
	flush := func() {
		if buf.Len() > 0 {
			nodes = append(nodes, buf.String())
			buf.Reset()
		}
	}
	for i := 0; i < len(cs); {
		c := cs[i]
		switch {
		case c == '\\' && i+1 < len(cs) && strings.ContainsRune(punctuation, cs[i+1]):
			buf.WriteString(escape(string(cs[i+1])))
			i += 2
		case c == '\\' && i+1 < len(cs) && cs[i+1] == '\n':
			buf.WriteString("<br />\n")
			i += 2
		case c == '\n':
			line := buf.String()
			stripped := strings.TrimRight(line, " ")
			buf.Reset()
			buf.WriteString(stripped)
			if len(line)-len(stripped) >= 2 {
				buf.WriteString("<br />\n")
			} else {
				buf.WriteString("\n")
			}
			i++
		case c == '`':
			j := i
			for j < len(cs) && cs[j] == '`' {
				j++
			}
			end := closingRun(cs, j, j-i)
			if end < 0 {
				buf.WriteString(strings.Repeat("`", j-i))
				i = j
				break
			}
			code := strings.ReplaceAll(string(cs[j:end]), "\n", " ")
			if len(code) >= 2 && code[0] == ' ' && code[len(code)-1] == ' ' && strings.Trim(code, " ") != "" {
				code = code[1 : len(code)-1]
			}
			buf.WriteString("<code>" + escape(code) + "</code>")
			i = end + (j - i)
		case c == '[':
			end, urlEnd, ok := linkEnd(cs, i)
			if !ok {
				buf.WriteString("[")
				i++
				break
			}
			label := inlines(string(cs[i+1 : end]))
			buf.WriteString(`<a href="` + escape(string(cs[end+2:urlEnd])) + `">` + label + "</a>")
			i = urlEnd + 1
		case c == '*':
			j := i
			for j < len(cs) && cs[j] == '*' {
				j++
			}
			before, after := '\n', '\n'
			if i > 0 {
				before = cs[i-1]
			}
			if j < len(cs) {
				after = cs[j]
			}
			left := !isSpace(after) && (!isPunct(after) || isSpace(before) || isPunct(before))
			right := !isSpace(before) && (!isPunct(before) || isSpace(after) || isPunct(after))
			flush()
			delims = append(delims, &delimiter{node: len(nodes), count: j - i, length: j - i, open: left, close: right, active: true})
			nodes = append(nodes, "")
			i = j
		default:
			buf.WriteString(escape(string(c)))
			i++
		}
	}
	flush()
	emphasis(delims)
	for _, d := range delims {
		s := strings.Join(d.closes, "") + strings.Repeat("*", d.count)
		for k := len(d.opens) - 1; k >= 0; k-- {
			s += d.opens[k]
		}
		nodes[d.node] = s
	}
	return strings.Join(nodes, "")
}

// closingRun returns the index of the next run of exactly n backticks at or after j, or -1.
func closingRun(cs []rune, j, n int) int {
	for j < len(cs) {
		if cs[j] != '`' {
			j++
			continue
		}
		k := j
		for k < len(cs) && cs[k] == '`' {
			k++
		}
		if k-j == n {
			return j
		}
		j = k
	}
	return -1
}

func linkEnd(cs []rune, i int) (end, urlEnd int, ok bool) {
	end = -1
	for k := i + 1; k < len(cs); k++ {
		if cs[k] == ']' {
			end = k
			break
		}
		if cs[k] == '[' {
			return 0, 0, false
		}
	}
	if end < 0 || end+1 >= len(cs) || cs[end+1] != '(' {
		return 0, 0, false
	}
	m := url.FindString(string(cs[end+2:]))
	if m == "" {
		return 0, 0, false
	}
	urlEnd = end + 2 + len([]rune(m))
	if urlEnd >= len(cs) || cs[urlEnd] != ')' {
		return 0, 0, false
	}
	return end, urlEnd, true
}

func emphasis(delims []*delimiter) {
	for k := 0; k < len(delims); {
		closer := delims[k]
		if !closer.active || !closer.close {
			k++
			continue
		}
		j := k - 1
		for ; j >= 0; j-- {
			d := delims[j]
			if !d.active || !d.open {
				continue
			}
			if (d.close || closer.open) && (d.length+closer.length)%3 == 0 && !(d.length%3 == 0 && closer.length%3 == 0) {
				continue
			}
			break
		}
		if j < 0 {
			if !closer.open {
				closer.active = false
			}
			k++
			continue
		}
		d := delims[j]
		use, tag := 1, "em"
		if d.count >= 2 && closer.count >= 2 {
			use, tag = 2, "strong"
		}
		d.count -= use
		closer.count -= use
		d.opens = append(d.opens, "<"+tag+">")
		closer.closes = append(closer.closes, "</"+tag+">")
		for m := j + 1; m < k; m++ {
			delims[m].active = false
		}
		if d.count == 0 {
			d.active = false
		}
		if closer.count == 0 {
			closer.active = false
			k++
		}
	}
}
