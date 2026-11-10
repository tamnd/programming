use std::cell::RefCell;

fn name_length() -> usize {
    let name = RefCell::new(String::from("Ada"));
    name.borrow().len()
}

fn main() {
    println!("{}", name_length());
}
