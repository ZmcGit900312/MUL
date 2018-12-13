using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Configuration
{
    public partial class FormRequest : Form
    {
        public static Dictionary<string,Cff> FarField { get; set; }=new Dictionary<string, Cff>();

        public FormRequest()
        {
            InitializeComponent();
        
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            RCSradioButton.Checked = true;

            foreach (var val in FarField)
            {
                FarFieldComboBox.Items.Add(val.Key);
            }
                       
        }

        private void HorizonButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"Horizon";
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void VerticalButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"VerticalPhi0";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"1";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void SphereButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"Sphere";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            if (FarFieldComboBox.Text == "")
            {
                FarFieldComboBox.BackColor = Color.Red;
                return;
            }

            var temp= new Cff
            {
                FieldType = 0,
                ThetaNum = int.Parse(ThetaNumText.Text),
                PhiNum = int.Parse(PhiNumText.Text),
                ThetaStart = double.Parse(ThetaStartText.Text),
                PhiStart = double.Parse(PhiStartText.Text),
                ThetaIncrement = double.Parse(ThetaIncrementText.Text),
                PhiIncrement = double.Parse(PhiIncrementText.Text),
                FarFileName = FarFieldComboBox.Text
            };
            
            if (FarField.ContainsKey(FarFieldComboBox.Text))
            {
                FarField[FarFieldComboBox.Text] = temp;
                FarFieldComboBox.BackColor = Color.LightBlue;
                
            }
            else
            {
                FarFieldComboBox.Items.Add(FarFieldComboBox.Text);
                FarField.Add(FarFieldComboBox.Text, temp);
                FarFieldComboBox.BackColor = Color.DarkSeaGreen;
            }
            
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            FarField.Remove(FarFieldComboBox.Text);
            FarFieldComboBox.Items.Remove(FarFieldComboBox.Text);
            FarFieldComboBox.BackColor = Color.White;



        }

        private void FarFieldComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {           
            string buffer = FarFieldComboBox.Text;
            if (buffer == "") return;
            var temp = FarField[buffer];
            //FarFieldComboBox.Text = temp.FarFileName;
            FarFieldComboBox.BackColor = Color.White;
            ThetaStartText.Text = temp.ThetaStart.ToString(CultureInfo.CurrentCulture);
            ThetaIncrementText.Text = temp.ThetaIncrement.ToString(CultureInfo.CurrentCulture);
            ThetaNumText.Text = temp.ThetaNum.ToString(CultureInfo.CurrentCulture);
            PhiStartText.Text = temp.PhiStart.ToString(CultureInfo.CurrentCulture);
            PhiIncrementText.Text = temp.PhiIncrement.ToString(CultureInfo.CurrentCulture);
            PhiNumText.Text = temp.PhiNum.ToString(CultureInfo.CurrentCulture);
            RCSradioButton.Checked = true;
#if DEBUG
            RequestNameLable.Text = temp.FarFileName;
#endif
        }
    }
}
