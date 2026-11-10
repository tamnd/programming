```csharp wrong id=interpolation
static string CommentHtml(string author, string text) =>
    $"<p class=\"comment\"><b>{author}</b>: {text}</p>";

Console.WriteLine(CommentHtml("alice", "Nice post."));
Console.WriteLine(CommentHtml("mallory", "<script>alert(1)</script>"));
```

```output for=interpolation
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: <script>alert(1)</script></p>
```

```csharp run id=encoder
using System.Text.Encodings.Web;

static string CommentHtml(string author, string text)
{
    var enc = HtmlEncoder.Default;
    return $"<p class=\"comment\"><b>{enc.Encode(author)}</b>: {enc.Encode(text)}</p>";
}

Console.WriteLine(CommentHtml("alice", "Nice post."));
Console.WriteLine(CommentHtml("mallory", "<script>alert(1)</script>"));
Console.WriteLine(System.Net.WebUtility.HtmlEncode("I <3 \"quotes\" & 'apostrophes'"));
```

```output for=encoder
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
I &lt;3 &quot;quotes&quot; &amp; &#39;apostrophes&#39;
```

- Verdict: stdlib. `System.Text.Encodings.Web` has `HtmlEncoder`, `JavaScriptEncoder` and `UrlEncoder`, one for each context. `System.Net.WebUtility.HtmlEncode` is a second HTML encoder in the base library.
- The Razor engine of ASP.NET Core encodes each value that a view writes with `@`. A value of type `HtmlString` is not encoded on output, so never make one from input.
- CodeQL query `cs/web/xss` follows input from a request to a write into the response.
