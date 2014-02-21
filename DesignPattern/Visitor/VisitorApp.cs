using System;
using System.Collections;

abstract public class Visitor
{
    abstract public void Visit(Element element);
}

public class IncomeVisitor : Visitor
{
    public override void Visit(Element element)
    {
        Employee employee = (Employee)element;

        employee.Income *= 10;
        Console.WriteLine("{0}'s new income:{1:C}", employee.Name, employee.Income);
    }
}

public class VacationVisitor : Visitor
{
    public override void Visit(Element element)
    {
        Employee employee = (Employee)element;

        employee.VacationDays += 3;
        Console.WriteLine("{0}'s new vacation days:{1}", employee.Name, employee.VacationDays);
    }
}

abstract public class Element
{
    abstract public void Accept(Visitor visitor);
}

public class Employee : Element
{
    public string Name { get; set; }
    public double Income { get; set; }
    public int VacationDays { get; set; }

    public Employee(string name, double income, int vacationDays)
    {
        Name = name;
        Income = income;
        VacationDays = vacationDays;
    }

    public override void Accept(Visitor visitor)
    {
        visitor.Visit(this);
    }
}

public class Employees
{
    private ArrayList employees = new ArrayList();

    public void Attach(Employee employee)
    {
        employees.Add(employee);
    }

    public void Detach(Employee employee)
    {
        employees.Remove(employee);
    }

    public void Accept(Visitor visitor)
    {
        foreach (Employee e in employees)
        {
            e.Accept(visitor);
        }
    }
}

public class VisitorApp
{
    public VisitorApp()
    {
        Employees e = new Employees();
        e.Attach(new Employee("Hank", 25000, 14));
        e.Attach(new Employee("Elly", 35000, 16));
        e.Attach(new Employee("Dick", 45000, 21));

        IncomeVisitor v1 = new IncomeVisitor();
        VacationVisitor v2 = new VacationVisitor();

        e.Accept(v1);
        e.Accept(v2);
    }
}