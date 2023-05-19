using System;
using System.IO;
using System.Collections.Generic;

namespace Day3;

class Program
{
    // Move coordinates (x,y) according to instruction c.
    // Return tuple of coordinates.
    static Tuple<int, int> process(char c, ref int x, ref int y)
    {
        switch (c)
        {
            case '>':
                x++;
                break;
            case '<':
                x--;
                break;
            case '^':
                y++;
                break;
            case 'v':
                y--;
                break;
        }
        return Tuple.Create(x, y);
    }

    static void Main(string[] args)
    {
        string data = File.ReadAllText("input.txt");
        var visited = new List<Tuple<int, int>>();
        Tuple<int, int> t;

        int n1 = 1, x = 0, y = 0;
        visited.Add(Tuple.Create(0, 0)); // Starting point.
        foreach (char c in data)
        {
            t = process(c, ref x, ref y);
            if (!visited.Contains(t))
            {
                visited.Add(t);
                n1++;
            }
        }
        visited.Clear();

        // Santa's x,y and Robo's x,y.
        int n2 = 1, sx = 0, sy = 0, rx = 0, ry = 0;
        bool rTurn = false;
        visited.Add(Tuple.Create(0, 0)); // Starting point.
        foreach (char c in data)
        {   
            t = rTurn ? process(c, ref rx, ref ry) : process(c, ref sx, ref sy);
            if (!visited.Contains(t))
            {
                visited.Add(t);
                n2++;
            }
            rTurn = !rTurn;
        }

        Console.WriteLine($"1: {n1}\n2: {n2}");
    }
}