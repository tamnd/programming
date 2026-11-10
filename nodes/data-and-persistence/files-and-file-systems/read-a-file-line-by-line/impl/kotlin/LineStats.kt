// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import java.nio.file.Path
import kotlin.io.path.useLines

/** The number of lines of a text file and the number of code points in its longest line. */
data class LineStats(val lines: Long, val longest: Long)

/**
 * Reads a UTF-8 text file line by line.
 *
 * Throws [java.nio.file.NoSuchFileException] if the file does not exist, and
 * [java.nio.charset.MalformedInputException] if the bytes are not valid UTF-8.
 */
fun lineStats(path: Path): LineStats =
    // Path.useLines reads with Files.newBufferedReader, which reports a bad byte sequence.
    path.useLines { lines ->
        var count = 0L
        var longest = 0L
        for (line in lines) {
            count++
            longest = maxOf(longest, line.codePointCount(0, line.length).toLong())
        }
        LineStats(count, longest)
    }
