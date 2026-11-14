// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

#ifndef DAY_OF_WEEK_H
#define DAY_OF_WEEK_H

// Returns the ISO 8601 day of the week: 1 for Monday to 7 for Sunday.
int day_of_week(int year, int month, int day);

// Writes the ISO 8601 week-based year and week number of a date.
void iso_week(int year, int month, int day, int *week_year, int *week);

#endif
