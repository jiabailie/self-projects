using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Xml;
using System.IO;

namespace RssReader
{
    public class rss_node
    {
        /// <summary>
        /// the name of rss source.
        /// </summary>
        public string key { get; set; }

        /// <summary>
        /// the url of this rss source.
        /// </summary>
        public string value { get; set; }

        public rss_node(string _key, string _value)
        {
            key = _key;
            value = _value;
        }
    }

    public class common
    {
        protected XmlDocument xmlDoc = null;
        protected string xmlPath = null;

        public common(string path)
        {
            xmlDoc = new XmlDocument();
            xmlPath = path;
            xmlDoc.Load(path);
        }

        public int FindRssCursor(string key, string value)
        {
            int i = 0;
            int cur = -1;
            try
            {
                XmlNodeList urls = xmlDoc.GetElementsByTagName("url");
                for (i = 0; i < urls.Count; i++)
                {
                    if (urls[i].Attributes["key"].Value.Equals(key) && urls[i].Attributes["value"].Value.Equals(value))
                    {
                        cur = i;
                        break;
                    }
                }
            }
            catch (Exception e)
            {
                throw new Exception("common.FindRssCursor:" + e.Message);
            }
            return cur;
        }

        public List<rss_node> GetRssList()
        {
            int len = 0;
            List<rss_node> list_rss = new List<rss_node>();

            try
            {
                XmlNodeList urls = xmlDoc.GetElementsByTagName("url");
                len = urls.Count;

                foreach (XmlNode xnode in urls)
                {
                    list_rss.Add(new rss_node(xnode.Attributes["key"].Value, xnode.Attributes["value"].Value));
                }
            }
            catch (Exception e)
            {
                throw new Exception("common.Get_Rss_List:" + e.Message);
            }
            return list_rss;
        }

        public void CreateRssNode(string key, string value)
        {
            try
            {
                if (FindRssCursor(key, value) != -1) { return; }

                XmlElement add = xmlDoc.CreateElement("url");
                add.SetAttribute("key", key);
                add.SetAttribute("value", value);

                xmlDoc.DocumentElement.FirstChild.AppendChild(add);

                xmlDoc.Save(xmlPath);
            }
            catch (Exception e)
            {
                throw new Exception("common.Add_Rss_List:" + e.Message);
            }
        }

        public void DeleteRssNode(string key, string value)
        {
            try
            {
                int cur = -1;
                cur = FindRssCursor(key, value);
                XmlNode root = xmlDoc.SelectSingleNode("config/urls");
                XmlNodeList urls = xmlDoc.GetElementsByTagName("url");

                if (cur != -1)
                {
                    root.RemoveChild(urls[cur]);
                }
                xmlDoc.Save(xmlPath);
            }
            catch (Exception e)
            {
                throw new Exception("common.DeleteRssList:" + e.Message);
            }
        }

        public void UpdateRssNode(string okey, string ovalue, string nkey, string nvalue)
        {
            try
            {
                int cur = FindRssCursor(okey, ovalue);
                if (cur == -1) { return; }
                XmlNodeList urls = xmlDoc.GetElementsByTagName("url");

                XmlElement xelem = (XmlElement)urls[cur];
                xelem.SetAttribute("key", nkey);
                xelem.SetAttribute("value", nvalue);

                xmlDoc.Save(xmlPath);
            }
            catch (Exception e)
            {
                throw new Exception("common.UpdateRssList:" + e.Message);
            }
        }
    }
}