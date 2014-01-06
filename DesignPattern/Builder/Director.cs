using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

public class Director
{
    public void Construct(Builder builder)
    {
        builder.BuildPartA();
        builder.BuildPartB();
    }
}

abstract public class Builder
{
    abstract public void BuildPartA();
    abstract public void BuildPartB();
    abstract public Product GetResult();
}

public class ConcreteBuilder1 : Builder
{
    private Product product;

    public override void BuildPartA()
    {
        product = new Product();
        product.Add("PartA");
    }

    public override void BuildPartB()
    {
        product.Add("PartB");
    }

    public override Product GetResult()
    {
        return product;
    }
}

public class ConcreteBuilder2 : Builder
{
    private Product product;

    public override void BuildPartA()
    {
        product = new Product();
        product.Add("PartX");
    }

    public override void BuildPartB()
    {
        product.Add("PartY");
    }

    public override Product GetResult()
    {
        return product;
    }
}

public class Product
{
    ArrayList parts = new ArrayList();
    public void Add(string part)
    {
        parts.Add(part);
    }
    public void Show()
    {
        Console.WriteLine("Product Parts ---------");
        foreach (string part in parts)
        {
            Console.WriteLine(part);
        }
    }
}

public class Client
{
    public Client()
    {
        Director director = new Director();

        Builder b1 = new ConcreteBuilder1();
        Builder b2 = new ConcreteBuilder2();

        director.Construct(b1);
        Product p1 = b1.GetResult();
        p1.Show();

        director.Construct(b2);
        Product p2 = b2.GetResult();
        p2.Show();
    }
}

