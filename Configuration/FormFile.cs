using System;
using System.Windows.Forms;
using System.Xml;


namespace Configuration
{
    public partial class FormFile : Form
    {
#if HXJ
        public static Cin Files { get; set; }=new Cin();
#else
        internal static XmlElement MeshCard { get; set; }
        internal static XmlNode Project { get; set; }
#endif

        public FormFile()
        {
            InitializeComponent();

#if HXJ
            MeshFileText.Text = Files.MeshFile; 
            NasRadioButton.Checked = Files.FileType == 3;
#else
            if (MeshCard != null)
            {
                foreach (XmlElement node in MeshCard.ChildNodes)
                {
                    switch (node.Name)
                    {
                        case "FilePath":
                            MeshFileText.Text = node.InnerText;
                            continue;
                        default: continue;
                    }
                }
                NasRadioButton.Checked = int.Parse(MeshCard.Attributes["Type"].Value) == 3;
            }
            else
            {
                MeshCard = XmlTool.GetInstance.Doc.CreateElement("Mesh");
                XmlTool.GetInstance.Root.FirstChild.AppendChild(MeshCard);
                NasRadioButton.Checked = true;
            }      
#endif

        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
#if HXJ
            Files.MeshFile = MeshFileText.Text;
            if (NasRadioButton.Checked) Files.FileType = 3;
#else
            MeshCard.Attributes["Type"].InnerText = NasRadioButton.Checked ? "3" : "0";
            MeshCard.ChildNodes[0].InnerText = MeshFileText.Text;
            //MeshCard.RemoveAll();
            //MeshCard.AppendChild(XmlTool.AddElementWithText("FilePath", MeshFileText.Text));
            //MeshCard.SetAttribute("Type", NasRadioButton.Checked ? "3" : "0");
#endif
            Close();
            
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AddMeshButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog meshFileDialog = new OpenFileDialog { Filter = @"MeshFile(*.gmsh,*.nas)|*.gmsh;*.nas" }; //定义新的文件打开位置控件
            if (meshFileDialog.ShowDialog() != DialogResult.OK) return;
            MeshFileText.Text = meshFileDialog.FileName;//显示
        }

        //Initialize the Template
        internal static XmlElement InitialTemplate()
        {
            XmlElement card= XmlTool.GetInstance.AddElementWithAttribute("IN", "Validate", "1");
            //mesh---
            
            MeshCard = XmlTool.GetInstance.AddElementWithAttribute("Mesh", "Type", "3");
            MeshCard.AppendChild(XmlTool.GetInstance.AddElementWithText("FilePath",
                @"E:\ZMC\Code\C_program\MUL\SourceData\MeshTest.nas"));
            //project---
            card.AppendChild(MeshCard);
            
            Project = XmlTool.GetInstance.Doc.CreateElement("Project");
            Project.AppendChild(XmlTool.GetInstance.AddElementWithText("Name", "test"));
            Project.AppendChild(XmlTool.GetInstance.AddElementWithText("Directory", @"E:\ZMC\Code\C_program\MUL\SourceData"));
            Project.AppendChild(XmlTool.GetInstance.AddElementWithText("Report", @"E:\ZMC\Code\C_program\MUL\SourceData\test.out"));
            card.AppendChild(Project);
            return card;
        }
    }
}
