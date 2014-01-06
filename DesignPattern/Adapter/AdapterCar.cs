using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public interface ICar
{
    void Drive();
}

public class CToyota : ICar
{
    public void Drive()
    {
        Console.WriteLine("Vroom Vroom, we're off in our Toyota...");
    }
}

public class CCessna
{
    public void Fly()
    {
        Console.WriteLine("Static runup OK, we're off in our C172...");
    }
}

public class CDrivableCessna : CCessna, ICar
{
    public void Drive() { base.Fly(); }
}

public class CDrivableCessna2 : ICar
{
    private CCessna m_oContained;
    public CDrivableCessna2()
    {
        m_oContained = new CCessna();
    }
    public void Drive()
    {
        m_oContained.Fly();
    }
}

public class CarClient
{
    public CarClient()
    {
        ICar oCar = new CToyota();
        Console.Write("Class Adapter: Driving an Automobile...");
        oCar.Drive();

        oCar = new CDrivableCessna();
        Console.Write("Driving a Cessna...");
        oCar.Drive();

        oCar = new CDrivableCessna2();
        Console.Write("Object Adapter: Driving a Cessna...");
        oCar.Drive();
    }
}