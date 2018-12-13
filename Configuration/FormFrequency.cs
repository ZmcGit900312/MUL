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
    public partial class FormFrequency : Form
    {
        public static Cfr fre { get; set; }=new Cfr();

        public FormFrequency()
        {
            InitializeComponent();
            FrequecyText.Text = fre.Frequency.ToString();
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            fre.Frequency = double.Parse(FrequecyText.Text);
            Close();
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
