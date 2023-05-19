using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace Day5;

class Program
{
    static readonly string[] pairs = {"ab", "cd", "pq", "xy"};
    static readonly Regex rgx = new Regex("[bcdfghjklmnpqrstvwxyz]");

    static bool IsNice1(string s)
    {
        if (rgx.Replace(s, "").Count() < 3 || !Regex.IsMatch(s, @"(.)\1{1,}"))
            return false;

        foreach (string pair in pairs)
            if (s.Contains(pair))
                return false;

        return true;
    }

    static bool IsNice2(string s)
    {
        if (!Regex.IsMatch(s, @"(\w{2}).*?(\1)") || !Regex.IsMatch(s, @"(\w)(\w)(\1)"))
            return false;

        return true;
    }

    static void Main(string[] args)
    {
        string[] lines = File.ReadAllLines("input.txt");
        int n1 = 0, n2 = 0;
        foreach (string line in lines)
        {
            n1 += IsNice1(line) ? 1 : 0;
            n2 += IsNice2(line) ? 1 : 0;
        }
        Console.WriteLine($"1: {n1}\n2: {n2}");
    }
}