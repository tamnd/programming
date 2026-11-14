// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

#ifndef DAYS_BETWEEN_H
#define DAYS_BETWEEN_H

// Returns the number of days from 1970-01-01 to a date of the proleptic
// Gregorian calendar.
long long days_from_civil(long long year, int month, int day);

// Returns the number of days from the start date to the end date.
long long days_between(int start_year, int start_month, int start_day, int end_year, int end_month, int end_day);

#endif
