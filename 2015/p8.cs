using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace Day8;

class Program
{
    static readonly List<string> lines = [.. File.ReadAllLines("input.txt")];

    static int Part1(string line) => line.Length - Regex.Unescape(line[1..^1]).Length;

    static int Part2(string line) => 2 - line.Length + line.Replace("\\", "\\\\").Replace("\"", "\\\"").Length;

    static void Main(string[] args)
    {
        Console.WriteLine($"1: {lines.Select(Part1).Sum()}\n2: {lines.Select(Part2).Sum()}");
    }
}
