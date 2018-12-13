using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

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
        }

        private void Start_Load(object sender, EventArgs e)
        {
            BatDir = _root + @"\Run.bat";
            CoreFullname = _root + Corename;
        }


        #region commandAPI
        private void ResultButton_Click(object sender, EventArgs e)
        {
            string resultFile = FormFile.Files.ProjectDir + '\\' + FormFile.Files.ProjectName + ".out";
            if (File.Exists(resultFile)) Process.Start("notepad.exe", resultFile);
            else InformationText.Text = @"Not exist file:\n" + resultFile;
        }

        private void ExitButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show(@"Exit The System, See You~~~");
            Application.Exit();
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog configSaveFileDialog = new SaveFileDialog { Filter = @"小雪球 |*.hxj" };//定义文件保存位置
            if (configSaveFileDialog.ShowDialog() == DialogResult.OK)//如果有保存路径
            {
                SaveButton.Enabled = false;
                ConfigurationFullName = configSaveFileDialog.FileName;

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
            if (!File.Exists(FormFile.Files.MeshFile))
            {
                InformationText.Text = @"Mesh File is not exist!";
                FileButton.BackColor = Color.Red;
                return;
            }
            else FileButton.BackColor = Color.Green;

            if (FormMethod.Impedance.FillingStrategy == 0)
            {
                FormSolution.Sol.PreConditionType = 0;
            }

            BFButton.BackColor = FormBasicFunction.BasicFunction.BasicFunctionType >-1&&
                                 File.Exists(FormBasicFunction.BasicFunction.BasicFunctionName) ?
                                  Color.Green: Color.Yellow ;

            InformationText.Text = @"Configuration File is Corrected!";
            
            ImpedanceButton.BackColor = Color.Green;
            FRButton.BackColor = Color.Green;
            ExcitationButton.BackColor = Color.Green;
            SolutionButton.BackColor = Color.Green;
            RequestButton.BackColor = Color.Green;
            SaveButton.Enabled = true;
            
        }

        private void LoadButton_Click(object sender, EventArgs e)
        {

            OpenFileDialog configFileDialog = new OpenFileDialog { Filter = @"小雪球 |*.hxj" }; //定义新的文件打开位置控件
            if (configFileDialog.ShowDialog() == DialogResult.OK)//如果有选择打开的文件
            {
                InformationText.Clear();
                InformationText.AppendText("Read Configuration File:\n" + configFileDialog.FileName + "\n");
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
                HxjLocation.Text =@"$ "+configFileDialog.FileName;
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
            FormBasicFunction.BasicFunction.BasicFunctionType = -1;
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
