// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

/// Returns `base` to the power `exp` as decimal text. -1000 <= base <= 1000.
/// The digits live in base-10^9 limbs, least significant first.
pub fn big_power(base: i32, exp: u32) -> String {
    const LIMB: u64 = 1_000_000_000;
    let m = u64::from(base.unsigned_abs());
    let mut limbs: Vec<u32> = vec![1];
    for _ in 0..exp {
        let mut carry = 0u64;
        for d in limbs.iter_mut() {
            let t = u64::from(*d) * m + carry;
            *d = (t % LIMB) as u32;
            carry = t / LIMB;
        }
        if carry != 0 {
            limbs.push(carry as u32);
        }
    }
    let mut out = String::new();
    if base < 0 && exp % 2 == 1 {
        out.push('-');
    }
    let mut it = limbs.iter().rev();
    if let Some(top) = it.next() {
        out.push_str(&top.to_string());
    }
    for d in it {
        out.push_str(&format!("{d:09}")); // keep the inner zeros
    }
    out
}
