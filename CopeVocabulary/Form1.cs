using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CopeVocabulary
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bool order = checkBox1.Checked;
            StringBuilder sb1 = new StringBuilder();
            StringBuilder sb2 = new StringBuilder();
            OperationSet oper = new OperationSet();
            string input = textBox1.Text.Trim();
            string output = textBox2.Text.Trim();

            string[] source = oper.getVocabulary(input);
            string[] desti = oper.copeVocabulary(order, output, source);

            foreach (string s in source)
            {
                sb1.Append(s + "\r\n");
            }
            textBox3.Text = sb1.ToString();

            foreach (string s in desti)
            {
                sb2.Append(s + "\r\n");
            }
            textBox4.Text = sb2.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}