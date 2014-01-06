using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

interface ITarget
{
    void Request();
}

class PAdapter
{
    public void SpecificRequest()
    {
        Console.WriteLine("Called SpecificRequest()");
    }
}

class Adapter : PAdapter, ITarget
{
    public void Request()
    {
        this.SpecificRequest();
    }
}

public class AdapterClient
{
    public AdapterClient()
    {
        ITarget it = new Adapter();
        it.Request();
    }
}
