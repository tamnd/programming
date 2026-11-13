// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/query-xml-with-xpath/.

using System.Xml;
using System.Xml.Linq;
using System.Xml.XPath;

// The text is not well-formed XML (parse-error), or the path is not valid (bad-path).
public sealed class QueryException(string kind, Exception inner) : Exception($"{kind}: {inner.Message}", inner)
{
    public string Kind { get; } = kind;
}

public static class QueryXpath
{
    // Returns the string value of each element that the path selects from the root element, in document order.
    public static List<string> Query(string text, string path, IReadOnlyDictionary<string, string> namespaces)
    {
        XDocument doc;
        try
        {
            // A document type declaration can define entities; the contract has none, so the reader refuses it.
            var settings = new XmlReaderSettings { DtdProcessing = DtdProcessing.Prohibit };
            using var reader = XmlReader.Create(new StringReader(text), settings);
            doc = XDocument.Load(reader);
        }
        catch (XmlException e)
        {
            throw new QueryException("parse-error", e);
        }
        // The prefixes of the path are bound here, not by the document.
        var manager = new XmlNamespaceManager(new NameTable());
        foreach (var (prefix, uri) in namespaces)
        {
            manager.AddNamespace(prefix, uri);
        }
        try
        {
            // Value joins the text of all descendants and skips comments, as the XPath string value does.
            return doc.Root!.XPathSelectElements(path, manager).Select(element => element.Value).ToList();
        }
        catch (XPathException e)
        {
            throw new QueryException("bad-path", e);
        }
    }
}
