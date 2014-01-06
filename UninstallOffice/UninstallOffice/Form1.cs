using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Helper;

namespace UninstallOffice
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            InitialFormSet();
        }

        private void InitialFormSet()
        {
            this.comboBox1.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.InitialDirectory = "C:\\";
            fileDialog.Filter = "executable files(*.exe)|*.exe|All files(*.*)|*.*";
            fileDialog.Title = "Select a executable file";
            fileDialog.FilterIndex = 1;
            fileDialog.RestoreDirectory = true;

            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = fileDialog.FileName;
            }
            else
            {
                textBox1.Text = "";
            }
        }

        private void comboBox1_SelectedValueChanged(object sender, EventArgs e)
        {
            string value = comboBox1.Text;
            List<string> office = new List<string>();

            if (!string.IsNullOrEmpty(value))
            {
                label3.Text = value + " Components";
                checkedListBox1.Items.Clear();

                if (value == "Office 2007")
                {
                    office = IOdealer.ReadFile(IOdealer.Office2007);
                }
                else if (value == "Office 2010")
                {
                    office = IOdealer.ReadFile(IOdealer.Office2010);
                }
                foreach (string s in office)
                {
                    checkedListBox1.Items.Add(s);
                }
            }
            else
            {
                label3.Text = "No version selected";
                checkedListBox1.Items.Clear();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int i = 0;
            string path = "";
            string strOutput = null;
            List<string> check = new List<string>();

            try
            {
                path = textBox1.Text.Trim();
                label4.Text = "";
                // if (!string.IsNullOrEmpty(path))
                if (JudgeSet.JudgeInputPath(path))
                {
                    for (i = 0; i < checkedListBox1.Items.Count; i++)
                    {
                        if (checkedListBox1.GetItemChecked(i))
                        {
                            check.Add(checkedListBox1.GetItemText(checkedListBox1.Items[i]));
                        }
                    }
                    if (check.Count > 0)
                    {
                        strOutput = ExecuteDosCommand.ExeCommand(textBox1.Text.Trim(), check);
                    }
                }
            }
            catch (Exception ex)
            {
                label4.Text = ex.Message;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textBox1.Text))
            {
                label4.Text = "";
            }
        }
    }
}
