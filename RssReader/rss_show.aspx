<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="rss_show.aspx.cs" Inherits="RssReader.rss_show" %>
<%
    StringBuilder page = new StringBuilder();

    page.Append("<html>");
    page.Append("<head>");
    page.Append("<title>");
    page.Append(owner);
    page.Append("</title>");
    page.Append("<link href=\"App_Themes/rss.css\" rel=\"stylesheet\" type=\"text/css\" />");
    page.Append("<script src=\"js/rss.js\" type=\"text/javascript\"></script>");
    page.Append("</head>");
    page.Append("<body>");
    page.Append(content);
    page.Append("</body>");
    page.Append("</html>");

    Response.Write(page.ToString());
 %>
