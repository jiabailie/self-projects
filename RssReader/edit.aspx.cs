using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RssReader
{
    public partial class edit : RssBase
    {
        protected List<rss_node> list_rss = new List<rss_node>();
        protected void Page_Load(object sender, EventArgs e)
        {
            CommonObject = new common(string.Format("{0}\\url.xml", Request.PhysicalApplicationPath));
            list_rss = CommonObject.GetRssList();
        }
    }
}