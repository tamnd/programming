// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

using System.Text;

public static class Normalization
{
    // Normalize returns s in the form NFC or NFD. The 2 forms of the same text
    // are different strings, so == is true only after this.
    public static string Normalize(string s, string form)
    {
        return s.Normalize(form == "NFD" ? NormalizationForm.FormD : NormalizationForm.FormC);
    }
}
