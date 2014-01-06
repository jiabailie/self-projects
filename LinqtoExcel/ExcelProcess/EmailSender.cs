using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Configuration;
using Microsoft.Office.Interop.Outlook;

namespace ExcelProcess
{
    public enum MailStatus { OK, Error }

    public interface MailClient
    {
        MailStatus sendEmail(List<string> ToMailAddress, List<string> CCMailAddress, string Subject, string HTMLMailBody);
    }

    public class EmailSender : MailClient
    {
        public MailStatus sendEmail(List<string> ToMailAddress, List<string> CCMailAddress, string Subject, string HTMLMailBody)
        {
            if (ToMailAddress == null)
            {
                throw new System.Exception("No Mail Address acceptor provided.");
            }
            if (Subject == null && HTMLMailBody == null)
            {
                throw new System.Exception("Don't allow to deliver message without content");
            }
            try
            {
                Application app = new Application();
                StringBuilder stmp = new StringBuilder();
                NameSpace ns = app.GetNamespace("MAPI");
                ns.Logon("t-ruyan@microsoft.com",
                         "yeying19850707",
                         false, true);
                MailItem message = (MailItem)app.CreateItem(OlItemType.olMailItem);
                message.To = "";
                message.CC = "";
                foreach (string MsgTo in ToMailAddress)
                    message.To += MsgTo + "; ";

                if (CCMailAddress != null && CCMailAddress.Count > 0)
                {
                    foreach (string s in CCMailAddress)
                    {
                        stmp.Append(s);
                        stmp.Append("@microsoft.com;");
                    }
                    message.CC = stmp.ToString();
                }
                message.Subject = Subject;
                message.HTMLBody = HTMLMailBody;
                //message.Send();
                ns.Logoff();
                return MailStatus.OK;
            }
            catch (System.Exception e)
            {
                Console.WriteLine(e.StackTrace);
                Console.WriteLine(e.ToString());

                return MailStatus.Error;
            }
        }
    }
}