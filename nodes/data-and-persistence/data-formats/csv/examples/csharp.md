```csharp run id=text-field-parser
using Microsoft.VisualBasic.FileIO;

string text = "id,name,note\r\n7,\"Smith, Ann\",\"said \"\"hi\"\"\r\nand left\"\r\n8,  Bo  ,\r\n";
using (var parser = new TextFieldParser(new StringReader(text)))
{
    parser.SetDelimiters(",");
    parser.HasFieldsEnclosedInQuotes = true;
    while (!parser.EndOfData)
    {
        string[] fields = parser.ReadFields()!;
        Console.WriteLine(string.Join(" | ", fields.Select(f => "[" + f.Replace("\r\n", "\\r\\n") + "]")));
    }
}

using (var parser = new TextFieldParser(new StringReader("8,  Bo  ,\r\n")))
{
    parser.SetDelimiters(",");
    parser.TrimWhiteSpace = false;
    Console.WriteLine(string.Join(" | ", parser.ReadFields()!.Select(f => "[" + f + "]")));
}

using (var parser = new TextFieldParser(new StringReader("1,ok\r\n7,\"Smith\" Ann\r\n9,end\r\n")))
{
    parser.SetDelimiters(",");
    while (!parser.EndOfData)
    {
        try
        {
            Console.WriteLine(string.Join(" | ", parser.ReadFields()!));
        }
        catch (MalformedLineException e)
        {
            Console.WriteLine($"line {e.LineNumber}: {parser.ErrorLine}");
        }
    }
}
```

```output for=text-field-parser
[id] | [name] | [note]
[7] | [Smith, Ann] | [said "hi"\r\nand left]
[8] | [Bo] | []
[8] | [  Bo  ] | []
1 | ok
line 2: 7,"Smith" Ann
9 | end
```

```csharp run id=write
using System.Text;

static string Field(string value) =>
    value.IndexOfAny(['"', ',', '\r', '\n']) >= 0 ? "\"" + value.Replace("\"", "\"\"") + "\"" : value;

string[][] rows = [["id", "name", "note"], ["7", "Smith, Ann", "said \"hi\""], ["8", "Bo", ""]];
var csv = new StringBuilder();
foreach (string[] row in rows)
{
    csv.Append(string.Join(",", row.Select(Field))).Append("\r\n");
}
Console.Write(csv.ToString().Replace("\r\n", "\\r\\n\n"));
```

```output for=write
id,name,note\r\n
7,"Smith, Ann","said ""hi"""\r\n
8,Bo,\r\n
```

- Verdict: stdlib. `TextFieldParser` in `Microsoft.VisualBasic.FileIO` ships with .NET and reads delimited and fixed-width text. It has no writer, so the second example writes CSV with a short function.
- `TrimWhiteSpace` is true by default, so the parser removes the spaces around `Bo`. RFC 4180 says that spaces are part of the field.
- A bad line raises `MalformedLineException`. `ErrorLine` holds the text of the line, and the parser goes on with the next line.
