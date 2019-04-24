using System;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class FormBasicFunction : Form
    {
#if HXJ
        public static Cos BasicFunction { get; set; }=new Cos();
#else
        internal static XmlElement OSCard { get; set; }
#endif
        public FormBasicFunction()
        {
            InitializeComponent();
#if HXJ
            BasicFunctionCheck.Checked = BasicFunction.BasicFunctionType != -1;
            BasicFunctionGroupBox.Enabled = BasicFunctionCheck.Checked;
            BFFileText.Text = BasicFunction.BasicFunctionName;//显示
#else
            if (OSCard != null)
            {
                BasicFunctionCheck.Checked = int.Parse(OSCard.Attributes["Validate"].Value) == 1;
                BasicFunctionGroupBox.Enabled = BasicFunctionCheck.Checked;
                if (BasicFunctionGroupBox.Enabled)
                {
                    RWGRadioButton.Checked = OSCard.ChildNodes[0].InnerText != "-1";
                    BFFileText.Text = OSCard.ChildNodes[1].InnerText;
                }

            }
            else
            {
                OSCard = XmlTool.GetInstance.Doc.CreateElement("OS");
                XmlTool.GetInstance.Root.SelectSingleNode("/Configuration/File")?.AppendChild(OSCard);
            }
#endif

        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog bfFileDialog = new OpenFileDialog { Filter = @"BasicFunctionFile(*.bf)|*.bf" }; //定义新的文件打开位置控件
            if (bfFileDialog.ShowDialog() != DialogResult.OK) return;
            BFFileText.Text = bfFileDialog.FileName;//显示
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
#if HXJ
            if (BasicFunctionCheck.Checked&& File.Exists(BFFileText.Text))
            {
                BasicFunction.BasicFunctionName = BFFileText.Text;
                if (RWGRadioButton.Checked) BasicFunction.BasicFunctionType = 0;        
            }
            else BasicFunction.BasicFunctionType = -1;//no check code
#else
            OSCard.RemoveAll();
            if (BasicFunctionCheck.Checked && File.Exists(BFFileText.Text))
            {
                OSCard.SetAttribute("Validate", "1");
                OSCard.AppendChild(XmlTool.GetInstance.AddElementWithText("Type", RWGRadioButton.Checked?"0":"-1"));
                OSCard.AppendChild(XmlTool.GetInstance.AddElementWithText("FilePath", BFFileText.Text));
            }
            else OSCard.SetAttribute("Validate", "0");
#endif
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

        //Initialize the Template
        internal static XmlElement InitialTemplate()
        {
            OSCard = XmlTool.GetInstance.AddElementWithAttribute("OS", "Validate", "1");
            OSCard.AppendChild(XmlTool.GetInstance.AddElementWithText("Type", "0"));
            OSCard.AppendChild(XmlTool.GetInstance.AddElementWithText("FilePath", @"E:\ZMC\Code\C_program\MUL\SourceData\test.bf"));
            return OSCard;
        }
    }
}
