<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="rss.aspx.cs" Inherits="RssReader.rss" %>

<html>
<head>
    <title>RSS Reader</title>
    <link href="App_Themes/rss.css" rel="stylesheet" type="text/css" />
    <script src="js/rss.js" type="text/javascript"></script>
</head>
<body>
    <h1 class="title">
        RSS Reader
    </h1>
    <hr />
    <p>RSS Address list&nbsp;[<a href="edit.aspx" target="_self">Edit</a>]</p>
    <%Response.Write(rssList.ToString());%>
</body>
</html>