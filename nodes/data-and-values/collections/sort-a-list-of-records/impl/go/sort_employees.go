// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

// Package records sorts records of employees.
package records

import (
	"cmp"
	"slices"
	"strings"
)

// Employee is a record of an employee.
type Employee struct {
	Name   string
	Dept   string
	Salary int32
}

// SortEmployees returns a new slice of the records by Dept, then by Salary
// from high to low. Records with equal keys keep their input order.
func SortEmployees(employees []Employee) []Employee {
	sorted := slices.Clone(employees)
	// slices.SortFunc is not stable. SortStableFunc is.
	slices.SortStableFunc(sorted, func(a, b Employee) int {
		// cmp.Or returns the first result that is not 0.
		// strings.Compare compares bytes, so UTF-8 text sorts by code point.
		return cmp.Or(
			strings.Compare(a.Dept, b.Dept),
			cmp.Compare(b.Salary, a.Salary), // b before a: high to low
		)
	})
	return sorted
}
