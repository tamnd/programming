// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-xml/.

// Package xmlrewrite parses an XML text and writes it back in a canonical form.
package xmlrewrite

import (
	"encoding/xml"
	"errors"
	"fmt"
	"io"
	"sort"
	"strings"
)

// ErrParse is the error for a text that is not well-formed XML or that has a document type declaration.
var ErrParse = errors.New("parse-error")

// node is an element or a piece of text of the tree.
type node struct {
	name     string // the qualified name of an element, as in the text; empty for text
	attrs    []attr
	children []*node
	text     string
}

type attr struct{ name, value string }

func qname(n xml.Name) string {
	// RawToken keeps the prefix of the text in Space.
	if n.Space == "" {
		return n.Local
	}
	return n.Space + ":" + n.Local
}

// parse reads the text into a tree. RawToken keeps the prefixes, but it does not check the structure, so parse checks the end tags, the duplicate attributes and the content outside the root.
func parse(text string) (*node, error) {
	dec := xml.NewDecoder(strings.NewReader(text))
	var root *node
	var stack []*node
	for {
		tok, err := dec.RawToken()
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, fmt.Errorf("%w: %v", ErrParse, err)
		}
		switch t := tok.(type) {
		case xml.StartElement:
			if root != nil && len(stack) == 0 {
				return nil, fmt.Errorf("%w: a second root element", ErrParse)
			}
			n := &node{name: qname(t.Name)}
			seen := map[string]bool{}
			for _, a := range t.Attr {
				name := qname(a.Name)
				if seen[name] {
					return nil, fmt.Errorf("%w: the attribute %s occurs twice", ErrParse, name)
				}
				seen[name] = true
				n.attrs = append(n.attrs, attr{name, a.Value})
			}
			if root == nil {
				root = n
			} else {
				top := stack[len(stack)-1]
				top.children = append(top.children, n)
			}
			stack = append(stack, n)
		case xml.EndElement:
			if len(stack) == 0 || stack[len(stack)-1].name != qname(t.Name) {
				return nil, fmt.Errorf("%w: the end tag %s does not match", ErrParse, qname(t.Name))
			}
			stack = stack[:len(stack)-1]
		case xml.CharData:
			if len(stack) == 0 {
				if strings.Trim(string(t), " \t\r\n") != "" {
					return nil, fmt.Errorf("%w: text outside the root element", ErrParse)
				}
				continue
			}
			top := stack[len(stack)-1]
			top.children = append(top.children, &node{text: string(t)})
		case xml.Directive:
			// The decoder never loads an external entity, but the contract refuses every document type declaration.
			return nil, fmt.Errorf("%w: a document type declaration is not allowed", ErrParse)
		}
		// Comments and processing instructions are skipped.
	}
	if root == nil || len(stack) != 0 {
		return nil, fmt.Errorf("%w: the root element is missing or not closed", ErrParse)
	}
	return root, nil
}

var textEscaper = strings.NewReplacer("&", "&amp;", "<", "&lt;", ">", "&gt;", "\r", "&#xD;")

// A parser turns a literal tab, line feed or carriage return in a value into a space, so these must be references.
var attrEscaper = strings.NewReplacer("&", "&amp;", "<", "&lt;", `"`, "&quot;", "\t", "&#x9;", "\n", "&#xA;", "\r", "&#xD;")

// attrKey puts the default namespace declaration first, then the other declarations, then the attributes.
func attrKey(name string) string {
	switch {
	case name == "xmlns":
		return "0"
	case strings.HasPrefix(name, "xmlns:"):
		return "1" + name
	}
	return "2" + name
}

func write(n *node, b *strings.Builder) {
	if n.name == "" {
		b.WriteString(textEscaper.Replace(n.text))
		return
	}
	sort.Slice(n.attrs, func(i, j int) bool { return attrKey(n.attrs[i].name) < attrKey(n.attrs[j].name) })
	b.WriteString("<" + n.name)
	for _, a := range n.attrs {
		b.WriteString(" " + a.name + `="` + attrEscaper.Replace(a.value) + `"`)
	}
	b.WriteString(">")
	for _, c := range n.children {
		write(c, b)
	}
	b.WriteString("</" + n.name + ">")
}

// XMLRewrite parses the XML text to a tree and writes it back in the canonical form of the contract.
func XMLRewrite(text string) (string, error) {
	root, err := parse(text)
	if err != nil {
		return "", err
	}
	var b strings.Builder
	write(root, &b)
	return b.String(), nil
}
