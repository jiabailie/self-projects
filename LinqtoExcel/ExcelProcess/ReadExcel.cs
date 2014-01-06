using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LinqToExcel;
using Remotion;

namespace ExcelProcess
{
    public class SR
    {
        public SR(string number, string title, int min)
        {
            this.Number = number;
            this.Title = title;
            this.Min = min;
        }
        public string Number { get; set; }
        public string Title { get; set; }
        public int Min { get; set; }
    }
    public class OneMail
    {
        public string alias { get; set; }
        public string UT { get; set; }
        public string MTD { get; set; }
        public string TMin { get; set; }
        public List<SR> SRList { get; set; }
    }

    public class ReadExcel
    {
        public List<OneMail> ReadExcelFileByPath(string path)
        {
            int i = 0;
            OneMail mail = new OneMail();
            string[] tmp = null;
            List<OneMail> ret = new List<OneMail>();
            ExcelQueryFactory excel = new ExcelQueryFactory(path);
            int Worksheetcount = excel.GetWorksheetNames().Count();

            for (i = 0; i < Worksheetcount; i++)
            {
                IQueryable<Row> query = from p in excel.Worksheet(i)
                                        select p;
                foreach (Row item in query)
                {
                    if (!string.IsNullOrEmpty(item[0].ToString().Trim()))
                    {
                        if (!string.IsNullOrEmpty(mail.alias))
                        {
                            ret.Add(mail);
                        }
                        mail = new OneMail();
                        tmp = item[2].ToString().Replace(')', ' ').Split('(');
                        mail.alias = item[0].ToString().Trim();
                        mail.TMin = tmp[0].Trim();
                        mail.UT = tmp[1].Trim();
                        
                        /// temporary use
                        mail.MTD = mail.UT;
                        /// 

                        mail.SRList = new List<SR>();
                    }
                    else
                    {
                        mail.SRList.Add(new SR(
                            item[1].ToString().Trim(),
                            item[3].ToString().Trim(),
                            Convert.ToInt32(item[2].ToString().Trim()
                            )));
                    }
                }
            }
            if (!string.IsNullOrEmpty(mail.alias))
            {
                ret.Add(mail);
            }
            return ret;
        }
    }
}
