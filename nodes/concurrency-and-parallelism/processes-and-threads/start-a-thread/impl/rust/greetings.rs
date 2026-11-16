// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

//! Starts one thread per name; each thread builds and returns the greeting for its name.

use std::thread;

/// Returns "hello from " + names[i] for each i, each built on its own thread.
/// Panics if the operating system cannot create a thread.
pub fn thread_greetings(names: &[&str]) -> Vec<String> {
    let handles: Vec<thread::JoinHandle<String>> = names
        .iter()
        .map(|name| {
            // spawn needs a 'static closure, so the thread gets its own String, moved in.
            let name = name.to_string();
            thread::spawn(move || format!("hello from {name}"))
        })
        .collect();
    // Start all threads first, then join each one and take its return value.
    handles
        .into_iter()
        .map(|handle| handle.join().expect("a greeter thread panicked"))
        .collect()
}
