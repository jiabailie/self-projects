using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class Target
{
    virtual public void Request() { }
}

class Adaptee
{
    public void SpecificRequest()
    {
        Console.WriteLine("Called SpecificRequest()");
    }
}

class Adapter : Target
{
    private Adaptee adaptee = new Adaptee();
    public override void Request()
    {
        adaptee.SpecificRequest();
    }
}

public class Client
{
    public Client()
    {
        Target t = new Adapter();
        t.Request();
    }
}