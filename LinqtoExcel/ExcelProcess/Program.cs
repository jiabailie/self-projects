using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ExcelProcess
{
    class Program
    {
        static void Main(string[] args)
        {
            string path = @"..\..\File\data.xlsx";
            string HtmlBody = "";
            string Subject = "";
            ReadExcel rExcel = new ReadExcel();
            SpellHtml sHtml = new SpellHtml();
            NameDictionary nDic = new NameDictionary();
            List<string> ToAddress = new List<string>();
            List<string> CcAddress = new List<string>();
            List<OneMail> ret = rExcel.ReadExcelFileByPath(path);
            EmailSender eSender = new EmailSender();

            foreach (OneMail om in ret)
            {
                HtmlBody = sHtml.SpellEmail(nDic.NameDic[om.alias], om);
                Subject = "Engineer's UT";

                //clear address
                ToAddress.Clear();
                CcAddress.Clear();

                //For Real Use
                ToAddress.Add(om.alias.ToLower() + "@microsoft.com");
                CcAddress.Add(nDic.TeamManagerDic[nDic.BelongDic[om.alias]]);
                foreach (string s in nDic.TechLeaderDic[nDic.BelongDic[om.alias]])
                {
                    CcAddress.Add(s);
                }

                MailStatus mStatus = eSender.sendEmail(ToAddress, CcAddress, Subject, HtmlBody);
            }
            Console.WriteLine("Process completed!");
            Console.ReadLine();
        }
    }
}