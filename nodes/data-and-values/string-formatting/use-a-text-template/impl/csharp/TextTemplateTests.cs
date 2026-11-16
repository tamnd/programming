// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

using System;
using System.Collections.Generic;
using Xunit;

public sealed class TextTemplateTests
{
    public static TheoryData<string, string, string[], string[], string> Cases => new()
    {
        { "greeting", "Hello, $name!", new string[] { "name" }, new string[] { "Ada" }, "Hello, Ada!" },
        { "braces", "${item}s cost $$${price}", new string[] { "item", "price" }, new string[] { "Apple", "3" }, "Apples cost $3" },
        { "two-uses", "$a and $a again", new string[] { "a" }, new string[] { "x" }, "x and x again" },
        { "no-placeholders", "plain text", Array.Empty<string>(), Array.Empty<string>(), "plain text" },
        { "empty-template", "", new string[] { "x" }, new string[] { "1" }, "" },
        { "dollar-escape", "$$name", new string[] { "name" }, new string[] { "Ada" }, "$name" },
        { "adjacent-text", "${n}th", new string[] { "n" }, new string[] { "4" }, "4th" },
        { "value-with-dollar", "$v", new string[] { "v" }, new string[] { "$x and ${y}" }, "$x and ${y}" },
        { "empty-value", "[$v]", new string[] { "v" }, new string[] { "" }, "[]" },
        { "underscore-name", "$first_name $last_name", new string[] { "first_name", "last_name" }, new string[] { "Grace", "Hopper" }, "Grace Hopper" },
        { "name-ends-at-punct", "$a.$b", new string[] { "a", "b" }, new string[] { "1", "2" }, "1.2" },
        { "unused-value", "$a", new string[] { "a", "b" }, new string[] { "1", "2" }, "1" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Substitute(string name, string template, string[] names, string[] values, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, TextTemplate.Substitute(template, names, values));
    }

    [Fact]
    public void UnknownName()
    {
        Assert.Throws<KeyNotFoundException>(() => TextTemplate.Substitute("Hi $who", new[] { "name" }, new[] { "Ada" }));
    }
}
