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
    public partial class FormMesh : Form
    {
        public static Cin CardIN { get; set; }=new Cin();

        public FormMesh()
        {
            InitializeComponent();

            MeshFileRichText.Text = CardIN.MeshFile;
            NasRadioButton.Checked = CardIN.FileType == 3;
            
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            CardIN.MeshFile = MeshFileRichText.Text;
            if (NasRadioButton.Checked) CardIN.FileType = 3;
            Close();
            
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AddMeshButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog meshFileDialog = new OpenFileDialog { Filter = @"MeshFile(*.gmsh,*.nas)|*.gmsh;*.nas" }; //定义新的文件打开位置控件
            if (meshFileDialog.ShowDialog() != DialogResult.OK) return;
            MeshFileRichText.Text = meshFileDialog.FileName;//显示
        }

        private void AddBFButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog bfFileDialog = new OpenFileDialog { Filter = @"BasicFunctionFile(*.bf)|*.bf" }; //定义新的文件打开位置控件
            if (bfFileDialog.ShowDialog() != DialogResult.OK) return;
            BFFileRichText.Text = bfFileDialog.FileName;//显示
        }
    }
}
