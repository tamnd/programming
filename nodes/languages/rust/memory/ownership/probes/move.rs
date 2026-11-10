fn main() {
    let a = String::from("hi");
    let b = a;
    let c = b.clone();
    println!("{} {}", b, c);
}
