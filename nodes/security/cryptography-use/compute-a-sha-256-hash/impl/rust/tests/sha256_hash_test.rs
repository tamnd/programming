// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

use algo_compute_a_sha_256_hash::{Sha256, sha256_file, sha256_hex, sha256_text, to_hex};
use std::fs;
use std::io::ErrorKind;
use std::path::PathBuf;
use std::sync::atomic::{AtomicU64, Ordering};

// A new empty directory for one test, removed when it goes out of scope.
struct TempDir(PathBuf);

impl TempDir {
    fn new() -> TempDir {
        static COUNT: AtomicU64 = AtomicU64::new(0);
        let n = COUNT.fetch_add(1, Ordering::Relaxed);
        let dir = std::env::temp_dir().join(format!("sha256-{}-{n}", std::process::id()));
        fs::create_dir(&dir).unwrap();
        TempDir(dir)
    }
}

impl Drop for TempDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

fn text_cases() -> Vec<(&'static str, String, &'static str)> {
    let a = |n| "a".repeat(n);
    vec![
        (
            "abc",
            "abc".into(),
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        ),
        (
            "two-block-56",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq".into(),
            "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
        ),
        (
            "fox",
            "The quick brown fox jumps over the lazy dog".into(),
            "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",
        ),
        (
            "empty",
            "".into(),
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
        ),
        (
            "a-55",
            a(55),
            "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318",
        ),
        (
            "a-56",
            a(56),
            "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a",
        ),
        (
            "a-63",
            a(63),
            "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34",
        ),
        (
            "a-64",
            a(64),
            "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb",
        ),
        (
            "multi-block-112",
            concat!(
                "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn",
                "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
            )
            .into(),
            "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1",
        ),
        (
            "non-ascii",
            "na\u{ef}ve caf\u{e9} \u{65e5}\u{672c}\u{8a9e} \u{1f642}".into(),
            "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d",
        ),
        (
            "nfc-cafe",
            "caf\u{e9}".into(),
            "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e",
        ),
        (
            "nfd-cafe",
            "cafe\u{301}".into(),
            "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a",
        ),
        (
            "trailing-newline",
            "abc\n".into(),
            "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb",
        ),
    ]
}

// From cases/files.jsonl.
fn file_cases() -> Vec<(&'static str, Vec<u8>, &'static str)> {
    vec![
        (
            "abc-file",
            b"abc".to_vec(),
            "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        ),
        (
            "abc-newline-file",
            b"abc\n".to_vec(),
            "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb",
        ),
        (
            "abc-crlf-file",
            b"abc\r\n".to_vec(),
            "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025",
        ),
        (
            "empty-file",
            vec![],
            "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
        ),
        (
            "binary-file",
            vec![0x00, 0x01, 0xff],
            "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092",
        ),
        // One million repetitions of "a": more than one chunk.
        (
            "million-a-file",
            vec![b'a'; 1_000_000],
            "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0",
        ),
    ]
}

// The SHA-256 Test Data of the NIST example values, SHA2_Additional.pdf.
fn nist_cases() -> Vec<(&'static str, Vec<u8>, &'static str)> {
    vec![
        (
            "nist-1",
            vec![0xbd],
            "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b",
        ),
        (
            "nist-2",
            vec![0xc9, 0x8c, 0x8e, 0x55],
            "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504",
        ),
        (
            "nist-3",
            vec![0; 55],
            "02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7",
        ),
        (
            "nist-4",
            vec![0; 56],
            "d4817aa5497628e7c77e6b606107042bbba3130888c5f47a375e6179be789fbb",
        ),
        (
            "nist-5",
            vec![0; 57],
            "65a16cb7861335d5ace3c60718b5052e44660726da4cd13bb745381b235a1785",
        ),
        (
            "nist-6",
            vec![0; 64],
            "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b",
        ),
        (
            "nist-7",
            vec![0; 1000],
            "541b3e9daa09b20bf85fa273e5cbd3e80185aa4ec298e765db87742b70138a53",
        ),
        (
            "nist-8",
            vec![b'A'; 1000],
            "c2e686823489ced2017f6059b8b239318b6364f6dcd835d0a519105a1eadd6e4",
        ),
        (
            "nist-9",
            vec![b'U'; 1005],
            "f4d62ddec0f3dd90ea1380fa16a5ff8dc4c54b21740650f24afc4120903552b0",
        ),
        (
            "nist-10",
            vec![0; 1_000_000],
            "d29751f2649b32ff572b5e0a9f541ea660a50f94ff0beedfb0b692b924cc8025",
        ),
    ]
}

#[test]
fn text() {
    for (name, text, want) in text_cases() {
        assert_eq!(sha256_text(&text), want, "{name}");
    }
}

#[test]
fn file() {
    let dir = TempDir::new();
    let path = dir.0.join("in.bin");
    for (name, data, want) in file_cases() {
        fs::write(&path, &data).unwrap();
        assert_eq!(sha256_file(&path).unwrap(), want, "{name}");
    }
}

#[test]
fn missing_file() {
    let dir = TempDir::new();
    let err = sha256_file(dir.0.join("missing.txt")).unwrap_err();
    assert_eq!(err.kind(), ErrorKind::NotFound);
}

#[test]
fn nist() {
    for (name, data, want) in nist_cases() {
        assert_eq!(sha256_hex(&data), want, "{name}");
    }
}

// The message in parts of 1, 2, ..., 130 bytes gives the same digest as in one part.
#[test]
fn update_in_parts() {
    let data: Vec<u8> = (0..1000u32).map(|i| (i % 251) as u8).collect();
    let want = sha256_hex(&data);
    for size in 1..=130 {
        let mut h = Sha256::new();
        for part in data.chunks(size) {
            h.update(part);
        }
        assert_eq!(to_hex(&h.finalize()), want, "parts of {size}");
    }
}

// One million repetitions of "a", in parts of 1000 bytes.
#[test]
fn million_a() {
    let mut h = Sha256::new();
    let part = [b'a'; 1000];
    for _ in 0..1000 {
        h.update(&part);
    }
    assert_eq!(
        to_hex(&h.finalize()),
        "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"
    );
}
