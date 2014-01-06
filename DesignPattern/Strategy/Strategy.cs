using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace lab
{
    abstract class Strategy
    {
        abstract public void AlgorithmInterface();
    }

    class ConcreteStrategyA : Strategy
    {
        public override void AlgorithmInterface()
        {
            Console.WriteLine("Called ConcreteStrategyA.AlgorithmInterface()");
        }
    }

    class ConcreteStrategyB : Strategy
    {
        public override void AlgorithmInterface()
        {
            Console.WriteLine("Called ConcreteStrategyB.AlgorithmInterface()");
        }
    }

    class ConcreteStrategyC : Strategy
    {
        public override void AlgorithmInterface()
        {
            Console.WriteLine("Called ConcreteStrategyC.AlgorithmInterface()");
        }
    }

    class Context
    {
        Strategy strategy;

        public Context(Strategy strategy)
        {
            this.strategy = strategy;
        }

        public void ContextInterface()
        {
            strategy.AlgorithmInterface();
        }
    }

    public class Client
    {
        public Client()
        {
            Context c = new Context(new ConcreteStrategyA());
            c.ContextInterface();

            Context d = new Context(new ConcreteStrategyB());
            d.ContextInterface();

            Context e = new Context(new ConcreteStrategyC());
            e.ContextInterface();
        }
    }
}
