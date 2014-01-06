using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace lab
{
    abstract class SortStrategy
    {
        abstract public void Sort(ArrayList list);
    }

    class QuickSort : SortStrategy
    {
        public override void Sort(ArrayList list)
        {
            // Quick sort
            Console.WriteLine("QuickSorted list");
        }
    }

    class ShellSort : SortStrategy
    {
        public override void Sort(ArrayList list)
        {
            // Shell sort
            Console.WriteLine("ShellSorted list");
        }
    }

    class MergeSort : SortStrategy
    {
        public override void Sort(ArrayList list)
        {
            // Merge sort
            Console.WriteLine("MergeSorted list");
        }
    }

    class SortedList
    {
        private ArrayList list = new ArrayList();
        private SortStrategy sortstrategy;

        public void SetSortStrategy(SortStrategy sortstrategy)
        {
            this.sortstrategy = sortstrategy;
        }

        public void Sort()
        {
            sortstrategy.Sort(list);
        }

        public void Add(string name)
        {
            list.Add(name);
        }

        public void Display()
        {
            foreach (string name in list)
            {
                Console.WriteLine(" " + name);
            }
        }
    }

    class StrategyApp
    {
        public StrategyApp()
        {
            SortedList StudentRecords = new SortedList();
            StudentRecords.Add("Samual");
            StudentRecords.Add("Jimmy");
            StudentRecords.Add("Sandra");
            StudentRecords.Add("Anna");
            StudentRecords.Add("Vivek");

            StudentRecords.SetSortStrategy(new MergeSort());
            StudentRecords.Sort();
            StudentRecords.Display();
        }
    }
}
