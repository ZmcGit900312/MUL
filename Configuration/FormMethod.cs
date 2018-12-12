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
    public partial class FormMethod : Form
    {
        public static Cam CardMD { get; set; }=new Cam();

        public FormMethod()
        {
            InitializeComponent();
            AIMRadioButton.PerformClick();
        }

        private void MoMRadioButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            if(AIMRadioButton.Checked)CardMD = new Cam
            {
                Order = OrderCombox.SelectedIndex + 2,
                Interval = double.Parse(IntervalText.Text),
                NearCorrectionEps = double.Parse(NearTolerance.Text),
                Threshold = double.Parse(ThresholdText.Text),
                Dimension = Dimension2D.Checked ? 2 : 3,
                FillingStrategy = FillingType.Checked ? 1 : 0
            };
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AIMRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            AIMPanel.Enabled = AIMRadioButton.Checked;
            if (AIMPanel.Enabled)
            {
                IntervalText.Text = CardMD.Interval.ToString();
                ThresholdText.Text = CardMD.Threshold.ToString();
                NearTolerance.Text = CardMD.NearCorrectionEps.ToString();
                Dimension3D.Checked = CardMD.Dimension == 3;
                FillingType.Checked = CardMD.FillingStrategy == 1;
                OrderCombox.SelectedIndex = CardMD.Order - 2;
            }
        }
    }
}
