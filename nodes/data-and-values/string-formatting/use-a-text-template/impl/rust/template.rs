// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

fn is_name(s: &str) -> bool {
    let mut chars = s.chars();
    matches!(chars.next(), Some(c) if c.is_ascii_alphabetic() || c == '_')
        && chars.all(|c| c.is_ascii_alphanumeric() || c == '_')
}

/// Returns template with $$ changed to $, and $name and ${name} changed to
/// the value of name. It reads the template once, so a value is not read as
/// a template. An unknown name or a $ without a name is an error.
pub fn substitute(template: &str, names: &[&str], values: &[&str]) -> Result<String, String> {
    let mut out = String::with_capacity(template.len());
    let mut rest = template;
    while let Some(i) = rest.find('$') {
        out.push_str(&rest[..i]);
        let after = &rest[i + 1..];
        if let Some(r) = after.strip_prefix('$') {
            out.push('$');
            rest = r;
            continue;
        }
        let (name, next) = if let Some(r) = after.strip_prefix('{') {
            let end = r.find('}').ok_or("a ${ without a }")?;
            (&r[..end], &r[end + 1..])
        } else {
            let end = after
                .find(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                .unwrap_or(after.len());
            (&after[..end], &after[end..])
        };
        if !is_name(name) {
            return Err(format!("a $ without a name: {rest}"));
        }
        let k = names
            .iter()
            .position(|n| *n == name)
            .ok_or_else(|| format!("unknown name: {name}"))?;
        out.push_str(values[k]);
        rest = next;
    }
    out.push_str(rest);
    Ok(out)
}
