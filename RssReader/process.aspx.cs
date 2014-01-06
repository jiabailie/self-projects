using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace RssReader
{
    public partial class process : RssBase
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            CommonObject = new common(string.Format("{0}\\url.xml", Request.PhysicalApplicationPath));
            string operate = Request["operate"];
            if (operate.Equals("c") || operate.Equals("u"))
            {
                string newkey = Request["newkey"].ToString().Trim();
                string newvalue = Request["newvalue"].ToString().Trim();

                if (operate.Equals("c"))
                {
                    CommonObject.CreateRssNode(newkey, newvalue);
                }
                else if (operate.Equals("u"))
                {
                    string oldkey = Request["oldkey"].ToString().Trim();
                    string oldvalue = Request["oldvalue"].ToString().Trim();
                    CommonObject.UpdateRssNode(oldkey, oldvalue, newkey, newvalue);
                }
            }
            else if (operate.Equals("d"))
            {
                string key = Request["key"].ToString().Trim();
                string value = Request["value"].ToString().Trim();

                CommonObject.DeleteRssNode(key, value);
            }
            Response.Redirect("edit.aspx");
        }
    }
}