using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Configuration
{
    public partial class FormFile : Form
    {
        public static Cin Files { get; set; }=new Cin();

        public FormFile()
        {
            InitializeComponent();

            MeshFileText.Text = Files.MeshFile;
            NasRadioButton.Checked = Files.FileType == 3;
            
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            Files.MeshFile = MeshFileText.Text;
            if (NasRadioButton.Checked) Files.FileType = 3;
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
    }
}
