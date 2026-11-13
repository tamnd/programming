```csharp run id=json-xml-binary
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Xml;

var user = new User(42, "Ada", ["admin", "ops"]);

// JSON: text, self-describing, with the source generator of System.Text.Json.
string json = JsonSerializer.Serialize(user, AppJson.Default.User);
Console.WriteLine($"json {Encoding.UTF8.GetByteCount(json)} {json}");
User back = JsonSerializer.Deserialize(json, AppJson.Default.User)!;
Console.WriteLine($"{back.Id} {back.Name} {string.Join(",", back.Tags)}");

// XML: text, written element by element with XmlWriter.
var sb = new StringBuilder();
using (var w = XmlWriter.Create(sb, new XmlWriterSettings { OmitXmlDeclaration = true }))
{
    w.WriteStartElement("user");
    w.WriteAttributeString("id", user.Id.ToString());
    w.WriteElementString("name", user.Name);
    foreach (string t in user.Tags) w.WriteElementString("tag", t);
    w.WriteEndElement();
}
Console.WriteLine($"xml  {sb.Length} {sb}");

// Binary: BinaryWriter writes a fixed layout. Only the code knows the field order.
var ms = new MemoryStream();
using (var bw = new BinaryWriter(ms, Encoding.UTF8, leaveOpen: true))
{
    bw.Write(user.Id);
    bw.Write(user.Name);
    bw.Write(user.Tags.Length);
    foreach (string t in user.Tags) bw.Write(t);
}
byte[] bin = ms.ToArray();
Console.WriteLine($"bin  {bin.Length} {Convert.ToHexString(bin)}");

ms.Position = 0;
using var br = new BinaryReader(ms);
int id = br.ReadInt32();
string name = br.ReadString();
string[] tags = new string[br.ReadInt32()];
for (int i = 0; i < tags.Length; i++) tags[i] = br.ReadString();
Console.WriteLine($"{id} {name} {string.Join(",", tags)}");

[JsonSourceGenerationOptions(PropertyNamingPolicy = JsonKnownNamingPolicy.CamelCase)]
[JsonSerializable(typeof(User))]
partial class AppJson : JsonSerializerContext;

record User(int Id, string Name, string[] Tags);
```

```output for=json-xml-binary
json 45 {"id":42,"name":"Ada","tags":["admin","ops"]}
42 Ada admin,ops
xml  67 <user id="42"><name>Ada</name><tag>admin</tag><tag>ops</tag></user>
bin  22 2A00000003416461020000000561646D696E036F7073
42 Ada admin,ops
```

- Verdict: stdlib. `System.Text.Json` writes the record as JSON, here with a source-generated context and camel-case names, and reads it back. `XmlWriter` writes XML one element at a time. `BinaryWriter` writes an `int` as four little-endian bytes and a string as a length byte and the UTF-8 bytes, so the record takes 22 bytes, against 45 for the JSON. The hex dump has no field names: the reader must call `ReadInt32`, `ReadString` and `ReadInt32` in the same order as the writer.
