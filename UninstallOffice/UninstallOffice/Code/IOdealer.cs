using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Helper
{
    public class IOdealer
    {
        private static string office2007 = "..\\..\\documents\\office 2007.txt";
        private static string office2010 = "..\\..\\documents\\office 2010.txt";

        public static string Office2007
        {
            get
            {
                return office2007;
            }
        }

        public static string Office2010
        {
            get
            {
                return office2010;
            }
        }

        public static List<string> ReadFile(string path)
        {
            string tmp = null;
            List<string> office = new List<string>();
            StreamReader sRead = new StreamReader(new FileStream(path, FileMode.Open));

            tmp = sRead.ReadLine();
            while (!string.IsNullOrEmpty(tmp))
            {
                office.Add(tmp);
                tmp = sRead.ReadLine();
            }
            sRead.Close();
            return office;
        }
    }
}