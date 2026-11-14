// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

using System.Text;
using Xunit;

public sealed class AlignColumnsTests
{
    private const string Rosetta =
        "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n" +
            "are$delineated$by$a$single$'dollar'$character,$write$a$program\n" +
            "that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n" +
            "column$are$separated$by$at$least$one$space.\n" +
            "Further,$allow$for$each$word$in$a$column$to$be$either$left$\n" +
            "justified,$right$justified,$or$center$justified$within$its$column.\n";

    public static TheoryData<string, string, string, string> Cases => new()
    {
        { "rosetta-left", Rosetta, "left", "Given      a          text       file   of     many      lines,     where    fields  within  a      line\n" +
            "are        delineated by         a      single 'dollar'  character, write    a       program\n" +
            "that       aligns     each       column of     fields    by         ensuring that    words   in     each\n" +
            "column     are        separated  by     at     least     one        space.\n" +
            "Further,   allow      for        each   word   in        a          column   to      be      either left\n" +
            "justified, right      justified, or     center justified within     its      column.\n" },
        { "rosetta-right", Rosetta, "right", "     Given          a       text   file     of      many     lines,    where  fields  within      a line\n" +
            "       are delineated         by      a single  'dollar' character,    write       a program\n" +
            "      that     aligns       each column     of    fields         by ensuring    that   words     in each\n" +
            "    column        are  separated     by     at     least        one   space.\n" +
            "  Further,      allow        for   each   word        in          a   column      to      be either left\n" +
            "justified,      right justified,     or center justified     within      its column.\n" },
        { "rosetta-center", Rosetta, "center", "  Given        a         text     file    of     many      lines,    where   fields  within    a    line\n" +
            "   are     delineated     by       a    single 'dollar'  character,  write      a    program\n" +
            "   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each\n" +
            "  column      are     separated    by     at     least      one      space.\n" +
            " Further,    allow       for      each   word     in         a       column    to      be    either left\n" +
            "justified,   right    justified,   or   center justified   within     its    column.\n" },
        { "two-lines", "a$bb$c\ndd$e$fff\n", "left", "a  bb c\ndd e  fff\n" },
        { "empty", "", "left", "" },
        { "empty-line", "a$b\n" +
            "\n" +
            "ccc$d\n", "left", "a   b\n" +
            "\n" +
            "ccc d\n" },
        { "empty-field", "a$$b\nc$dd$e\n", "left", "a    b\nc dd e\n" },
        { "trailing-dollar", "a$b$\ncc$d\n", "left", "a  b\ncc d\n" },
        { "ragged", "a$b$c\nddd\n", "right", "  a b c\nddd\n" },
        { "one-column", "a\n" +
            "bbb\n" +
            "cc\n", "right", "  a\n" +
            "bbb\n" +
            " cc\n" },
        { "no-final-newline", "a$b\ncc$d", "left", "a  b\ncc d\n" },
        { "center-odd", "a$b\nbbbb$c\n", "center", " a   b\nbbbb c\n" },
        { "center-last", "x$a\nx$bbbb\n", "center", "x  a\nx bbbb\n" },
        { "non-ascii", "caf\u00e9$x\nab$y\n", "left", "caf\u00e9 x\nab   y\n" },
        { "outside-the-bmp", "\ud83d\ude00$x\nab$y\n", "left", "\ud83d\ude00  x\nab y\n" },
        { "spaces-in-field", "a b$c\nd$e\n", "left", "a b c\nd   e\n" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void AlignsTheColumns(string name, string text, string align, string want)
    {
        Assert.Equal(want, AlignColumns.Align(text, align));
    }

    [Fact]
    public void ManyLines()
    {
        var text = new StringBuilder();
        for (int i = 0; i < 100000; i++)
        {
            text.Append(i).Append("$x\n");
        }
        string[] lines = AlignColumns.Align(text.ToString(), "right").Split('\n');
        Assert.Equal(100001, lines.Length);
        Assert.Equal("    0 x", lines[0]);
        Assert.Equal("99999 x", lines[99999]);
    }

    [Fact]
    public void BadAlign()
    {
        Assert.Throws<ArgumentException>(() => AlignColumns.Align("a$b\n", "justify"));
    }
}
