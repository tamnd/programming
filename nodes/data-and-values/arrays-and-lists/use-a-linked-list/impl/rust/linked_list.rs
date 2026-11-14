// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

use std::collections::LinkedList;

/// Runs the operations on a new linked list and returns its items.
///
/// `LinkedList` has no stable method that removes an item in the middle, so
/// `remove` splits the list at the item, drops the item and joins the parts.
pub fn run_ops(ops: &[&str]) -> Vec<i32> {
    let mut list = LinkedList::new();
    for &op in ops {
        let (name, arg) = op.split_once(' ').unwrap_or((op, ""));
        let value = || arg.parse::<i32>().expect("an operation needs a 32-bit integer");
        match name {
            "push_front" => list.push_front(value()),
            "push_back" => list.push_back(value()),
            "pop_front" => {
                list.pop_front();
            }
            "pop_back" => {
                list.pop_back();
            }
            "remove" => {
                let v = value();
                if let Some(i) = list.iter().position(|&x| x == v) {
                    let mut rest = list.split_off(i);
                    rest.pop_front();
                    list.append(&mut rest);
                }
            }
            _ => panic!("bad operation: {op}"),
        }
    }
    list.into_iter().collect()
}
