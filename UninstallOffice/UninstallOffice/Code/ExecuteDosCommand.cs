using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace Helper
{
    public class ExecuteDosCommand
    {
        public static string ExeCommand(string[] commandTexts)
        {
            string strOutput = null;
            Process p = new Process();

            p.StartInfo.FileName = "cmd.exe";
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.CreateNoWindow = true;

            try
            {
                p.Start();
                foreach (string item in commandTexts)
                {
                    p.StandardInput.WriteLine(item);
                }
                p.StandardInput.WriteLine("exit");
                strOutput = p.StandardOutput.ReadToEnd();
                p.WaitForExit();
                p.Close();
            }
            catch (Exception e)
            {
                strOutput = e.Message;
            }
            return strOutput;
        }

        public static string ExeCommand(string path, List<string> uninstall)
        {
            int i = 0;
            string strOutput = null;
            string[] commandTexts = new string[uninstall.Count];
            for (i = 0; i < uninstall.Count; i++)
            {
                commandTexts[i] = path + " /uninstall " + uninstall[i];
            }
            strOutput = ExeCommand(commandTexts);
            return strOutput;
        }
    }
}
