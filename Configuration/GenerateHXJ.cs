using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace Configuration
{
    public partial class Start : Form
    {
        public static Dictionary<Card, ICard> ContextCards { get; set; }=new Dictionary<Card, ICard>();
        public string BatDir { get; set; }
        private string CoreFullname { get; set; }
        private string ConfigurationFullName { get; set; }
        private const string Corename = @"\Core.exe";
        private readonly string _root;
        public Start()
        {
            InitializeComponent();
            _root= Directory.GetCurrentDirectory();
        }
        private void Start_Load(object sender, EventArgs e)
        {
            ContextCards.Clear();
            BatDir = _root + @"\Run.bat";
            CoreFullname = _root + Corename;
        }
        private void ExitButton_Click(object sender, EventArgs e)
        {
            ContextCards.Clear();
            MessageBox.Show(@"Exit The System, See You~~~");
            Application.Exit();
        }

        private void AIMRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            AIMPanel.Enabled = AIMRadioButton.Checked;
        }

        private void HorizonButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"Horizon";
            ThetaStartText.Text = @"90.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"1";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
        }

        private void VerticalButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"Vertical";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"1";
        }

        private void SphereButton_Click(object sender, EventArgs e)
        {
            FarFieldNameText.Text = @"Sphere";
            ThetaStartText.Text = @"0.0";
            ThetaIncrementText.Text = @"1.0";
            ThetaNumText.Text = @"181";
            PhiStartText.Text = @"0.0";
            PhiIncrementText.Text = @"1.0";
            PhiNumText.Text = @"360";
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog configSaveFileDialog = new SaveFileDialog {Filter = @"小雪球 |*.hxj" };//定义文件保存位置
            if (configSaveFileDialog.ShowDialog() == DialogResult.OK)//如果有保存路径
            {
                SaveButton.Enabled = false;
                ConfigurationFullName = configSaveFileDialog.FileName;
                ((Cin)ContextCards[Card.IN]).ProjectDir = Path.GetDirectoryName(configSaveFileDialog.FileName);
                ((Cin)ContextCards[Card.IN]).ProjectName = Path.GetFileNameWithoutExtension(configSaveFileDialog.FileName);
                StreamWriter sw = File.CreateText(configSaveFileDialog.FileName);

                foreach (var element in ContextCards) element.Value.Write(sw); 
                
                sw.WriteLine((int)Card.EN);
                sw.Flush();
                sw.Close();
                InformationText.Text=@"Configuration is written Successfully:";
                InformationText.AppendText("\n"+configSaveFileDialog.FileName);
#if DEBUG
                CoreFullname=@"E:\ZMC\Code\C_program\MUL\x64\Release\Core.exe ";
                string command= CoreFullname+ ConfigurationFullName;
                BatDir = @"E:\ZMC\Code\C_program\MUL\x64\Release\Run.bat";
                Generate_BatFile(command);
                RunButton.Enabled = true;
#else
                RunButton.Enabled = File.Exists(CoreFullname);
                if(!RunButton.Enabled) InformationText.AppendText("\n\nError:The C++ tools Core.exe can't be found!\n");
#endif
            }
        }

        private void ValidateButton_Click(object sender, EventArgs e)
        {
            //mesh
            if (!File.Exists(MeshFileRichText.Text))
            {
                InformationText.Text = @"Error:Please give the corrected Mesh File(*.nas)\n";
                return;
            }
            {
                Cin hxj = new Cin {MeshFile = MeshFileRichText.Text};

                if (NasRadioButton.Checked) hxj.FileType = 3;
                InsertContext(hxj);
            }
            //Frequency
            if (FrequecyText.Text == "")
            {
                InformationText.Text = @"Error:Please give the corrected Frequency\n";
                return;
            }
            {
                Cfr hxj = new Cfr {Frequency = double.Parse(FrequecyText.Text)};
                InsertContext(hxj);
            }
            //Method
            if (AIMRadioButton.Checked)
            {
                Cam hxj = new Cam
                {
                    Order = OrderCombox.SelectedIndex + 2,
                    Interval = double.Parse(IntervalText.Text),
                    NearCorrectionEps = double.Parse(NearTolerance.Text),
                    Threshold = double.Parse(ThresholdText.Text),
                    FillingStrategy=FillingType.Checked?1:0
                };
                if (Dimension2D.Checked) hxj.Dimension = 2;
                if (Dimension3D.Checked) hxj.Dimension = 3;
                InsertContext(hxj);
            }
            else if(ContextCards.ContainsKey(Card.AM))
                ContextCards.Remove(Card.AM);
            
            //FarField
            if (FarFieldNameText.Text == "")
            {
                InformationText.Text = @"Error:Please give the FarFieldName\n";
                return;
            }
            {
                Cff hxj = new Cff
                {
                    FarFileName = FarFieldNameText.Text,
                    ThetaNum = uint.Parse(ThetaNumText.Text),
                    PhiNum = uint.Parse(PhiNumText.Text),
                    ThetaStart = double.Parse(ThetaStartText.Text),
                    PhiStart = double.Parse(PhiStartText.Text),
                    ThetaIncrement = double.Parse(ThetaIncrementText.Text),
                    PhiIncrement = double.Parse(PWPhiIncrementText.Text)
                };
                if (RCSradioButton.Checked) hxj.FieldType = 0;
                InsertContext(hxj);
            }
            //Planewave
            if (PWNameText.Text == "")
            {
                InformationText.Text = @"Error:Please give the PlaneWaveName\n";
                return;
            }
            {
                Ca0 hxj = new Ca0
                {
                    RotationX = double.Parse(RXText.Text),
                    RotationY = double.Parse(RYText.Text),
                    RotationZ = double.Parse(RZText.Text),
                    ThetaNum = uint.Parse(PWThetaNumberText.Text),
                    PhiNum = uint.Parse(PWPhiNumberText.Text),
                    ThetaStart = double.Parse(PWThetaStartText.Text),
                    PhiStart = double.Parse(PWPhiStartText.Text),
                    ThetaIncrement = double.Parse(PWThetaIncrementText.Text),
                    PhiIncrement = double.Parse(PWPhiIncrementText.Text),
                    Magnitude = double.Parse(MagenitudeText.Text),
                    Phase = double.Parse(PhaseText.Text),
                    Eta = double.Parse(EtaText.Text),
                    ExcitationName = PWNameText.Text   
                };
                if (LeftRadioButton.Checked) hxj.Polarisation = -1;
                if (LinearRadioButton.Checked) hxj.Polarisation = 0;
                if (RightRadioButton.Checked) hxj.Polarisation = 1;
                
                InsertContext(hxj);
            }
            //Solution
            {
                Ccg hxj = new Ccg
                {
                    SolutionType = (uint) SolutionTypeComboBox.SelectedIndex,
                    Maxiteration = uint.Parse(MaxIterationsText.Text),
                    Tolerance = double.Parse(StopCriterionText.Text),
                    MaxStopTolerance = double.Parse(StopMaxText.Text),
                    PreConditionType = (uint)PreconditionComboBox.SelectedIndex
                };
                InsertContext(hxj);
            }

            InformationText.Text =@"Configuration File is Corrected!";
            SaveButton.Enabled = true;
        }
       

        private void LoadButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog configFileDialog = new OpenFileDialog{ Filter = @"小雪球 |*.hxj" }; //定义新的文件打开位置控件
            if (configFileDialog.ShowDialog() == DialogResult.OK)//如果有选择打开的文件
            {
                InformationText.Clear();
                InformationText.AppendText("Read Configuration File:\n"+configFileDialog.FileName+"\n");
                StreamReader sd = new StreamReader(configFileDialog.FileName);
                Card hxj = Card.EN;
                ContextCards.Clear();
                do
                {
                    var buffer = sd.ReadLine()?.Split();
                    if (buffer != null)
                    {
                        hxj = (Card) int.Parse(buffer[0]);
                        switch (hxj)
                        {
                            case Card.IN:
                                if (buffer[1] == "3") NasRadioButton.Checked = true;
                                MeshFileRichText.Text = buffer[3];
                                break;
                            case Card.A0:
                                PWNameText.Text = buffer[14];
                                RXText.Text = buffer[11];
                                RYText.Text = buffer[12];
                                RZText.Text = buffer[13];
                                MagenitudeText.Text = buffer[8];
                                PhaseText.Text = buffer[9];
                                EtaText.Text = buffer[10];
                                PWThetaNumberText.Text = buffer[1];
                                PWPhiNumberText.Text = buffer[2];
                                PWThetaStartText.Text = buffer[3];
                                PWPhiStartText.Text = buffer[4];
                                PWThetaIncrementText.Text = buffer[5];
                                PWPhiIncrementText.Text = buffer[6];
                                int p = int.Parse(buffer[7]);
                                if (p > 0) LeftRadioButton.Checked = true;
                                else if (p < 0) RightRadioButton.Checked = true;
                                else LinearRadioButton.Checked = true;
                                break;
                            case Card.CG:
                                MaxIterationsText.Text = buffer[3];
                                StopCriterionText.Text = buffer[4];
                                StopMaxText.Text = buffer[5];
                                SolutionTypeComboBox.SelectedIndex = int.Parse(buffer[1]);
                                PreconditionComboBox.SelectedIndex = int.Parse(buffer[2]);
                                break;
                            case Card.FF:
                                ThetaNumText.Text = buffer[2];
                                PhiNumText.Text = buffer[3];
                                ThetaStartText.Text = buffer[4];
                                PhiStartText.Text = buffer[5];
                                ThetaIncrementText.Text = buffer[6];
                                PhiIncrementText.Text = buffer[7];
                                FarFieldNameText.Text = buffer[8];
                                if (int.Parse(buffer[1]) == 0) RCSradioButton.Checked = true;
                                break;
                            case Card.FR:
                                FrequecyText.Text = buffer[1];
                                break;
                            case Card.AM:
                                AIMRadioButton.Checked = true;
                                IntervalText.Text = buffer[2];
                                ThresholdText.Text = buffer[3];
                                NearTolerance.Text = buffer[4];
                                if (buffer[5] == "3") Dimension3D.Checked = true;
                                else Dimension2D.Checked = true;
                                FillingType.Checked = buffer[6] == "1";
                                OrderCombox.SelectedIndex = int.Parse(buffer[1]) - 2;
                                break;
                        }
                    }
                } while (hxj!=Card.EN);
                sd.Close();
               
            }
        }     

        private void AddMeshButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog meshFileDialog = new OpenFileDialog { Filter = @"MeshFile(*.stl,*.nas)|*.stl;*.nas" }; //定义新的文件打开位置控件
            if (meshFileDialog.ShowDialog() != DialogResult.OK) return;
            MeshFileRichText.Text = meshFileDialog.FileName;//显示
        }
        //Insert in the dictionary if not exist
        private void InsertContext(ICard hxj)
        {
            if (ContextCards.ContainsKey(hxj.Type)) ContextCards[hxj.Type] = hxj;
            else { ContextCards.Add(hxj.Type, hxj); }
        }

        private void RunButton_Click(object sender, EventArgs e)
        {
            RunButton.Enabled = false;
            using (Process p = new Process())
            {
                string command = CoreFullname + " " + ConfigurationFullName;
                Generate_BatFile(command);
                
                p.StartInfo.FileName = BatDir;
                //p.StartInfo.UseShellExecute = false;
                //p.StartInfo.RedirectStandardInput = true;
                //p.StartInfo.RedirectStandardOutput = true;
                //p.StartInfo.RedirectStandardError = true;
                //proc.StartInfo.CreateNoWindow = true;                 
                //proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;//这里设置DOS窗口不显示，经实践可行
            
                p.Start();
                p.WaitForExit();
                p.Close();
                if(File.Exists(BatDir))File.Delete(BatDir);
            }
           
        }

        private void Generate_BatFile(string command)
        {
            FileStream fs = new FileStream(BatDir, FileMode.Create, FileAccess.Write);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(command);
            sw.Close();
            fs.Close();
            
        }

    }
}
