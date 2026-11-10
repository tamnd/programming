// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

using Xunit;

public sealed class SortEmployeesTests
{
    private static Employee[] E(params (string Name, string Dept, int Salary)[] rows) =>
        rows.Select(r => new Employee(r.Name, r.Dept, r.Salary)).ToArray();

    // The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
    private static readonly Employee[] Many = Enumerable.Range(0, 40)
        .Select(i => new Employee($"n{i + 1:D2}", i % 2 == 1 ? "a" : "b", 1))
        .ToArray();

    public static TheoryData<string, Employee[], Employee[]> Cases => new()
    {
        { "mixed",
          E(("Ada", "eng", 120), ("Bob", "sales", 90), ("Cy", "eng", 150), ("Di", "ops", 100)),
          E(("Cy", "eng", 150), ("Ada", "eng", 120), ("Di", "ops", 100), ("Bob", "sales", 90)) },
        { "same-salary-other-dept",
          E(("Ann", "ops", 100), ("Ben", "eng", 100), ("Cat", "hr", 100)),
          E(("Ben", "eng", 100), ("Cat", "hr", 100), ("Ann", "ops", 100)) },
        { "empty", E(), E() },
        { "single", E(("Ada", "eng", 120)), E(("Ada", "eng", 120)) },
        { "sorted",
          E(("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)),
          E(("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)) },
        { "reversed",
          E(("Bob", "ops", 90), ("Ada", "eng", 120), ("Cy", "eng", 150)),
          E(("Cy", "eng", 150), ("Ada", "eng", 120), ("Bob", "ops", 90)) },
        { "ties",
          E(("Zoe", "eng", 100), ("Xi", "hr", 80), ("Al", "eng", 100), ("Yu", "hr", 80), ("Mo", "eng", 100)),
          E(("Zoe", "eng", 100), ("Al", "eng", 100), ("Mo", "eng", 100), ("Xi", "hr", 80), ("Yu", "hr", 80)) },
        { "many-ties", Many, [.. Many.Where((_, i) => i % 2 == 1), .. Many.Where((_, i) => i % 2 == 0)] },
        { "case",
          E(("Ann", "eng", 100), ("Bea", "Sales", 90), ("Cal", "ops", 80)),
          E(("Bea", "Sales", 90), ("Ann", "eng", 100), ("Cal", "ops", 80)) },
        { "non-ascii",
          E(("Eve", "équipe", 100), ("Fay", "zoo", 90), ("Gus", "eng", 80)),
          E(("Gus", "eng", 80), ("Fay", "zoo", 90), ("Eve", "équipe", 100)) },
        { "negative",
          E(("Hal", "eng", 0), ("Ida", "eng", -5), ("Jo", "eng", 5)),
          E(("Jo", "eng", 5), ("Hal", "eng", 0), ("Ida", "eng", -5)) },
        { "overflow",
          E(("Kim", "eng", -2000000000), ("Lee", "eng", 2000000000)),
          E(("Lee", "eng", 2000000000), ("Kim", "eng", -2000000000)) },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SortsByDeptThenSalaryDescending(string name, Employee[] given, Employee[] want)
    {
        Employee[] input = [.. given];
        Assert.True(want.SequenceEqual(Employees.SortEmployees(input)), name);
        Assert.Equal(given, input);
    }
}
