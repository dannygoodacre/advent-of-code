using System;
using System.IO;
using System.Text;
using System.Security.Cryptography;

namespace Day4;

class Program
{
    static void Main(string[] args)
    {
        string data = File.ReadAllText("input.txt");
        var md5 = MD5.Create();
        bool found = false;
        double i, j;

        for (i = 0; !found; i++)
        {
            byte[] inputBytes = Encoding.ASCII.GetBytes(data + i.ToString());
            string hash = Convert.ToHexString(md5.ComputeHash(inputBytes));
            found = hash.Substring(0, 5).Equals("00000");
        }

        found = false;
        for (j = 0; !found; j++)
        {
            byte[] inputBytes = Encoding.ASCII.GetBytes(data + j.ToString());
            string hash = Convert.ToHexString(md5.ComputeHash(inputBytes));
            found = hash.Substring(0, 6).Equals("000000");
        }

        Console.WriteLine($"1: {i}\n2: {j}");
    }
}