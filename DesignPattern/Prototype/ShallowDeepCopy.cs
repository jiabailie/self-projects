using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

#region Shallow copy
public class ShallowCopy : ICloneable
{
    public int[] v = { 1, 2, 3 };
    public object Clone()
    {
        return this.MemberwiseClone();
    }
    public void Display()
    {
        foreach (int i in v)
        {
            Console.Write(i + " ");
        }
        Console.WriteLine();
    }
}

public class ShallowClient
{
    public ShallowClient()
    {
        ShallowCopy sc1=new ShallowCopy();
        ShallowCopy sc2=(ShallowCopy)sc1.Clone();

        sc1.v[0]=9;

        sc1.Display();
        sc2.Display();
    }
}
#endregion

#region Deep copy
public class DeepCopy : ICloneable
{
    public int[] v = { 1, 2, 3 };
    public DeepCopy() { }
    private DeepCopy(int[] v)
    {
        this.v = (int[])v.Clone();
    }
    public object Clone()
    {
        return new DeepCopy(this.v);
    }
    public void Display()
    {
        foreach (int i in v)
        {
            Console.Write(i + " ");
        }
        Console.WriteLine();
    }
}

public class DeepCopyClient
{
    public DeepCopyClient()
    {
        DeepCopy dc1 = new DeepCopy();
        DeepCopy dc2 = (DeepCopy)dc1.Clone();
        dc1.v[0] = 9;

        dc1.Display();
        dc2.Display();
    }
}
#endregion
