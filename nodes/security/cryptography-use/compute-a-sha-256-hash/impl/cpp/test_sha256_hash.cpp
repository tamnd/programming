// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "sha256_hash.hpp"

namespace {

int failures = 0;

void check(std::string_view name, const std::string& got, std::string_view want) {
    if (got != want) {
        std::fprintf(stderr, "case %.*s failed: got %s\n", static_cast<int>(name.size()), name.data(),
                     got.c_str());
        ++failures;
    }
}

struct Case {
    std::string name, data, want;
};

}  // namespace

int main() {
    using namespace std::string_literals;
    const std::vector<Case> text_cases = {
        {"abc", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
        {"two-block-56", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"},
        {"fox", "The quick brown fox jumps over the lazy dog",
         "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"},
        {"empty", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
        {"a-55", std::string(55, 'a'), "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"},
        {"a-56", std::string(56, 'a'), "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"},
        {"a-63", std::string(63, 'a'), "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34"},
        {"a-64", std::string(64, 'a'), "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb"},
        {"multi-block-112",
         "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
         "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
         "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1"},
        {"non-ascii", "na\xc3\xafve caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e \xf0\x9f\x99\x82",
         "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d"},
        {"nfc-cafe", "caf\xc3\xa9", "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e"},
        {"nfd-cafe", "cafe\xcc\x81", "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a"},
        {"trailing-newline", "abc\n", "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
    };
    for (const auto& c : text_cases) {
        check(c.name, sha256_text(c.data), c.want);
    }

    // The SHA-256 Test Data of the NIST example values, SHA2_Additional.pdf.
    const std::vector<Case> nist_cases = {
        {"nist-1", "\xbd", "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b"},
        {"nist-2", "\xc9\x8c\x8e\x55", "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504"},
        {"nist-3", std::string(55, '\0'), "02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7"},
        {"nist-4", std::string(56, '\0'), "d4817aa5497628e7c77e6b606107042bbba3130888c5f47a375e6179be789fbb"},
        {"nist-5", std::string(57, '\0'), "65a16cb7861335d5ace3c60718b5052e44660726da4cd13bb745381b235a1785"},
        {"nist-6", std::string(64, '\0'), "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b"},
        {"nist-7", std::string(1000, '\0'), "541b3e9daa09b20bf85fa273e5cbd3e80185aa4ec298e765db87742b70138a53"},
        {"nist-8", std::string(1000, 'A'), "c2e686823489ced2017f6059b8b239318b6364f6dcd835d0a519105a1eadd6e4"},
        {"nist-9", std::string(1005, 'U'), "f4d62ddec0f3dd90ea1380fa16a5ff8dc4c54b21740650f24afc4120903552b0"},
        {"nist-10", std::string(1000000, '\0'), "d29751f2649b32ff572b5e0a9f541ea660a50f94ff0beedfb0b692b924cc8025"},
    };
    for (const auto& c : nist_cases) {
        check(c.name, sha256_text(c.data), c.want);
    }

    // From cases/files.jsonl.
    const std::vector<Case> file_cases = {
        {"abc-file", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
        {"abc-newline-file", "abc\n", "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
        {"abc-crlf-file", "abc\r\n", "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025"},
        {"empty-file", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
        {"binary-file", "\x00\x01\xff"s, "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092"},
        // One million repetitions of "a": more than one chunk.
        {"million-a-file", std::string(1000000, 'a'),
         "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"},
    };
    const std::filesystem::path input = "sha256_hash_input.bin";
    for (const auto& c : file_cases) {
        {
            std::ofstream out(input, std::ios::binary);
            out << c.data;
        }
        auto got = sha256_file(input);
        check(c.name, got.value_or("error"), c.want);
    }
    std::filesystem::remove(input);

    auto missing = sha256_file("sha256_hash_missing.txt");
    if (missing.has_value() || missing.error() != HashError::cannot_open) {
        std::fprintf(stderr, "case missing-file failed\n");
        ++failures;
    }

    // The message in parts of 1, 2, ..., 130 bytes gives the same digest as in one part.
    std::string data(1000, '\0');
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<char>(i % 251);
    }
    const std::string want = sha256_text(data);
    for (std::size_t size = 1; size <= 130; ++size) {
        Sha256 h;
        for (std::size_t i = 0; i < data.size(); i += size) {
            h.update(std::string_view(data).substr(i, size));
        }
        check("parts of " + std::to_string(size), to_hex(h.finalize()), want);
    }

    if (failures > 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return EXIT_FAILURE;
    }
    std::puts("ok");
    return EXIT_SUCCESS;
}
