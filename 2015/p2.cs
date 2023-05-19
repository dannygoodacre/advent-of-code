using System;
using System.IO;
using System.Linq;

namespace Day2;

class Program
{
    static void Main(string[] args)
    {
        string[] lines = File.ReadAllLines("input.txt");
        int n1 = 0, n2 = 0;
        foreach (string line in lines)
        {
            var v = line.Split('x').Select(int.Parse).OrderBy(x => x).ToArray();
            n1 += 3*v[0]*v[1] + 2*(v[1]*v[2] + v[0]*v[2]);
            n2 += 2*v[0] + 2*v[1] + v[0]*v[1]*v[2];
        }

        Console.WriteLine($"1: {n1}\n2: {n2}");
    }
}