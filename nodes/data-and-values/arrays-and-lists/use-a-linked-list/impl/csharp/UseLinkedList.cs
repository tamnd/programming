// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

using System.Globalization;

public static class UseLinkedList
{
    // Runs the operations on a new linked list and returns its items.
    public static int[] Run(string[] ops)
    {
        var list = new LinkedList<int>();
        foreach (var op in ops)
        {
            var p = op.Split(' ');
            switch (p[0])
            {
                case "push_front":
                    list.AddFirst(int.Parse(p[1], CultureInfo.InvariantCulture));
                    break;
                case "push_back":
                    list.AddLast(int.Parse(p[1], CultureInfo.InvariantCulture));
                    break;
                case "pop_front":
                    list.RemoveFirst();
                    break;
                case "pop_back":
                    list.RemoveLast();
                    break;
                case "remove":
                    // Remove(T) removes the first node with the value, if there is one.
                    list.Remove(int.Parse(p[1], CultureInfo.InvariantCulture));
                    break;
                default:
                    throw new ArgumentException($"bad operation: {op}", nameof(ops));
            }
        }
        return [.. list];
    }
}
