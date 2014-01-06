<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="edit.aspx.cs" Inherits="RssReader.edit" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Modify Rss List</title>
    <link href="App_Themes/rss.css" rel="stylesheet" type="text/css" />
    <script src="js/rss.js" type="text/javascript"></script>
</head>
<body>
    <h1 class="title">
        Modify Rss List&nbsp;[<a href="javascript:voie(0);" onclick="create();">Create</a>]&nbsp;[<a href="rss.aspx" target="_self">Back</a>]
    </h1>
    <hr />
    <div id="d_modify" style="display: none;">
        <input type="hidden" id="flag" />
        <table class="modify">
            <tr>
                <th>
                    Key
                </th>
                <th>
                    <input type="text" id="mkey" size="50" />
                    <input type="hidden" id="hkey" />
                </th>
            </tr>
            <tr>
                <th>
                    Value
                </th>
                <th>
                    <input type="text" id="mvalue" size="50" />
                    <input type="hidden" id="hvalue" />
                </th>
            </tr>
            <tr>
                <td colspan="2">
                    <input type="button" value="Cancel" onclick="cancel();" />
                    <input type="button" value="Confirm" onclick="update();" />
                </td>
            </tr>
        </table>
        <hr />
    </div>
    <table class="gridtable">
        <tr>
            <th>
                Rss Name
            </th>
            <th>
                Rss Link
            </th>
            <th>
                Delete
            </th>
            <th>
                Update
            </th>
        </tr>
        <%
            StringBuilder spell = new StringBuilder();
            for (int i = 0; i < list_rss.Count; i++)
            {
                spell.Append("<tr>");
                spell.Append(string.Format("<td>{0}</td><td>{1}</td>\n",
                    list_rss[i].key, list_rss[i].value));
                spell.Append(string.Format("<td><a href='process.aspx?operate=d&key={0}&value={1}'>Delete</a></td>",
                    list_rss[i].key, list_rss[i].value));
                spell.Append(string.Format("<td><a href='javascript:void(0);' onclick=\"modify('{0}','{1}');\">Update</a></td>",
                    list_rss[i].key, list_rss[i].value));
                spell.Append("</tr>");
            }
            Response.Write(spell.ToString());
        %>
    </table>
</body>
</html>
