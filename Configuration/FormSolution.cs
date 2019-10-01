using System;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class FormSolution : Form
    {
#if HXJ
        public static Ccg Sol=new Ccg();
#else
        internal static XmlElement SolutionMod { get; set; } = null;
#endif
        public FormSolution()
        {
            InitializeComponent();
#if HXJ
            MaxIterationsText.Text = Sol.Maxiteration.ToString();
            StopCriterionText.Text = Sol.Residum.ToString();
            StopMaxText.Text = Sol.StopTolerance.ToString();
            SolutionTypeComboBox.SelectedIndex = Sol.SolutionType;
            PreconditionComboBox.SelectedIndex = Sol.PreConditionType;
#else
            XmlNode cg = SolutionMod.ChildNodes[0];
            if (cg.Attributes["Validate"].InnerText == "1")
            {
                SolutionTypeComboBox.SelectedIndex = int.Parse(cg.ChildNodes[0].InnerText);
                MaxIterationsText.Text = cg.ChildNodes[1].InnerText;
                StopMaxText.Text = cg.ChildNodes[2].InnerText;
                StopCriterionText.Text = cg.ChildNodes[3].InnerText;
                PreconditionComboBox.SelectedIndex = int.Parse(cg.ChildNodes[4].InnerText);
            }
#endif

        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
#if HXJ
            Sol.SolutionType = SolutionTypeComboBox.SelectedIndex;
            if (Sol.SolutionType == 0)
            {
                Sol.Maxiteration = int.Parse(MaxIterationsText.Text);
                Sol.Residum = double.Parse(StopCriterionText.Text);
                Sol.StopTolerance = double.Parse(StopMaxText.Text);
                Sol.PreConditionType = PreconditionComboBox.SelectedIndex;
            }
#else
            XmlNode cg = SolutionMod.ChildNodes[0];
            cg.Attributes[0].InnerText = "0";
            if (SolutionTypeComboBox.SelectedIndex == 0)
            {
                cg.Attributes[0].InnerText = "1";
                cg.ChildNodes[0].InnerText = SolutionTypeComboBox.SelectedIndex.ToString();
                cg.ChildNodes[1].InnerText = MaxIterationsText.Text;
                cg.ChildNodes[2].InnerText = StopMaxText.Text;
                cg.ChildNodes[3].InnerText = StopCriterionText.Text;
                cg.ChildNodes[4].InnerText = PreconditionComboBox.SelectedIndex.ToString();
            }           
#endif
            Close();
        }

        private void CloseButton_Click(object sender, EventArgs e)=>Close();

        //Initial Template
        internal static XmlElement InitialTemplate()
        {
            SolutionMod = XmlTool.GetInstance.Doc.CreateElement("Solution");
            XmlElement card = XmlTool.GetInstance.AddElementWithAttribute("CG", "Validate", "1");
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("SolutionType", "0"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Iterations", "0"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("StopTolerance", "10.0"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Residum", "1.0e-4"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Preconditioning", "2"));
            SolutionMod.AppendChild(card);
            return SolutionMod;
        }
    }
}
