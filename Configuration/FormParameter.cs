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
    public partial class FormParameter : Form
    {
        public static Cfr Para { get; set; }=new Cfr();

        public FormParameter()
        {
            InitializeComponent();
            FrequecyText.Text = Para.Frequency.ToString();
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            Para.Frequency = double.Parse(FrequecyText.Text);
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
