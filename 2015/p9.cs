using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace Day9;

class Program
{
    static readonly List<string> lines = [.. File.ReadAllLines("input.txt")];
    static readonly Dictionary<(string, string), int> distances = lines
        .SelectMany(line =>
        {
            var parts = line.Split(" = ");
            var cities = parts[0].Split(" to ");
            int distance = int.Parse(parts[1]);

            return new[]
            {
                new KeyValuePair<(string, string), int>((cities[0], cities[1]), distance),
                new KeyValuePair<(string, string), int>((cities[1], cities[0]), distance)
            };
        })
        .ToDictionary(pair => pair.Key, pair => pair.Value);
    static readonly List<string> cities = distances.Keys.SelectMany(pair => new[] { pair.Item1, pair.Item2 }).Distinct().ToList();
    static readonly IEnumerable<int> allRouteLengths = GetPermutations(cities, cities.Count).Select(p => GetDistance(p));

    static IEnumerable<IEnumerable<T>> GetPermutations<T>(IEnumerable<T> list, int length)
    {
        if (length == 1) 
            return list.Select(t => new T[] { t });

        return GetPermutations(list, length - 1).SelectMany(t => list.Where(e => !t.Contains(e)), (t1, t2) => t1.Concat([t2]));
    }

    static int GetDistance(IEnumerable<string> cities)
    {
        return cities.Zip(cities.Skip(1), (first, second) => distances[(first, second)]).Sum();
    }

    static void Main(string[] args)
    {
        Console.WriteLine($"1: {allRouteLengths.Min()}\n2: {allRouteLengths.Max()}");
    }
}
