namespace Day7;

class Program
{
    static readonly string[][] rows = File.ReadAllLines("input.txt").Select(line => line.Split(' ')).ToArray();
    static readonly Dictionary<string, ushort?> values = [];

    static ushort? Process(string a, string b, Func<ushort?, ushort?, ushort?> op)
    {
        bool aIsNum = ushort.TryParse(a, out ushort x);
        bool bIsNum = ushort.TryParse(b, out ushort y);

        if (aIsNum && bIsNum)
            return op(x, y);
        else if (aIsNum && !bIsNum)
            return op(x, values[b]);
        else if (!aIsNum && bIsNum)
            return op(values[a], y);
        return op(values[a], values[b]);
    }

    static ushort? Calculate(string val)
    {
        while (values[val] is null)
        {
            foreach (var data in rows)
            {
                if (values[data[^1]] is not null)
                    continue;

                if (data[1] == "->")
                    values[data[^1]] = values[data[0]];
                else if (data[2] == "->" && values[data[1]] != 0)
                    values[data[^1]] = (ushort?) ~values[data[1]];
                else if (data[3] == "->")
                {
                    Func<ushort?, ushort?, ushort?>? op;

                    if (data[1] == "AND")
                        op = delegate (ushort? a, ushort? b){ return (ushort?)(a & b); };
                    else if (data[1] == "OR")
                        op = delegate (ushort? a, ushort? b){ return (ushort?)(a | b); };
                    else if (data[1] == "LSHIFT")
                        op = delegate (ushort? a, ushort? b){ return (ushort?)(a << b); };
                    else
                        op = delegate (ushort? a, ushort? b){ return (ushort?)(a >> b); };

                    values[data[^1]] = Process(data[0], data[2], op);
                }
            }
        }

        return values[val];
    }

    static void Main(string[] args)
    {
        foreach (var data in rows)
        {
            values.Add(data[^1], null);
            if (data[1] == "->" && ushort.TryParse(data[0], out ushort n))
                values[data[^1]] = n;
        }

        Console.WriteLine($"1: {Calculate("a")}");

        var a = values["a"];
        foreach (var key in values.Keys.ToList())
            values[key] = null;

        foreach (var data in rows)
            if (data[1] == "->" && ushort.TryParse(data[0], out ushort n))
                values[data[^1]] = n;

        values["b"] = a;
        Console.WriteLine($"2: {Calculate("a")}");
    }
}
