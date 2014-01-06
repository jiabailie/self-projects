using System;
using System.Text;
using System.Xml;
using System.Collections;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RssReader
{
    public partial class rss : RssBase
    {
        protected StringBuilder rssList = new StringBuilder();
        protected void Page_Load(object sender, EventArgs e)
        {
            CommonObject = new common(string.Format("{0}\\url.xml", Request.PhysicalApplicationPath));
            List<rss_node> list_rss = CommonObject.GetRssList();

            foreach (rss_node rnode in list_rss)
            {
                rssList.Append(
                    string.Format("<li><a href='javascript:void(0);' onclick='submit(\"{0}\");'>{1}</a></li>\n",
                    rnode.value, rnode.key)
                    );
            }
        }
    }
}