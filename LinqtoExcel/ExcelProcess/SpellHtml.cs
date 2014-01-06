using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ExcelProcess
{
    public class SpellHtml
    {
        private static string preGreet = "<p class=\"MsoNormal\" style='margin-bottom: 12.0pt'>"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>Hi ";
        private static string posGreet = ", this"
         + "is just a kindly notice for your labor count @ last week and this month, please"
         + "kindly check it.<o:p></o:p></span></p>";

        private static string preUT = "<table class=\"MsoNormalTable\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" style='border-collapse: collapse; border: none'>"
            + "<tr style='width=200px'>"
            + "<td style='border: solid black 1.0pt; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: #1F497D'>"
           + "Your Yesterday UT</span><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'></span></p>"
           + "</td>"
           + "<td width=\"79\" style='width: 59.25pt; border: solid black 1.0pt; border-left: none; background: white; padding: .75pt 3.75pt .75pt 3.75pt'>"
           + "<p class=\"MsoNormal\">"
           + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: #1F497D'>"
           + "</span><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string posUT = "</span></p></td></tr>";

        private static string preMTD = "<tr style='width=200px'>"
            + "<td style='border: solid black 1.0pt; border-top: none; background: white; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: #1F497D'>"
            + "Your MTD UT</span><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'></span></p>"
            + "</td>"
            + "<td width=\"79\" style='width: 59.25pt; border-top: none; border-left: none; border-bottom: solid black 1.0pt; border-right: solid black 1.0pt; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string posMTD = "</span></p></td></tr></table>";

        private static string note1 = "<p class=\"MsoNormal\" style='margin-bottom: 12.0pt'>"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>"
            + "<br>Detailed information:<br>======================================================<br><br>"
            + "Labor Report of <span style='color: #1F497D'>Yesterday</span>:</span></p>";

        private static string preAlias = "<table class=\"MsoNormalTable\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" style='border-collapse: collapse; border: none'>"
            + "<thead>"
            + "<tr>"
            + "<td style='border: solid black 1.0pt; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>Labor Report</span></b></p>"
            + "</td>"
            + "<td style='border: solid black 1.0pt; border-left: none; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>";
        private static string posAlias = "</span></b></p></td></tr></thead>";

        private static string preDate = "<tr>"
            + "<td style='border: solid black 1.0pt; border-top: none; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "</td>"
            + "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: #1F497D'>";
        private static string posDate = "</span></b><b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'></span></b></p></td></tr></table>";

        private static string note2 = "<p class=\"MsoNormal\">"
            + "<span style='font-size: 1.0pt;'>"
            + "<o:p>&nbsp;</o:p></span></p>";

        private static string preTable = "<table class=\"MsoNormalTable\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" style='border-collapse: collapse; border: none'>"
            + "<thead>"
            + "<tr>"
            + "<td style='border: solid black 1.0pt; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>"
            + "Labor Type</span></b></p>"
            + "</td>"
            + "<td style='border: solid black 1.0pt; border-left: none; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>"
            + "SR Number</span></b></p>"
            + "</td>"
            + "<td style='border: solid black 1.0pt; border-left: none; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>"
            + "SR Title</span></b></p>"
            + "</td>"
            + "<td style='border: solid black 1.0pt; border-left: none; background: #0080B7; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"; color: white'>";
        private static string posTable = "</span></b></p>"
            + "</td>"
            + "</tr>"
            + "</thead>"
            + "<tr>"
            + "<td style='border: solid black 1.0pt; border-top: none; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>SR Total</span></b></p>"
            + "</td>"
            + "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "</td>"
            + "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "</td>"
            + "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; background: #77BBFF; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\" align=\"center\" style='text-align: center'>"
            + "<b><span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string preSR = "</span></b></p>"
            + "</td>"
            + "</tr>"
            + "<tr>"
            + "<td rowspan=\"18\" style='border: solid black 1.0pt; border-top: none; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>SR</span></p>"
            + "</td>"
            + "</tr>";
        private static string preSRNum = "<tr>"
          + "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
          + "border-right: solid black 1.0pt; padding: .75pt 3.75pt .75pt 3.75pt'>"
          + "<p class=\"MsoNormal\">"
          + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string posSRNum = "</span></p></td>";

        private static string preSRTitle = "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string posSRTitle = "</span></p></td>";

        private static string preSRTime = "<td style='border-top: none; border-left: none; border-bottom: solid black 1.0pt;"
            + "border-right: solid black 1.0pt; padding: .75pt 3.75pt .75pt 3.75pt'>"
            + "<p class=\"MsoNormal\">"
            + "<span style='font-size: 9.0pt; font-family: \"Calibri\",\"sans-serif\"'>";
        private static string posSRTime = "</span></p></td></tr>";
        private static string posSR = "</table>";

        public string SpellEmail(string alias, OneMail mail)
        {
            DateTime now = DateTime.Now.AddDays(-1);
            if (now.DayOfWeek.ToString() == "Sunday")
            {
                now = DateTime.Now.AddDays(-3);
            }
            StringBuilder strB = new StringBuilder();
            strB.Append(preGreet);
            strB.Append(alias);
            strB.Append(posGreet);
            strB.Append(preUT);
            strB.Append(mail.UT);
            strB.Append(posUT);
            strB.Append(preMTD);
            strB.Append(mail.MTD);
            strB.Append(posMTD);
            strB.Append(note1);
            strB.Append(preAlias);
            strB.Append(alias);
            strB.Append(posAlias);
            strB.Append(preDate);
            strB.Append(now.Year + "-" + now.Month + "-" + now.Day);
            strB.Append(posDate);
            strB.Append(note2);
            strB.Append(preTable);
            strB.Append(alias);
            strB.Append(posTable);
            strB.Append(mail.TMin + " Min");
            strB.Append(preSR);

            foreach (SR x in mail.SRList)
            {
                strB.Append(preSRNum);
                strB.Append(x.Number);
                strB.Append(posSRNum);
                strB.Append(preSRTitle);
                strB.Append(x.Title);
                strB.Append(posSRTitle);
                strB.Append(preSRTime);
                strB.Append(x.Min + " Min");
                strB.Append(posSRTime);
            }
            strB.Append(posSR);
            return strB.ToString();
        }
    }
}