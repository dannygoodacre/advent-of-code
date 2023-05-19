using System;
using System.IO;

namespace Day1;

class Program
{
    static void Main(string[] args)
    {
        string data = File.ReadAllText("input.txt");
        int floor = 0;
        int basement_pos = 0;
        bool found = false;

        for (int i = 0; i < data.Length; i++)
        {
            floor += (data[i] == '(') ? 1 : -1; // If not '(', then is ')'.
            if (floor == -1 && !found) // Haven't reached basement yet.
            {
                basement_pos = i + 1;
                found = true;
            }
        }

        Console.WriteLine($"1: {floor}\n2: {basement_pos}");
    }
}