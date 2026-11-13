// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

public static class IndexOf
{
    // Find returns the first index of target in the sorted array items, or -1.
    public static int Find(int[] items, int target)
    {
        // Array.BinarySearch can return any index of equal items, so this is
        // a search for the first index lo with items[lo] >= target.
        int lo = 0;
        int hi = items.Length;
        while (lo < hi)
        {
            int mid = lo + (hi - lo) / 2;
            if (items[mid] < target)
            {
                lo = mid + 1;
            }
            else
            {
                hi = mid;
            }
        }
        return lo < items.Length && items[lo] == target ? lo : -1;
    }
}
