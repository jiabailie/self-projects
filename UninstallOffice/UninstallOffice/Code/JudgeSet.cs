using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helper
{
   public class JudgeSet
    {
       public static bool JudgeInputPath(string path)
       {
           bool flag = true;

           if (string.IsNullOrEmpty(path))
           {
               throw new Exception("Please select the path of setup.exe.");
           }
           else if (path.IndexOf("setup.exe") == -1)
           {
               throw new Exception("Please select the path of your Office product's setup.exe.");
           }

           return flag;
       }
    }
}
