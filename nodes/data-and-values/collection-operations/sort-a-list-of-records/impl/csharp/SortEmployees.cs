// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

/// <summary>A record of an employee.</summary>
public sealed record Employee(string Name, string Dept, int Salary);

public static class Employees
{
    /// <summary>
    /// Returns a new list of the records by Dept, then by Salary from high to low.
    /// OrderBy is a stable sort, so records with equal keys keep their input order.
    /// </summary>
    public static List<Employee> SortEmployees(IEnumerable<Employee> employees) =>
        // Give StringComparer.Ordinal: the default comparer of string uses the current culture.
        employees
            .OrderBy(e => e.Dept, StringComparer.Ordinal)
            .ThenByDescending(e => e.Salary)
            .ToList();
}
