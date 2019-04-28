using System;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class FormExcitation : Form
    {

        internal static XmlElement ExcitationMod { get; set; } = null;

        public FormExcitation()
        {
            InitializeComponent();

            XmlNode a0 = ExcitationMod.ChildNodes[0];
            if (a0 != null)
            {
                PWNameText.Text = a0.Attributes[0].InnerText;
                int pol = int.Parse(a0.ChildNodes[0].InnerText);
                if (pol > 0) LeftRadioButton.Checked = true;
                else if (pol < 0) RightRadioButton.Checked = true;
                else LinearRadioButton.Checked = true;
                //Theta
                XmlNode node = a0.ChildNodes[1];
                PWThetaNumberText.Text = node.ChildNodes[0].InnerText;
                PWThetaStartText.Text = node.ChildNodes[1].InnerText;
                PWThetaIncrementText.Text = node.ChildNodes[2].InnerText;
                //Phi
                node = node.NextSibling;  
                PWPhiNumberText.Text = node.ChildNodes[0].InnerText;
                PWPhiStartText.Text = node.ChildNodes[1].InnerText;
                PWPhiIncrementText.Text = node.ChildNodes[2].InnerText;
                //Rotation
                node = node.NextSibling;
                RXText.Text = node.ChildNodes[0].InnerText;
                RYText.Text = node.ChildNodes[1].InnerText;
                RZText.Text = node.ChildNodes[2].InnerText;
                //EField
                node = node.NextSibling;
               
                EtaText.Text = node.ChildNodes[0].InnerText;
                MagenitudeText.Text = node.ChildNodes[1].InnerText;
                PhaseText.Text = node.ChildNodes[2].InnerText;
                EllipticityText.Text = node.ChildNodes[3].InnerText;
            }

        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ApplyButton_Click(object sender, EventArgs e)
        {
            XmlNode a0 = ExcitationMod.ChildNodes[0];
            if (a0 != null)
            {
                a0.Attributes[0].InnerText= PWNameText.Text;
                XmlNode node = a0.ChildNodes[0];
                node.InnerText = LeftRadioButton.Checked ? "-1" :
                    RightRadioButton.Checked ? "1" : "0";
                //Theta
                node = node.NextSibling;
                node.ChildNodes[0].InnerText= PWThetaNumberText.Text;
                node.ChildNodes[1].InnerText= PWThetaStartText.Text;
                node.ChildNodes[2].InnerText= PWThetaIncrementText.Text;
                //Phi
                node = node.NextSibling;
                node.ChildNodes[0].InnerText= PWPhiNumberText.Text;
                node.ChildNodes[1].InnerText= PWPhiStartText.Text;
                node.ChildNodes[2].InnerText= PWPhiIncrementText.Text;
                //Rotation
                node = node.NextSibling;
                node.ChildNodes[0].InnerText= RXText.Text;
                node.ChildNodes[1].InnerText= RYText.Text;
                node.ChildNodes[2].InnerText= RZText.Text;
                //EField
                node = node.NextSibling;
                node.ChildNodes[0].InnerText= EtaText.Text;
                node.ChildNodes[1].InnerText= MagenitudeText.Text;
                node.ChildNodes[2].InnerText= PhaseText.Text;
                node.ChildNodes[3].InnerText=EllipticityText.Text;
            }
            else
            {
                a0 = XmlTool.GetInstance.AddElementWithAttribute("A0", "Name", PWNameText.Text);
                ExcitationMod.AppendChild(a0);

                a0.AppendChild(XmlTool.GetInstance.AddElementWithText("Polarisation",
                    LeftRadioButton.Checked ? "-1" : RightRadioButton.Checked ? "1" : "0"));
                //Theta
                XmlElement node = XmlTool.GetInstance.Doc.CreateElement("Theta");
                a0.AppendChild(node);
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaNumber", PWThetaNumberText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaStart", PWThetaStartText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaIncrement", PWThetaIncrementText.Text));
                //Phi
                node = XmlTool.GetInstance.Doc.CreateElement("Phi");
                a0.AppendChild(node);
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiNumber", PWPhiNumberText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiStart", PWPhiStartText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiIncrement", PWPhiIncrementText.Text));
                //Rotation
                node = XmlTool.GetInstance.Doc.CreateElement("Rotation");
                a0.AppendChild(node);
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("x", RXText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("y", RYText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("z", RZText.Text));
                //EField
                node = XmlTool.GetInstance.Doc.CreateElement("EField");
                a0.AppendChild(node);
                
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("Eta", EtaText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("Magenitude", MagenitudeText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("Phase", PhaseText.Text));
                node.AppendChild(XmlTool.GetInstance.AddElementWithText("Ellipticity", EllipticityText.Text));
            }
                Close();
        }
        //Initialize Template
        internal static XmlElement InitialTemplate()
        {
            ExcitationMod = XmlTool.GetInstance.Doc.CreateElement("Excitation");

            //A0
            XmlElement a0 = XmlTool.GetInstance.AddElementWithAttribute("A0", "Name", "T0P90");
            ExcitationMod.AppendChild(a0);
            a0.AppendChild(XmlTool.GetInstance.AddElementWithText("Polarisation", "0"));
            //Theta
            XmlElement node = XmlTool.GetInstance.Doc.CreateElement("Theta");
            a0.AppendChild(node);
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaNumber", "1"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaStart", "0.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("ThetaIncrement", "1.0"));
            //Phi
            node = XmlTool.GetInstance.Doc.CreateElement("Phi");
            a0.AppendChild(node);
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiNumber", "1"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiStart", "90.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("PhiIncrement", "1.0"));
            //Rotation
            node = XmlTool.GetInstance.Doc.CreateElement("Rotation");
            a0.AppendChild(node);
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("x", "0.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("y", "0.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("z", "0.0"));
            //EField
            node = XmlTool.GetInstance.Doc.CreateElement("EField");
            a0.AppendChild(node);   
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("Eta", "0.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("Magenitude", "1.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("Phase", "0.0"));
            node.AppendChild(XmlTool.GetInstance.AddElementWithText("Ellipticity", "0.0"));
            return ExcitationMod;
        }
    }
}
