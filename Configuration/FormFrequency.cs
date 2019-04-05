using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class FormFrequency : Form
    {
#if HXJ
        public static Cfr fre { get; set; }=new Cfr();
#else
        internal static XmlElement ParameterMod { get; set; } = null;
#endif
        public FormFrequency()
        {
            InitializeComponent();
#if HXJ
            FrequecyText.Text = fre.Frequency.ToString();
#else
            XmlNode card = ParameterMod.ChildNodes[0];
            if (card.Attributes["Validate"].InnerText == "1")
            {
                FrequecyText.Text = card.ChildNodes[0].InnerText;
            }
#endif

        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
#if HXJ         
            fre.Frequency = double.Parse(FrequecyText.Text);
#else
            XmlNode card = ParameterMod.ChildNodes[0];
            card.Attributes["Validate"].InnerText = "1";
            card.ChildNodes[0].InnerText = FrequecyText.Text;
#endif
            Close();
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }
        //Initial Template
        internal static XmlElement InitialTemplate()
        {
            ParameterMod = XmlTool.Doc.CreateElement("EMCPara");
            XmlElement card = XmlTool.AddElementWithAttribute("FR", "Validate", "1");
            card.AppendChild(XmlTool.AddElementWithText("SingleFrequency", "3.0e8"));
            ParameterMod.AppendChild(card);
            return ParameterMod;
        }
    }
}
