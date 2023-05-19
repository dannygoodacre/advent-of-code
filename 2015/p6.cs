using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Day6;

class Program
{
    const int DIM = 1000;
    static readonly string[] lines = File.ReadAllLines("input.txt");

    static void AdjustBulb1(List<int> start, List<int> end, ref bool[,] grid, Func<bool, bool> f)
    {
        for (int x = start[0]; x <= end[0]; x++)
            for (int y = start[1]; y <= end[1]; y++)
                grid[x, y] = f(grid[x, y]);
    }

    static void AdjustBulb2(List<int> start, List<int> end, ref int[,] grid, Func<int, int> f)
    {
        for (int x = start[0]; x <= end[0]; x++)
            for (int y = start[1]; y <= end[1]; y++)
                grid[x, y] = f(grid[x, y]);
    }

    static int Part1()
    {
        bool[,] grid = new bool[DIM,DIM];
        for (int x = 0; x < DIM; x++)
            for (int y = 0; y < DIM; y++)
                grid[x, y] = false; // Begin will all lights off.

        foreach (string line in lines)
        {
            List<string> data = line.Split(' ').ToList();

            // Remove "turn" and "through".
            if (!data[0].Equals("toggle")) 
                data.RemoveAt(0);
            data.RemoveAt(2);

            // All lines now have the form: "on/off/toggle x1,y1 x2,y2".

            // Coordinates.
            List<int> start = data[1].Split(',').Select(int.Parse).ToList();
            List<int> end = data[2].Split(',').Select(int.Parse).ToList();

            if (data[0] == "on")
                AdjustBulb1(start, end, ref grid, p => true);
            else if (data[0] == "off")
                AdjustBulb1(start, end, ref grid, p => false);
            else
                AdjustBulb1(start, end, ref grid, p => !p);
        }

        return grid.Cast<bool>().Where(p => p).Count();
    }

    static int Part2()
    {
        int[,] grid = new int[DIM,DIM];

        foreach (string line in lines)
        {
            List<string> data = line.Split(' ').ToList();

            // Remove "turn" and "through".
            if (!data[0].Equals("toggle"))
                data.RemoveAt(0);
            data.RemoveAt(2); 
            
            // All lines now have the form: "on/off/toggle x1,y1 x2,y2".
            
            // Coordinates.
            List<int> start = data[1].Split(',').Select(int.Parse).ToList();
            List<int> end = data[2].Split(',').Select(int.Parse).ToList();

            if (data[0] == "on")
                AdjustBulb2(start, end, ref grid, p => p + 1);
            else if (data[0] == "off")
                AdjustBulb2(start, end, ref grid, p => p > 0 ? p - 1 : p);
            else
                AdjustBulb2(start, end, ref grid, p => p + 2);
        }

        return grid.Cast<int>().Sum();
    }

    static void Main(string[] args)
    {
        Console.WriteLine($"1: {Part1()}\n2: {Part2()}");
    }
}