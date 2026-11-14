---
tag: X790Z0
kind: work
slug: falsehoods-about-time
name: Falsehoods programmers believe about time
aliases: [falsehoods programmers believe about time, more falsehoods programmers believe about time, falsehoods programmers believe about names, falsehood list, falsehoods programmers believe, awesome-falsehood]
area: data-and-values/dates-and-times
level: core
summary: >-
  A 2012 list by Noah Sussman of false beliefs about clocks, calendars and time zones. It followed a 2010 list about names and started a genre of falsehood lists.
authors: [Noah Sussman]
year: 2012
url: https://infiniteundo.com/post/25326999628/falsehoods-programmers-believe-about-time
see_also: [time-zone, tz-database, leap-seconds, calendar-arithmetic, unix-time, iso-8601, measure-elapsed-time, date-and-time-values]
sources:
- url: https://infiniteundo.com/post/25326999628/falsehoods-programmers-believe-about-time
  title: Noah Sussman, Falsehoods programmers believe about time, Infinite Undo, 2012
  locator: The list
- url: https://infiniteundo.com/rss
  title: Infinite Undo, RSS feed of the blog
  locator: Channel; dc:creator of the items
- url: https://gist.github.com/thanatos/eee17100476a336a711e
  title: A critique of Falsehoods programmers believe about time, GitHub Gist by thanatos
  locator: Intro and Ground-rules; The Falsehoods, items 1 to 34
- url: https://news.ycombinator.com/item?id=4128208
  title: Falsehoods Programmers believe about Time, Hacker News submission
  locator: Submission of 18 June 2012
- url: https://infiniteundo.com/post/25509354022/more-falsehoods-programmers-believe-about-time
  title: Noah Sussman, More falsehoods programmers believe about time, Infinite Undo, 2012
  locator: The list
- url: https://www.kalzumeus.com/2010/06/17/falsehoods-programmers-believe-about-names/
  title: Patrick McKenzie, Falsehoods Programmers Believe About Names, Kalzumeus, June 2010
  locator: Introduction; the list; closing paragraph
- url: https://github.com/kdeldycke/awesome-falsehood
  title: Kevin Deldycke and contributors, Awesome Falsehood, GitHub
  locator: Introduction; Dates and Time; Human Identity; the section list
- url: https://alexwlchan.net/2019/05/falsehoods-programmers-believe-about-unix-time/
  title: Alex Chan, Falsehoods programmers believe about Unix time, May 2019
  locator: The three falsehoods and their explanations
- url: https://www.creativedeletion.com/2015/01/28/falsehoods-programmers-date-time-zones.html
  title: Lau Taarnskov, Falsehoods programmers believe about time and time zones, Creative Deletion, January 2015
  locator: The list; closing paragraphs
- url: https://gist.github.com/timvisee/fcda9bbdff88d45cc9061606b4b923ca
  title: Falsehoods programmers believe about time, a compiled list, GitHub Gist by timvisee
  locator: Falsehoods; Sources
- url: https://www.zainrizvi.io/blog/falsehoods-programmers-believe-about-time-zones/
  title: Zain Rizvi, Falsehoods programmers believe about time zones, October 2020
  locator: Misconceptions 1 to 6
- url: https://codeblog.jonskeet.uk/2019/03/27/storing-utc-is-not-a-silver-bullet/
  title: Jon Skeet, Storing UTC is not a silver bullet, March 2019
  locator: Introduction; the options for storage
- url: https://kevin.deldycke.com/2016/falsehoods-programmers-believe-about-falsehoods-lists
  title: Kevin Deldycke, Falsehoods Programmers Believe About Falsehoods Lists
  locator: Falsehoods; Falsehood lists; Implementation
---

## Summary

"Falsehoods programmers believe about time" is a blog post by Noah Sussman on his blog Infinite Undo[^1][^2]. It is a numbered list of beliefs about clocks, calendars, time zones and timestamps. The post presents every belief as false. Many items step back from the item before them and make a weaker claim, which is also false[^3]. The post reached Hacker News on 18 June 2012[^4]. A second post, "More falsehoods programmers believe about time", added more items[^5].

The form came from an essay about personal names. In June 2010 Patrick McKenzie listed "assumptions your systems probably make about names" and wrote "All of these assumptions are wrong."[^6] The time list uses the same form for time. Later writers used it for time zones, Unix time, addresses, email, phone numbers and many other subjects[^7].

The list does not teach a technique. It teaches doubt. Each item is a test case that a date and time program can fail. This page gives the groups of items and what each group teaches. It does not repeat the lists. Read the originals for the full items.

### Contents

