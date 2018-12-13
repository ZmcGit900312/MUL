using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Configuration
{
    public partial class FormBasicFunction : Form
    {
        public static Cos BasicFunction { get; set; }=new Cos();

        public FormBasicFunction()
        {
            InitializeComponent();

            BasicFunctionCheck.Checked = BasicFunction.BasicFunctionType != -1;
            BasicFunctionGroupBox.Enabled = BasicFunctionCheck.Checked;
            BFFileText.Text = BasicFunction.BasicFunctionName;//显示
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog bfFileDialog = new OpenFileDialog { Filter = @"BasicFunctionFile(*.bf)|*.bf" }; //定义新的文件打开位置控件
            if (bfFileDialog.ShowDialog() != DialogResult.OK) return;
            BFFileText.Text = bfFileDialog.FileName;//显示
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            if (BasicFunctionCheck.Checked&& File.Exists(BFFileText.Text))
            {
                BasicFunction.BasicFunctionName = BFFileText.Text;
                if (RWGRadioButton.Checked) BasicFunction.BasicFunctionType = 0;        
            }
            else BasicFunction.BasicFunctionType = -1;//no check code
            Close();
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void BasicFunctionCheck_CheckedChanged(object sender, EventArgs e)
        {
            BasicFunctionGroupBox.Enabled = BasicFunctionCheck.Checked;
        }
    }
}
