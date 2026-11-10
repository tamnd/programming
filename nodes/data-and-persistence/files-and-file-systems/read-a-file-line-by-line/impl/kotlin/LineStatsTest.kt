// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

import java.nio.charset.MalformedInputException
import java.nio.file.NoSuchFileException
import java.nio.file.Path
import kotlin.io.path.writeBytes
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Test
import org.junit.jupiter.api.assertThrows
import org.junit.jupiter.api.io.TempDir

class LineStatsTest {
    @TempDir lateinit var dir: Path

    private fun write(data: ByteArray): Path = dir.resolve("input.txt").also { it.writeBytes(data) }

    private fun bytes(vararg b: Int) = ByteArray(b.size) { b[it].toByte() }

    @Test
    fun cases() {
        val cases =
            listOf(
                Triple("lf", "alpha\nbeta\ngamma\n", LineStats(3, 5)),
                Triple("blank-lines", "first\n\n\nlast line\n", LineStats(4, 9)),
                Triple("empty", "", LineStats(0, 0)),
                Triple("only-newline", "\n", LineStats(1, 0)),
                Triple("no-final-newline", "one\ntwo\nthree", LineStats(3, 5)),
                Triple("crlf", "one\r\ntwo\r\nthree\r\n", LineStats(3, 5)),
                Triple("crlf-no-final-newline", "one\r\ntwo\r\nthree", LineStats(3, 5)),
                Triple("mixed-endings", "one\r\ntwo\nthree\r\n", LineStats(3, 5)),
                Triple("whitespace", "  two spaces\ttab \n", LineStats(1, 17)),
                Triple("backslashes", "C:\\temp\\new\n", LineStats(1, 11)),
                Triple("long-line", "x".repeat(1 shl 20) + "\nend\n", LineStats(2, 1L shl 20)),
                Triple("non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", LineStats(3, 10)),
            )
        for ((name, data, want) in cases) {
            assertEquals(want, lineStats(write(data.toByteArray())), name)
        }
    }

    @Test
    fun missingFile() {
        assertThrows<NoSuchFileException> { lineStats(dir.resolve("missing.txt")) }
    }

    @Test
    fun invalidUtf8() {
        val cases =
            mapOf(
                "invalid-byte" to bytes(0x6f, 0x6b, 0x0a, 0xff, 0xfe, 0x0a),
                "truncated-at-end" to bytes(0x6f, 0x6b, 0x0a, 0x63, 0x61, 0x66, 0xc3),
                "encoded-surrogate" to bytes(0x61, 0xed, 0xa0, 0x80, 0x62, 0x0a),
                "overlong" to bytes(0xc0, 0xaf, 0x0a),
            )
        for ((name, data) in cases) {
            val path = write(data)
            assertThrows<MalformedInputException>(name) { lineStats(path) }
        }
    }
}
