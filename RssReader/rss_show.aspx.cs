using System;
using System.IO;
using System.Xml;
using System.Net;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RssReader
{
    public partial class rss_show : RssBase
    {
        public string owner = null;
        public string address = null;
        public string content = null;

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                owner = Request["owner"].ToString().Trim();
                address = Request["address"].ToString().Trim();

                content = Get_Rss_Content();
            }
        }

        protected string Get_Rss_Content()
        {
            int i = 0;
            StringBuilder ret = new StringBuilder();
            string title = "", link = "", desc = "";
            try
            {
                WebRequest request = WebRequest.Create(address);
                WebResponse response = request.GetResponse();

                Stream rssStream = response.GetResponseStream();
                XmlDocument rssDoc = new XmlDocument();
                rssDoc.Load(rssStream);

                XmlNodeList rssItems = rssDoc.SelectNodes("rss/channel/item");
                XmlNode rssDetail = null;

                for (i = 0; i < rssItems.Count; i++)
                {
                    title = link = desc = "";

                    rssDetail = rssItems.Item(i).SelectSingleNode("title");
                    if (rssDetail != null)
                    {
                        title = rssDetail.InnerText;
                    }

                    rssDetail = rssItems.Item(i).SelectSingleNode("link");
                    if (rssDetail != null)
                    {
                        link = rssDetail.InnerText;
                    }

                    rssDetail = rssItems.Item(i).SelectSingleNode("description");
                    if (rssDetail != null)
                    {
                        desc = rssDetail.InnerText;
                    }

                    ret.Append(string.Format("<p><b><a href='{0}' target='new'>{1}</a></b><br/>{2}</p>", link, title, desc));
                }
            }
            catch (Exception e)
            {
                Response.Write(e.Message);
            }
            return ret.ToString();
        }
    }
}
