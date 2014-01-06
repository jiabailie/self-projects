using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ExcelProcess
{
    public class MTD
    {
        public void WriteMTD(string alias, string ut)
        {
            StreamReader sRead = new StreamReader(new FileStream(@"D:\ACM\Codes\GCJ\file\B.in", FileMode.Open));
            StreamWriter sWrite = new StreamWriter(new FileStream(@"D:\ACM\Codes\GCJ\file\B.out", FileMode.OpenOrCreate));
        }
        public string ReadMTD(string alias)
        {
            return null;
        }
    }
}