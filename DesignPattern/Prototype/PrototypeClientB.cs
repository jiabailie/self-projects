using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

abstract class ColorPrototype
{
    public abstract ColorPrototype Clone();
}

class Color : ColorPrototype
{
    private int red, green, blue;

    public Color(int red, int green, int blue)
    {
        this.red = red;
        this.green = green;
        this.blue = blue;
    }

    public override ColorPrototype Clone()
    {
        return (ColorPrototype)this.MemberwiseClone();
    }

    public void Display()
    {
        Console.WriteLine("RGB values are: {0} {1} {2}", red, green, blue);
    }
}

class ColorManager
{
    Hashtable colors = new Hashtable();

    public ColorPrototype this[string name]
    {
        get { return (ColorPrototype)colors[name]; }
        set { colors.Add(name, value); }
    }
}

class PrototypeClientB
{
    public PrototypeClientB()
    {
        ColorManager colormanager = new ColorManager();

        colormanager["red"] = new Color(255, 0, 0);
        colormanager["green"] = new Color(0, 255, 0);
        colormanager["blue"] = new Color(0, 0, 255);

        colormanager["angry"] = new Color(255, 54, 0);
        colormanager["peace"] = new Color(128, 211, 128);
        colormanager["flame"] = new Color(211, 34, 20);

        string colorName = "red";
        Color c1 = (Color)colormanager[colorName].Clone();
        c1.Display();

        colorName = "peace";
        Color c2 = (Color)colormanager[colorName].Clone();
        c2.Display();

        colorName = "flame";
        Color c3 = (Color)colormanager[colorName].Clone();
        c3.Display();
    }
}
