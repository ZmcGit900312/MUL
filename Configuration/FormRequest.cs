using System;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class FormRequest : Form
    {
#if HXJ
        public static Dictionary<string,Cff> FarField { get; set; }=new Dictionary<string, Cff>();
#else
        internal static XmlElement RequestMod { get; set; }
#endif
        public FormRequest()
        {
            InitializeComponent();
        
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            RCSradioButton.Checked = true;

#if HXJ
            foreach (var val in FarField)FarFieldComboBox.Items.Add(val.Key);
#else
            foreach (XmlNode val in RequestMod.ChildNodes)
            {
                if (val?.Name=="FF")
                {
                    FarFieldComboBox.Items.Add(val.Attributes[0].InnerText);
                }
            }
            
#endif

        }

        private void HorizonButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"Horizon";
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void VerticalButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"VerticalPhi0";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"1";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void SphereButton_Click(object sender, EventArgs e)
        {
            FarFieldComboBox.Text = @"Sphere";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
            FarFieldComboBox.BackColor = Color.White;
        }

        private void CloseButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            if (FarFieldComboBox.Text == "")
            {
                FarFieldComboBox.BackColor = Color.Red;
                return;
            }
#if HXJ
            var temp= new Cff
            {
                FieldType = 0,
                ThetaNum = int.Parse(ThetaNumText.Text),
                PhiNum = int.Parse(PhiNumText.Text),
                ThetaStart = double.Parse(ThetaStartText.Text),
                PhiStart = double.Parse(PhiStartText.Text),
                ThetaIncrement = double.Parse(ThetaIncrementText.Text),
                PhiIncrement = double.Parse(PhiIncrementText.Text),
                FarFileName = FarFieldComboBox.Text
            };
            
            if (FarField.ContainsKey(FarFieldComboBox.Text))
            {
                FarField[FarFieldComboBox.Text] = temp;
                FarFieldComboBox.BackColor = Color.LightBlue;
                
            }
            else
            {
                FarFieldComboBox.Items.Add(FarFieldComboBox.Text);
                FarField.Add(FarFieldComboBox.Text, temp);
                FarFieldComboBox.BackColor = Color.DarkSeaGreen;
            }
#else
            string buffer = "/Configuration/Request/FF[@FarField='" + FarFieldComboBox.Text + "']";
            XmlNode ff = XmlTool.Root.SelectSingleNode(buffer);

            if (ff != null)
            {
                ff.FirstChild.ChildNodes[0].InnerText= ThetaNumText.Text;
                ff.FirstChild.ChildNodes[1].InnerText= ThetaStartText.Text;
                ff.FirstChild.ChildNodes[2].InnerText= ThetaIncrementText.Text ;

                ff.LastChild.ChildNodes[0].InnerText= PhiNumText.Text;
                ff.LastChild.ChildNodes[1].InnerText= PhiStartText.Text ;
                ff.LastChild.ChildNodes[2].InnerText= PhiIncrementText.Text;

                FarFieldComboBox.BackColor = Color.LightBlue;
            }
            else
            {
                ff = XmlTool.AddElementWithAttribute("FF","FarField", FarFieldComboBox.Text);
                RequestMod.AppendChild(ff);

                XmlElement node = XmlTool.Doc.CreateElement("Theta");
                ff.AppendChild(node);

                node.AppendChild(XmlTool.AddElementWithText("ThetaNumber", ThetaNumText.Text));
                node.AppendChild(XmlTool.AddElementWithText("ThetaStart", ThetaStartText.Text));
                node.AppendChild(XmlTool.AddElementWithText("ThetaIncrement", ThetaIncrementText.Text));

                //Phi
                node = XmlTool.Doc.CreateElement("Phi");
                ff.AppendChild(node);

                node.AppendChild(XmlTool.AddElementWithText("PhiNumber", PhiNumText.Text));
                node.AppendChild(XmlTool.AddElementWithText("PhiStart", PhiStartText.Text));
                node.AppendChild(XmlTool.AddElementWithText("PhiIncrement", PhiIncrementText.Text));

                FarFieldComboBox.Items.Add(FarFieldComboBox.Text);
            }
#endif

        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
#if HXJ
            FarField.Remove(FarFieldComboBox.Text);
#else
            string buffer = "/Configuration/Request/FF[@FarField='" + FarFieldComboBox.Text + "']";
            RequestMod.RemoveChild(XmlTool.Root.SelectSingleNode(buffer));
#endif
            FarFieldComboBox.Items.Remove(FarFieldComboBox.Text);
            FarFieldComboBox.BackColor = Color.White;
        }

        private void FarFieldComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
#if HXJ
            string buffer = FarFieldComboBox.Text;
            if (buffer == "") return;

            var temp = FarField[buffer];      
            
            ThetaStartText.Text = temp.ThetaStart.ToString(CultureInfo.CurrentCulture);
            ThetaIncrementText.Text = temp.ThetaIncrement.ToString(CultureInfo.CurrentCulture);
            ThetaNumText.Text = temp.ThetaNum.ToString(CultureInfo.CurrentCulture);
            PhiStartText.Text = temp.PhiStart.ToString(CultureInfo.CurrentCulture);
            PhiIncrementText.Text = temp.PhiIncrement.ToString(CultureInfo.CurrentCulture);
            PhiNumText.Text = temp.PhiNum.ToString(CultureInfo.CurrentCulture);
            
#if DEBUG
            RequestNameLable.Text = temp.FarFileName;
#endif
#else
            string buffer = "/Configuration/Request/FF[@FarField='"+ FarFieldComboBox.Text + "']";
            XmlNode ff =XmlTool.Root.SelectSingleNode(buffer);

            if (ff == null) return;

            ThetaNumText.Text=ff.FirstChild.ChildNodes[0].InnerText;
            ThetaStartText.Text = ff.FirstChild.ChildNodes[1].InnerText;
            ThetaIncrementText.Text = ff.FirstChild.ChildNodes[2].InnerText;

            PhiNumText.Text = ff.LastChild.ChildNodes[0].InnerText;
            PhiStartText.Text = ff.LastChild.ChildNodes[1].InnerText;
            PhiIncrementText.Text = ff.LastChild.ChildNodes[2].InnerText;
#endif
            RCSradioButton.Checked = true;
            FarFieldComboBox.BackColor = Color.White;
        }

        internal static XmlElement InitialTemplate()
        {
            RequestMod = XmlTool.Doc.CreateElement("Request");

            //FF
            XmlElement card = XmlTool.AddElementWithAttribute("FF", "FarField", "Horizon");
            RequestMod.AppendChild(card);

            //Theta
            XmlElement node = XmlTool.Doc.CreateElement("Theta");
            card.AppendChild(node);

            node.AppendChild(XmlTool.AddElementWithText("ThetaNumber", "1"));
            node.AppendChild(XmlTool.AddElementWithText("ThetaStart", "90.0"));
            node.AppendChild(XmlTool.AddElementWithText("ThetaIncrement", "1.0"));

            //Phi
            node = XmlTool.Doc.CreateElement("Phi");
            card.AppendChild(node);

            node.AppendChild(XmlTool.AddElementWithText("PhiNumber", "361"));
            node.AppendChild(XmlTool.AddElementWithText("PhiStart", "0.0"));
            node.AppendChild(XmlTool.AddElementWithText("PhiIncrement", "1.0"));
            return RequestMod;
        }
    }
}
