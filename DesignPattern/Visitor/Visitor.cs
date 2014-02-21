using System;
using System.Collections;

abstract public class Visitor
{
    abstract public void VisitConcreteElementA(ConcreteElementA concreteElementA);

    abstract public void VisitConcreteElementB(ConcreteElementB concreteElementB);
}

public class ConcreteVisitor1 : Visitor
{
    public override void VisitConcreteElementA(ConcreteElementA concreteElementA)
    {
        Console.WriteLine("{0} is visited by {1}", concreteElementA, this);
    }

    public override void VisitConcreteElementB(ConcreteElementB concreteElementB)
    {
        Console.WriteLine("{0} is visited by {1}", concreteElementB, this);
    }
}

public class ConcreteVisitor2 : Visitor
{
    public override void VisitConcreteElementA(ConcreteElementA concreteElementA)
    {
        Console.WriteLine("{0} is visited by {1}", concreteElementA, this);
    }

    public override void VisitConcreteElementB(ConcreteElementB concreteElementB)
    {
        Console.WriteLine("{0} is visited by {1}", concreteElementB, this);
    }
}

abstract public class Element
{
    abstract public void Accept(Visitor visitor);
}

public class ConcreteElementA : Element
{
    public override void Accept(Visitor visitor)
    {
        visitor.VisitConcreteElementA(this);
    }

    public void OperationA() { }
}

public class ConcreteElementB : Element
{
    public override void Accept(Visitor visitor)
    {
        visitor.VisitConcreteElementB(this);
    }

    public void OperationB() { }
}

public class ObjectStructure
{
    private ArrayList elements = new ArrayList();

    public void Attach(Element element)
    {
        elements.Add(element);
    }

    public void Detach(Element element)
    {
        elements.Remove(element);
    }

    public void Accept(Visitor visitor)
    {
        foreach (Element e in elements)
        {
            e.Accept(visitor);
        }
    }
}

public class Client
{
    public Client()
    {
        ObjectStructure o = new ObjectStructure();
        o.Attach(new ConcreteElementA());
        o.Attach(new ConcreteElementB());

        ConcreteVisitor1 v1 = new ConcreteVisitor1();
        ConcreteVisitor2 v2 = new ConcreteVisitor2();

        o.Accept(v1);
        o.Accept(v2);
    }
}
    