**The length of a day, a month and a year.** The first item is "There are always 24 hours in a day."[^3] In a local time zone, the day when daylight saving time starts has 23 hours, and the day when it ends has 25 hours[^3]. In UTC, a day with a leap second had 86,401 seconds[^8]. Months and years also vary: "February is always 28 days long."[^3] The lesson is to not compute dates by multiplying fixed lengths. Use the calendar arithmetic of a date library, and keep durations of elapsed time separate from calendar periods. See [[calendar-arithmetic]] and [[leap-seconds]].

**Weeks and years.** "A week (or a month) always begins and ends in the same year."[^3] For ISO week dates this is false: "2014 December 28th belongs to week 1 of 2015."[^9] The lesson is that the week-based year of ISO 8601 is not the calendar year. A format pattern that uses the week-based year by mistake prints the wrong year for a few days around the new year. See [[iso-8601]].

**Time zones and their rules.** The time list starts with "The machine that a program runs on will always be in the GMT time zone." and then retreats one step at a time[^3]. A compiled list of the items of both posts adds that offsets between zones change, that daylight saving time does not happen on the same date everywhere, and that it does not always move the clock by one hour[^10]. Lau Taarnskov added items from his work on a tz parser for Elixir. Rules change often: "In 2014 there were 10 updates to the Olson database released during the year."[^9] An abbreviation is not an identifier, because "CST is also used for: Cuba Summer Time, China Time, Central Standard Time (Australia)."[^9] Some local times do not exist, and some occur twice, at the transitions of daylight saving time[^9]. Offsets are not whole hours either: "Nepal likes to be at the 45 minute UTC offset."[^11] The lesson is to store a zone name of the IANA database, such as America/Los_Angeles, to keep the database current, and to decide what a program does with a skipped or repeated local time. See [[time-zone]] and [[tz-database]].

**Clocks.** About a third of the 34 items are about clocks[^3]. A system clock can be wrong, it can drift, and it can be wrong by decades. The critique of the list gives a reason for the last case: when the battery of the real-time clock dies, the clock "might also just reset to something like 1970."[^3] A server clock and a client clock never agree exactly. The lesson is to not trust a client clock, to synchronize servers with NTP, and to measure elapsed time with a monotonic clock, not with the wall clock. See [[measure-elapsed-time]].

**Unix time.** Alex Chan gave three beliefs about Unix time in 2019, such as "Unix time can never go backwards"[^8]. All three are false for one reason: "leap seconds"[^8]. Unix time gives every day 86,400 seconds, so it repeats a second when UTC inserts a leap second. The lesson is that Unix time is not a count of SI seconds and that a wall clock can step back. See [[unix-time]].

**Timestamps and formats.** "Time stamps will always be specified in the same format."[^3] "Time stamps will always have the same level of precision."[^3] "A time stamp of sufficient precision can safely be considered unique."[^3] And a numeric date such as 05/07/11 has no single reading. The lesson is to exchange timestamps in one documented format, such as RFC 3339, to state the precision, and to not use a timestamp as a key. See [[iso-8601]] and [[parse-and-format-a-date]].

**Names.** The 2010 list has 40 items[^6]. It starts with "People have exactly one canonical full name." and ends with "People have names."[^6] The items cover the number of names, their length, their order, their character set and their changes. The lesson is the same as for time: the data model of a program encodes beliefs about the world, and a person who does not fit the model cannot use the program.

### Influence

The form spread. Awesome Falsehood, a curated list kept by Kevin Deldycke on GitHub, collects falsehood lists by subject. It calls the names essay "The article that started it all." and the time list "Seminal article on dates and time."[^7] Its sections include emails, geography, phone numbers, postal addresses, typography and video games[^7].

The time list itself grew. A critique on GitHub takes the 34 items one by one and gives the reason each is false. Its author writes "I'll be answering a few with true" and notes that some items are vague[^3]. A compiled list on GitHub merges the items of both of Sussman's posts into one list of more than 100 items[^10]. Taarnskov wrote about time zones in 2015[^9], Chan about Unix time in 2019[^8], and Zain Rizvi about time zones in 2020[^11].

A common rule is to store every timestamp in UTC. Jon Skeet showed in 2019 that UTC is not enough for a future event in local time, because "time zone rules change, and that applications should expect that they will change."[^12] He recommends that a program store the local time and the zone name that the user gave, and derive the UTC instant again when the rules change[^12].

The form also has critics. Deldycke wrote a list of falsehoods about falsehood lists, with items such as "All falsehoods can be listed." and "Falsehood lists are illustrated with counter-examples."[^13] The point is that a list of false beliefs does not tell a reader which true rule to use instead. The follow-ups, such as the critique and the posts by Taarnskov and Chan, add the counterexamples and the reasons[^3][^9][^8].

## References
