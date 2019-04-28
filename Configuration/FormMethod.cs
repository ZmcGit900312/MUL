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
using System.Xml;

namespace Configuration
{
    public partial class FormMethod : Form
    {
        internal static XmlElement MethodCard { get; set; }
        public FormMethod()
        {
            InitializeComponent();
          
            if (MethodCard != null)
            {
                FillingType.Checked=int.Parse(MethodCard.ChildNodes[0].InnerText) ==1;
                if (MethodCard.ChildNodes[1].Attributes?["Validate"].InnerText == "1")
                {
                    AIMRadioButton.Checked = true;
                    return;
                }
                if (MethodCard.ChildNodes[2].Attributes?["Validate"].InnerText == "1")
                {
                    MoMRadioButton.Checked = true;
                    return;
                }
            }
        }

        private void MoMRadioButton_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {

            MethodCard.ChildNodes[0].InnerText = FillingType.Checked ? "1" : "0";
            if (MoMRadioButton.Checked)
            {
                MethodCard.ChildNodes[1].Attributes["Validate"].InnerText = "0";
                MethodCard.ChildNodes[2].Attributes["Validate"].InnerText = "1";
            }

            if (AIMRadioButton.Checked)
            {
                XmlNode aim = MethodCard.ChildNodes[1];
                aim.Attributes["Validate"].InnerText = "1";
                MethodCard.ChildNodes[2].Attributes["Validate"].InnerText = "0";

                aim.ChildNodes[0].InnerText = (OrderCombox.SelectedIndex + 2).ToString();
                aim.ChildNodes[1].InnerText = IntervalText.Text;
                aim.ChildNodes[2].InnerText = ThresholdText.Text;
                aim.ChildNodes[3].InnerText = NearTolerance.Text;
                aim.ChildNodes[4].InnerText = Dimension2D.Checked ? "2" : "3";
                aim.ChildNodes[5].InnerText = VirtualGridTechniqueCheckBox.Checked?"1":"0";
            }

            Close();
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AIMRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            AIMPanel.Enabled = AIMRadioButton.Checked;
            if (AIMPanel.Enabled)
            {

                XmlNode aim = MethodCard.ChildNodes[1];
                OrderCombox.SelectedIndex = int.Parse(aim.ChildNodes[0].InnerText)-2;
                IntervalText.Text = aim.ChildNodes[1].InnerText;
                ThresholdText.Text = aim.ChildNodes[2].InnerText;
                NearTolerance.Text = aim.ChildNodes[3].InnerText;
                Dimension3D.Checked = aim.ChildNodes[4].InnerText == "3";
                VirtualGridTechniqueCheckBox.Checked = aim.ChildNodes[5].InnerText == "1";
            }
        }

        //Initialize the Template
        internal static XmlElement InitialTemplate()
        {
            MethodCard = XmlTool.GetInstance.Doc.CreateElement("Method");
            MethodCard.AppendChild(XmlTool.GetInstance.AddElementWithText("TriangleFillingStrategy", "1"));
            //AM

            XmlElement card = XmlTool.GetInstance.AddElementWithAttribute("AIM", "Validate", "1");
            
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Order", "2"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Interval", "0.05"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Threshold", "0.3"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("NearTolerance", "0.01"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Dimension", "3"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("VirtualGrid", "1"));
            MethodCard.AppendChild(card);
            MethodCard.AppendChild(XmlTool.GetInstance.AddElementWithAttribute("MoM", "Validate", "0"));

            return MethodCard;
        }
    }
}
