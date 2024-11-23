using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Linq;

namespace Day11;

class Program
{
    static readonly string input = new(File.ReadAllText("input.txt"));

    static void Main(string[] args)
    {
        var passwords = ValidPasswords();
        Console.WriteLine($"Part 1: {passwords.First()}\nPart 2: {passwords.Last()}");
    }

    static IEnumerable<string> ValidPasswords()
    {
        string password = input;
        do
            password = IncrementPassword(password);
        while (!IsValidPassword(password));
        yield return password;

        do
            password = IncrementPassword(password);
        while (!IsValidPassword(password));
  
        yield return password;
    }

    static bool IsValidPassword(string password)
    {
        if (password.Contains('i') || password.Contains('o') || password.Contains('l'))
            return false;

        bool hasStraight = false;
        for (int i = 0; i < password.Length - 2; i++)
        {
            if (password[i]+1 == password[i + 1] && password[i]+2 == password[i + 2])
            {
                hasStraight = true;
                break;
            }
        }
        if (!hasStraight)
            return false;

        int pairCount = 0;
        for (int i = 0; i < password.Length - 1; i++)
        {
            if (password[i] == password[i + 1])
            {
                pairCount++;
                i++; // Skip next character to avoid overlapping pairs.
            }
        }
        if (pairCount < 2)
            return false;

        return true;
    }

    static string IncrementPassword(string password)
    {
        var result = new StringBuilder(password);
        var index = result.Length - 1;

        while (index >= 0)
        {
            if (result[index] == 'z')
            {
                result[index] = 'a';
                index--;
            }
            else
            {
                result[index]++;
                break;
            }
        }

        return result.ToString();
    }
}
