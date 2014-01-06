using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ExcelProcess
{
    public class NameDictionary
    {
        public Dictionary<string, string> NameDic = null;
        public Dictionary<string, string> BelongDic = null;
        public Dictionary<string, string> TeamManagerDic = null;
        public Dictionary<string, string[]> TechLeaderDic = null;

        public NameDictionary()
        {
            TeamManagerDic = new Dictionary<string, string>();
            TechLeaderDic = new Dictionary<string, string[]>();
            NameDic = new Dictionary<string, string>();
            BelongDic = new Dictionary<string, string>();

            // Tm
            TeamManagerDic.Add("Office", "danielzh");
            TeamManagerDic.Add("SharePoint", "michzhou");
            TeamManagerDic.Add("Exchange", "jeffchen");
            TeamManagerDic.Add("OCS", "wuyi");

            // Tl
            TechLeaderDic.Add("Office", new string[] { "shirleyz" });
            TechLeaderDic.Add("SharePoint", new string[] { "karenzhe", "fange" });
            TechLeaderDic.Add("Exchange", new string[] { "tahuan" });
            TechLeaderDic.Add("OCS", new string[] { "jezh", "jojosu" });

            // DANIEL
            NameDic.Add("DAJIANG", "Daisy Jiang");
            NameDic.Add("DAVWANG", "David Wang");
            NameDic.Add("WEIGE", "Fish Ge");
            NameDic.Add("HZHOU", "Hongtao Zhou");
            NameDic.Add("JUSTIW", "Justin Wang");
            NameDic.Add("KEITHLIN", "Keith Lin");
            NameDic.Add("KEVINXU", "Kevin Xu");
            NameDic.Add("LEOSUN", "Leo Sun");
            NameDic.Add("T-PEBAO", "Pengqing Bao");
            NameDic.Add("T-RUYAN", "Ruiguo Yang");
            NameDic.Add("VINCENTZ", "Vicent Zhang");
            NameDic.Add("SHIRLEYZ", "Shirley Zhou");
            NameDic.Add("XIZOU", "Xiaowen Zou");
            BelongDic.Add("DAJIANG", "Office");
            BelongDic.Add("DAVWANG", "Office");
            BelongDic.Add("WEIGE", "Office");
            BelongDic.Add("HZHOU", "Office");
            BelongDic.Add("JUSTIW", "Office");
            BelongDic.Add("KEITHLIN", "Office");
            BelongDic.Add("KEVINXU", "Office");
            BelongDic.Add("LEOSUN", "Office");
            BelongDic.Add("T-PEBAO", "Office");
            BelongDic.Add("T-RUYAN", "Office");
            BelongDic.Add("VINCENTZ", "Office");
            BelongDic.Add("SHIRLEYZ", "Office");
            BelongDic.Add("XIZOU", "Office");

            // Michael
            NameDic.Add("ADDING", "Adam Ding");
            NameDic.Add("KARENZHE", "Karen Zheng");
            NameDic.Add("CHRISMEN", "Chris meng");
            NameDic.Add("DANNYZH", "Danny Zhang");
            NameDic.Add("DEREKLI", "Derek Li");
            NameDic.Add("FANGE", "Cindy Ge");
            NameDic.Add("T-DOFAN", "Dongyun Fan");
            NameDic.Add("T-HAHEN", "Hao Heng");
            NameDic.Add("HENRYWAN", "Henry Wang");
            NameDic.Add("JESSICAL", "Jessical Liu");
            NameDic.Add("JOHNSUN", "Johnson Sun");
            NameDic.Add("JOHNXU", "John Xu");
            NameDic.Add("A-KAZHAN", "Karl Zhang");
            NameDic.Add("KEZH", "Kevin Zhu");
            NameDic.Add("T-LEILI", "Lei Li");
            NameDic.Add("LILLIAN", "Lillian Lin");
            NameDic.Add("NINGREN", "Ning Ren");
            NameDic.Add("SHUOYANG", "Shuo Yang");
            NameDic.Add("SIXU", "Simon Xu");
            NameDic.Add("SONGFANG", "Song Fang");
            NameDic.Add("TERESAX", "Teresa Xia");
            NameDic.Add("YANQWA", "Yanqun Wang");
            NameDic.Add("YUZHAO", "Yu Zhao");
            BelongDic.Add("ADDING", "SharePoint");
            BelongDic.Add("A-KAZHAN", "SharePoint");
            BelongDic.Add("CHRISMEN", "SharePoint");
            BelongDic.Add("DANNYZH", "SharePoint");
            BelongDic.Add("DEREKLI", "SharePoint");
            BelongDic.Add("FANGE", "SharePoint");
            BelongDic.Add("T-DOFAN", "SharePoint");
            BelongDic.Add("T-HAHEN", "SharePoint");
            BelongDic.Add("HENRYWAN", "SharePoint");
            BelongDic.Add("JESSICAL", "SharePoint");
            BelongDic.Add("JOHNSUN", "SharePoint");
            BelongDic.Add("JOHNXU", "SharePoint");
            BelongDic.Add("KARENZHE", "SharePoint");
            BelongDic.Add("KEZH", "SharePoint");
            BelongDic.Add("T-LEILI", "SharePoint");
            BelongDic.Add("LILLIAN", "SharePoint");
            BelongDic.Add("NINGREN", "SharePoint");
            BelongDic.Add("SHUOYANG", "SharePoint");
            BelongDic.Add("SIXU", "SharePoint");
            BelongDic.Add("SONGFANG", "SharePoint");
            BelongDic.Add("TERESAX", "SharePoint");
            BelongDic.Add("YANQWA", "SharePoint");
            BelongDic.Add("YUZHAO", "SharePoint");

            // JEFFCHEN
            NameDic.Add("ALEXDONG", "Alex Dong");
            NameDic.Add("ALZHAN", "Alpha Zhang");
            NameDic.Add("BREENY", "Breen Yang");
            NameDic.Add("BRIANWAN", "Brian Wang");
            NameDic.Add("T-CHAOC", "Chao Chen");
            NameDic.Add("DEANLI", "Dean Liu");
            NameDic.Add("ERICALI", "Erica Li");
            NameDic.Add("T-GUCH", "Guyuan Chen");
            NameDic.Add("HELELI", "Helen Li");
            NameDic.Add("HOQIAN", "Holly Qian");
            NameDic.Add("JAHE", "Jason He");
            NameDic.Add("JOHNZHU", "Johnny Zhu");
            NameDic.Add("T-LINXUE", "Lin Xue");
            NameDic.Add("PAOLOLIN", "Paolo Lin");
            NameDic.Add("RENEEW", "Renee Wang");
            NameDic.Add("SAFENG", "Samuel Feng");
            NameDic.Add("STEVENJI", "Steven Jiang");
            NameDic.Add("TAHUAN", "Tairan Huang");
            NameDic.Add("TONYL", "Tony Li");
            NameDic.Add("TONYZ", "Tony Zhang");
            NameDic.Add("WENBINWU", "Wenbin Wu");
            NameDic.Add("WIKONG", "Will Kong");
            NameDic.Add("T-XCHU", "Xiaowei Chu");
            NameDic.Add("YAUJIA", "Yauk Jia");
            NameDic.Add("T-YULAI", "Yechen Lai");
            BelongDic.Add("ALEXDONG", "Exchange");
            BelongDic.Add("ALZHAN", "Exchange");
            BelongDic.Add("BREENY", "Exchange");
            BelongDic.Add("BRIANWAN", "Exchange");
            BelongDic.Add("T-CHAOC", "Exchange");
            BelongDic.Add("DEANLI", "Exchange");
            BelongDic.Add("ERICALI", "Exchange");
            BelongDic.Add("T-GUCH", "Exchange");
            BelongDic.Add("HELELI", "Exchange");
            BelongDic.Add("HOQIAN", "Exchange");
            BelongDic.Add("JAHE", "Exchange");
            BelongDic.Add("JOHNZHU", "Exchange");
            BelongDic.Add("T-LINXUE", "Exchange");
            BelongDic.Add("PAOLOLIN", "Exchange");
            BelongDic.Add("RENEEW", "Exchange");
            BelongDic.Add("SAFENG", "Exchange");
            BelongDic.Add("STEVENJI", "Exchange");
            BelongDic.Add("TAHUAN", "Exchange");
            BelongDic.Add("TONYL", "Exchange");
            BelongDic.Add("TONYZ", "Exchange");
            BelongDic.Add("WENBINW", "Exchange");
            BelongDic.Add("WIKONG", "Exchange");
            BelongDic.Add("T-XCHU", "Exchange");
            BelongDic.Add("YAUJIA", "Exchange");
            BelongDic.Add("T-YULAI", "Exchange");

            //WuYi
            NameDic.Add("BAXIE", "Barry Xie");
            NameDic.Add("CATHYWEI", "Cathy Wei");
            NameDic.Add("CHICHEN", "Chimex Chen");
            NameDic.Add("WWANG", "Elaine Wang");
            NameDic.Add("JACKYFAN", "Jacky Fan");
            NameDic.Add("JEZH", "Jet Zhu");
            NameDic.Add("JOJOSU", "Jojo Su");
            NameDic.Add("KELIAN", "Kevin Liang");
            NameDic.Add("KEZUO", "Kenny Zuo");
            NameDic.Add("LEICAO", "Charlie Cao");
            NameDic.Add("T-QWANG", "Qinqin Wang");
            NameDic.Add("MICHXU", "Michael Xu");
            NameDic.Add("MXSHAO", "Shawn Shao");
            NameDic.Add("STEVCHEN", "Steven Chen");
            NameDic.Add("XIANC", "Xiang Cheng");
            NameDic.Add("VINZHU", "Vincent J Zhu");
            NameDic.Add("T-YUSFA", "Yusong Fang");
            BelongDic.Add("BAXIE", "OCS");
            BelongDic.Add("CATHYWEI", "OCS");
            BelongDic.Add("CHICHEN", "OCS");
            BelongDic.Add("WWANG", "OCS");
            BelongDic.Add("JACKYFAN", "OCS");
            BelongDic.Add("JEZH", "OCS");
            BelongDic.Add("JOJOSU", "OCS");
            BelongDic.Add("KELIAN", "OCS");
            BelongDic.Add("KEZUO", "OCS");
            BelongDic.Add("LEICAO", "OCS");
            BelongDic.Add("T-QWANG", "OCS");
            BelongDic.Add("MICHXU", "OCS");
            BelongDic.Add("MXSHAO", "OCS");
            BelongDic.Add("STEVCHEN", "OCS");
            BelongDic.Add("XIANC", "OCS");
            BelongDic.Add("VINZHU", "OCS");
            BelongDic.Add("T-YUSFA", "OCS");
        }
    }
}