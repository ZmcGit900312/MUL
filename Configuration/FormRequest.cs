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
    public partial class FormRequest : Form
    {
        public static List<Cff> FarField { get; set; }=new List<Cff>();

        public FormRequest()
        {
            InitializeComponent();

            FarFieldNameText.Text = @"Horizon";
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            RCSradioButton.Checked = true;
            
            FarFieldComboBox.DataSource = FarField;
            FarFieldComboBox.DisplayMember = "FarFileName";
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void HorizonButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"Horizon";
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
        }

        private void VerticalButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"VerticalPhi0";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"1";
        }

        private void SphereButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"Sphere";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
        }

        private void AddButton_Click(object sender, EventArgs e)
        {

                //FarFieldNameText.BackColor=Color.Red;

            {
                FarField.Add(new Cff
                {
                    FieldType = 0,
                    ThetaNum = int.Parse(ThetaNumText.Text),
                    PhiNum = int.Parse(PhiNumText.Text),
                    ThetaStart = double.Parse(ThetaStartText.Text),
                    PhiStart = double.Parse(PhiStartText.Text),
                    ThetaIncrement = double.Parse(ThetaIncrementText.Text),
                    PhiIncrement = double.Parse(PhiIncrementText.Text),
                    FarFileName = FarFieldNameText.Text
                });
              
            }
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            string selstring = FarFieldComboBox.SelectedText;
        }
    }
}
