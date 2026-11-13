// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-xml/.

using System.Text;
using System.Xml;

// The text is not well-formed XML, or it has a document type declaration (parse-error).
public sealed class XmlRewriteException(string kind, Exception inner) : Exception($"{kind}: {inner.Message}", inner)
{
    public string Kind { get; } = kind;
}

public static class XmlRewrite
{
    // Parses the XML text to a tree and writes it back in the canonical form of the contract.
    public static string Rewrite(string text)
    {
        // Without this, XmlDocument drops the white space between elements.
        var doc = new XmlDocument { PreserveWhitespace = true };
        try
        {
            // Prohibit is the default; it is written here so that the choice is visible.
            var settings = new XmlReaderSettings { DtdProcessing = DtdProcessing.Prohibit };
            using var reader = XmlReader.Create(new StringReader(text), settings);
            doc.Load(reader);
        }
        catch (XmlException e)
        {
            throw new XmlRewriteException("parse-error", e);
        }
        var output = new StringBuilder();
        Write(doc.DocumentElement!, output);
        return output.ToString();
    }

    private static void Write(XmlNode node, StringBuilder output)
    {
        switch (node)
        {
            // Text, CDATA sections and white space are all written as escaped text.
            case XmlCharacterData data when node is XmlText or XmlCDataSection or XmlWhitespace or XmlSignificantWhitespace:
                EscapeText(data.Data, output);
                break;
            case XmlElement element:
                output.Append('<').Append(element.Name);
                var attributes = element.Attributes.Cast<XmlAttribute>().OrderBy(AttributeKey, StringComparer.Ordinal);
                foreach (var attribute in attributes)
                {
                    output.Append(' ').Append(attribute.Name).Append("=\"");
                    EscapeAttribute(attribute.Value, output);
                    output.Append('"');
                }
                output.Append('>');
                foreach (XmlNode child in element.ChildNodes)
                {
                    Write(child, output);
                }
                output.Append("</").Append(element.Name).Append('>');
                break;
            default:
                // Comments and processing instructions are not written.
                break;
        }
    }

    // The default namespace declaration comes first, then the other declarations, then the attributes.
    private static string AttributeKey(XmlAttribute attribute) =>
        attribute.Name == "xmlns" ? "0" : (attribute.Name.StartsWith("xmlns:", StringComparison.Ordinal) ? "1" : "2") + attribute.Name;

    private static void EscapeText(string text, StringBuilder output)
    {
        foreach (char c in text)
        {
            output.Append(c switch
            {
                '&' => "&amp;",
                '<' => "&lt;",
                '>' => "&gt;",
                '\r' => "&#xD;",
                _ => c.ToString(),
            });
        }
    }

    private static void EscapeAttribute(string value, StringBuilder output)
    {
        foreach (char c in value)
        {
            output.Append(c switch
            {
                '&' => "&amp;",
                '<' => "&lt;",
                '"' => "&quot;",
                // A parser turns a literal tab, line feed or carriage return in a value into a space.
                '\t' => "&#x9;",
                '\n' => "&#xA;",
                '\r' => "&#xD;",
                _ => c.ToString(),
            });
        }
    }
}
