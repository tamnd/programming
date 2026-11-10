fn main() {
    let mut scores = vec![7, 3, 9];
    let best = &scores[2];
    println!("{}", best);
    scores.push(10);
    println!("{}", scores.len());
}
