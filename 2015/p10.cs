using System;
using System.IO;
using System.Text;

namespace Day10;

class Program
{
    static StringBuilder input = new(File.ReadAllText("input.txt"));

    static void Main(string[] args)
    {
        for (int i = 1; i <= 50; i++)
        {
            input = LookSay(input);
            if (i == 40)
                Console.WriteLine($"Part 1: {input.Length}");
        }
        Console.WriteLine($"Part 2: {input.Length}");
    }

    static int GetSubstringLength(StringBuilder s, int start)
    {
        char targetChar = s[start];
        int length = 0;

        for (int i = start; i < s.Length; i++)
        {
            if (s[i] != targetChar)
                break;
            length++;
        }

        return length;
    }

    static StringBuilder LookSay(StringBuilder s)
    {
        var result = new StringBuilder();
        for (int i = 0; i < input.Length; i++)
        {
            var length = GetSubstringLength(s, i);
            result.Append(length.ToString() + s[i]);
            i += length - 1;
        }

        return result;
    }
}
