// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

#ifndef CONVERT_TIME_ZONE_H
#define CONVERT_TIME_ZONE_H

// The size of "YYYY-MM-DDTHH:MM:SS+HH:MM" with its terminating null.
#define TZCONV_SIZE 26

// Writes the local time in to_zone for a local time in from_zone to out.
// Returns 0, or -1 if a zone has no file in the tz database. The function
// changes the TZ environment variable and restores it, so it is not safe
// to call from more than one thread.
int convert_time_zone(int year, int month, int day, int hour, int minute,
                      int second, const char *from_zone, const char *to_zone,
                      char out[TZCONV_SIZE]);

#endif
