using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Deployment.Application;
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
                    
                }
                if (MethodCard.ChildNodes[2].Attributes?["Validate"].InnerText == "1")
                {
                    MoMRadioButton.Checked = true;                  
                }

                XmlNode iecard = MethodCard.ChildNodes[3];
                var ie = int.Parse(iecard.ChildNodes[0].InnerText);

                switch (ie)
                {
                    case 1:
                        MFIEButton.Checked = true;
                        break;
                    case 2:
                        CFIEButton.Checked = true;
                        break;
                    case 3:
                        IBCEFIEButton.Checked = true;
                        break;
                    case 4:
                        IBCMFIEButton.Checked = true;
                        break;
                    case 5:
                        IBCCFIEButton.Checked = true;
                        break;
                    default:
                        EFIEButton.Checked = true;
                        break;
                }

                AlphaText.Text = iecard.ChildNodes[1].InnerText;
                EtaText.Text = iecard.ChildNodes[2].InnerText;
                ZsText.Text = iecard.ChildNodes[3].InnerText;
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

            var type="0";

            if (EFIEButton.Checked) type = "0";
            if (MFIEButton.Checked) type = "1";
            if (CFIEButton.Checked) type = "2";
            if (IBCEFIEButton.Checked) type = "3";
            if (IBCMFIEButton.Checked) type = "4";
            if (IBCCFIEButton.Checked) type = "5";

            XmlNode ie = MethodCard.ChildNodes[3];
            ie.Attributes["Validate"].InnerText = "1";
            ie.ChildNodes[0].InnerText = type;
            ie.ChildNodes[1].InnerText = AlphaText.Text;
            ie.ChildNodes[2].InnerText = EtaText.Text;
            ie.ChildNodes[3].InnerText = ZsText.Text;
                
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


            card = XmlTool.GetInstance.AddElementWithAttribute("IE", "Validate", "1");

            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Type", "0"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Alpha", "0.3"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Eta", "376.991112"));
            card.AppendChild(XmlTool.GetInstance.AddElementWithText("Zs", "376.991112"));
            MethodCard.AppendChild(card);

            return MethodCard;
        }
    }
}
