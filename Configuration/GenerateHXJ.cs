using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace Configuration
{
    public partial class Start : Form
    {
        public string BatDir { get; set; }
        private string CoreFullname { get; set; }
        private string ConfigurationFullName { get; set; }
        private const string Corename = @"\Core.exe";
        private readonly string _root;

        public Start()
        {
            InitializeComponent();         
            _root= Directory.GetCurrentDirectory();
            XmlTool.GetInstance.Initialization(@"E:\ZMC\Code\C_program\MUL\Configuration\Resources\ConfigurationTemplate.xsd");
#if DEBUG
            TestButton.Enabled = true;
#else
            TestButton.Enabled=false;
#endif
        }

        private void Start_Load(object sender, EventArgs e)
        {
            BatDir = _root + @"\Run.bat";
            CoreFullname = _root + Corename;
            
        }


#region commandAPI
        private void ResultButton_Click(object sender, EventArgs e)
        {
#if HXJ
            string resultFile = FormFile.Files.ProjectDir + '\\' + FormFile.Files.ProjectName + ".out";
#else
            string resultFile;           
            if (FormFile.Project != null)resultFile = FormFile.Project.ChildNodes[2].InnerText;
            else return;
            
#endif
            if (File.Exists(resultFile)) Process.Start("notepad.exe", resultFile);
            else InformationText.Text = @"Not exist file:" + resultFile;
        }

        private void ExitButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show(@"Exit The System, See You~~~");
            Application.Exit();
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
#if HXJ
            SaveFileDialog configSaveFileDialog = new SaveFileDialog { Filter = @"小雪球 |*.hxj" };//定义文件保存位置
#else
            SaveFileDialog configSaveFileDialog = new SaveFileDialog { Filter = @"配置文件 |*.xml" };//定义文件保存位置
#endif
            if (configSaveFileDialog.ShowDialog() == DialogResult.OK)//如果有保存路径
            {
                SaveButton.Enabled = false;
                ConfigurationFullName = configSaveFileDialog.FileName;
#if HXJ
                FormFile.Files.ProjectDir = Path.GetDirectoryName(configSaveFileDialog.FileName);
                FormFile.Files.ProjectName = Path.GetFileNameWithoutExtension(configSaveFileDialog.FileName);
                StreamWriter sw = File.CreateText(configSaveFileDialog.FileName);

                //Write
                FormFile.Files.Write(sw);
                if(FormBasicFunction.BasicFunction.BasicFunctionType>-1) FormBasicFunction.BasicFunction.Write(sw);
                FormMethod.Impedance.Write(sw);
                FormFrequency.fre.Write(sw);
                FormExcitation.Planewave.Write(sw);
                FormSolution.Sol.Write(sw);

                foreach (var val in FormRequest.FarField)val.Value.Write(sw);
                              
                sw.WriteLine((int)Card.EN);
                sw.Flush();
                sw.Close();
#else
                FormFile.Project.ChildNodes[0].InnerText = Path.GetFileNameWithoutExtension(configSaveFileDialog.FileName);
                FormFile.Project.ChildNodes[1].InnerText = Path.GetDirectoryName(configSaveFileDialog.FileName);
                FormFile.Project.ChildNodes[2].InnerText =
                    FormFile.Project.ChildNodes[1].InnerText + '\\' + FormFile.Project.FirstChild.InnerText + ".out";
                XmlTool.GetInstance.Root.Attributes["Date"].InnerText = DateTime.Now.ToString("s");

                XmlTool.GetInstance.Doc.Save(configSaveFileDialog.FileName);
#endif
                InformationText.Text = @"Configuration is written Successfully:";
                InformationText.AppendText("\n" + configSaveFileDialog.FileName);
#if DEBUG
                CoreFullname = @"E:\ZMC\Code\C_program\MUL\x64\Release\Core.exe ";
                string command = CoreFullname + ConfigurationFullName;
                BatDir = @"E:\ZMC\Code\C_program\MUL\x64\Release\Run.bat";
                Generate_BatFile(command);
                RunButton.Enabled = true;
#else
                RunButton.Enabled = File.Exists(CoreFullname);
                if(!RunButton.Enabled) InformationText.AppendText("\n\nError:The C++ tools Core.exe can't be found!\n");
#endif
                if (FileButton.BackColor == Color.Green && BFButton.BackColor == Color.Green &&
                    FRButton.BackColor == Color.Green &&RequestButton.BackColor == Color.Green )
                    PostButton.Enabled = true;
            }
        }

        private void ValidateButton_Click(object sender, EventArgs e)
        {
#if HXJ
            if (!File.Exists(FormFile.Files.MeshFile))
#else
            RunButton.Enabled=false;
            if(!File.Exists(FormFile.MeshCard.FirstChild.InnerText))
#endif
            {
                InformationText.Text = @"Mesh File is not exist!";
                FileButton.BackColor = Color.Red;
                return;
            }          

#if HXJ
            if (FormMethod.Impedance.FillingStrategy == 0)FormSolution.Sol.PreConditionType = 0;
            BFButton.BackColor = FormBasicFunction.BasicFunction.BasicFunctionType >-1&&
                                 File.Exists(FormBasicFunction.BasicFunction.BasicFunctionName) ?
                                  Color.Green: Color.Yellow ;
#else
            if (FormMethod.MethodCard.FirstChild.InnerText != "1")
                FormSolution.SolutionMod.FirstChild.ChildNodes[4].InnerText = "0";
            //BFButton.BackColor = int.Parse(FormBasicFunction.OSCard.FirstChild.InnerText) > -1 &&
            //                     File.Exists(FormBasicFunction.OSCard.LastChild.InnerText) ?
            //    Color.Green : Color.Yellow;
            
#endif
            
            if (XmlTool.GetInstance.Validate())
            {
                InformationText.Text = XmlTool.GetInstance.Error;
                SaveButton.Enabled = false;
                ValidateButton.BackColor = Color.Red;
            }
            else
            {
                InformationText.Text = @"Configuration File is Corrected!";
                SaveButton.Enabled = true;
                ValidateButton.BackColor = Color.MistyRose;
            }
         
        }

        private void LoadButton_Click(object sender, EventArgs e)
        {
#if HXJ
            OpenFileDialog configFileDialog = new OpenFileDialog { Filter = @"小雪球 |*.hxj" }; //定义新的文件打开位置控件
#else
            OpenFileDialog configFileDialog = new OpenFileDialog { Filter = @"配置文件 |*.xml" }; //定义新的文件打开位置控件
#endif
            if (configFileDialog.ShowDialog() == DialogResult.OK)//如果有选择打开的文件
            {
                InformationText.Clear();
                InformationText.AppendText("Read Configuration File:\n" + configFileDialog.FileName + "\n");
#if HXJ
                StreamReader sd = new StreamReader(configFileDialog.FileName);
                
                FormRequest.FarField.Clear();

                Card hxj = Card.EN;
                do
                {
                    var buffer = sd.ReadLine()?.Split();
                    if (buffer != null)
                    {
                        hxj = (Card)int.Parse(buffer[0]);
                        switch (hxj)
                        {
                            case Card.IN:
                                FormFile.Files.Read(buffer);
                                break;
                            case Card.A0:
                                FormExcitation.Planewave.Read(buffer);
                                break;
                            case Card.CG:
                                FormSolution.Sol.Read(buffer);
                                break;
                            case Card.FF:                               
                                FormRequest.FarField.Add(buffer[8],new Cff(buffer));
                                break;
                            case Card.FR:
                                FormFrequency.fre.Read(buffer);
                                break;
                            case Card.AM:
                                FormMethod.Impedance.Read(buffer);
                                break;
                            case Card.OS:
                                FormBasicFunction.BasicFunction.Read(buffer);
                                break;
                            case Card.EN: break;
                        }
                    }
                } while (hxj != Card.EN);
                sd.Close();
#else    

                if (XmlTool.GetInstance.Load(configFileDialog.FileName))
                {
                    InformationText.Text = XmlTool.GetInstance.Error;
                    LoadButton.BackColor = Color.Red;
                    return;
                }
                //FormRequest.FarField.Clear();

                InformationText.Text = @"Load Configuration Successfully!";                    
                LoadButton.BackColor = Color.MistyRose;
               
#endif
                HxjLocation.Text =configFileDialog.FileName;
            }

        }

        //Insert in the dictionary if not exist

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
                if (File.Exists(BatDir)) File.Delete(BatDir);
            }
            ResultButton.Enabled = true;
        }

        private void PostButton_Click(object sender, EventArgs e)
        {
            PostButton.Enabled = false;
            using (Process p = new Process())
            {
                string command = CoreFullname + " " + ConfigurationFullName+" P";
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
                if (File.Exists(BatDir)) File.Delete(BatDir);
            }
        }
