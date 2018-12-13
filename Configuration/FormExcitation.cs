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
    public partial class FormExcitation : Form
    {
        public static Ca0 Planewave=new Ca0();
        public FormExcitation()
        {
            InitializeComponent();

            PWNameText.Text = Planewave.ExcitationName;
            RXText.Text = Planewave.RotationX.ToString();
            RYText.Text = Planewave.RotationY.ToString();
            RZText.Text = Planewave.RotationZ.ToString();
            MagenitudeText.Text = Planewave.Magnitude.ToString();
            PhaseText.Text = Planewave.Phase.ToString();
            EtaText.Text = Planewave.Eta.ToString();
            PWThetaNumberText.Text = Planewave.ThetaNum.ToString();
            PWPhiNumberText.Text = Planewave.PhiNum.ToString();
            PWThetaStartText.Text = Planewave.ThetaStart.ToString();
            PWPhiStartText.Text = Planewave.PhiStart.ToString();
            PWThetaIncrementText.Text = Planewave.ThetaIncrement.ToString();
            PWPhiIncrementText.Text = Planewave.PhiIncrement.ToString();
            if (Planewave.Polarisation > 0) LeftRadioButton.Checked = true;
            else if (Planewave.Polarisation < 0) RightRadioButton.Checked = true;
            else LinearRadioButton.Checked = true;
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            Planewave.RotationX = double.Parse(RXText.Text);
            Planewave.RotationY = double.Parse(RYText.Text);
            Planewave.RotationZ = double.Parse(RZText.Text);
            Planewave.ThetaNum = uint.Parse(PWThetaNumberText.Text);
            Planewave.PhiNum = uint.Parse(PWPhiNumberText.Text);
            Planewave.ThetaStart = double.Parse(PWThetaStartText.Text);
            Planewave.PhiStart = double.Parse(PWPhiStartText.Text);
            Planewave.ThetaIncrement = double.Parse(PWThetaIncrementText.Text);
            Planewave.PhiIncrement = double.Parse(PWPhiIncrementText.Text);
            Planewave.Magnitude = double.Parse(MagenitudeText.Text);
            Planewave.Phase = double.Parse(PhaseText.Text);
            Planewave.Eta = double.Parse(EtaText.Text);
            Planewave.ExcitationName = PWNameText.Text;
            Planewave.Polarisation = LeftRadioButton.Checked ? -1 : RightRadioButton.Checked ? 1 : 0;
            Close();
        }
    }
}
