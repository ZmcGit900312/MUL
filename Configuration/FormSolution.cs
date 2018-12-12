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
    public partial class FormSolution : Form
    {
        public static Ccg Sol=new Ccg();

        public FormSolution()
        {
            InitializeComponent();
            MaxIterationsText.Text = Sol.Maxiteration.ToString();
            StopCriterionText.Text = Sol.Tolerance.ToString();
            StopMaxText.Text = Sol.MaxStopTolerance.ToString();
            SolutionTypeComboBox.SelectedIndex = Sol.SolutionType;
            PreconditionComboBox.SelectedIndex = Sol.PreConditionType;
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            Sol.SolutionType = SolutionTypeComboBox.SelectedIndex;
            if (Sol.SolutionType == 1)
            {
                Sol.Maxiteration = int.Parse(MaxIterationsText.Text);
                Sol.Tolerance = double.Parse(StopCriterionText.Text);
                Sol.MaxStopTolerance = double.Parse(StopMaxText.Text);
                Sol.PreConditionType = FormMethod.CardMD.FillingStrategy == 1 ? PreconditionComboBox.SelectedIndex : 0;
            }
          
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