#endregion

#region CardAPI
        //Item API
        private void Mesh_Click(object sender, EventArgs e)
        {
            Form mesh = new FormFile();
            mesh.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Mesh";
            FileButton.BackColor = Color.MistyRose;
#if HXJ
            FormBasicFunction.BasicFunction.BasicFunctionType = -1;
#else
            FormBasicFunction.OSCard.FirstChild.InnerText = "-1";
#endif
            BFButton.BackColor = Color.MistyRose;
        }

        private void ImpedanceButton_Click(object sender, EventArgs e)
        {
            Form imp = new FormMethod();
            imp.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Impedance";
            ImpedanceButton.BackColor = Color.MistyRose;
        }

        private void FRButton_Click(object sender, EventArgs e)
        {
            Form fre = new FormFrequency();
            fre.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Parameter";
            FRButton.BackColor = Color.MistyRose;
        }

        private void ExcitationButton_Click(object sender, EventArgs e)
        {
            Form excitation = new FormExcitation();
            excitation.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Excitation";
            ExcitationButton.BackColor = Color.MistyRose;
        }

        private void SolutionButton_Click(object sender, EventArgs e)
        {
            Form sol = new FormSolution();
            sol.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Solution";
            SolutionButton.BackColor = Color.MistyRose;
        }

        private void RequestButton_Click(object sender, EventArgs e)
        {
            Form re = new FormRequest();
            re.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Request";
            RequestButton.BackColor = Color.MistyRose;
        }

        private void BFButton_Click(object sender, EventArgs e)
        {
            Form bf =new FormBasicFunction();
            bf.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click BasicFunction";
            BFButton.BackColor = Color.MistyRose;
        }

        private void TestButton_Click(object sender, EventArgs e)
        {
            if (File.Exists(ConfigurationFullName))
                Process.Start("D:/Program Files/Softwares/UltraEdit/uedit64.exe", ConfigurationFullName);
            else InformationText.Text = @"Not exist file:\n" + ConfigurationFullName;

        }
#endregion

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
