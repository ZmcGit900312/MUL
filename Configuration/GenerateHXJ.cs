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
            SaveFileDialog configSaveFileDialog = new SaveFileDialog { Filter = @"配置文件 |*.xml" };//定义文件保存位置
            if (configSaveFileDialog.ShowDialog() == DialogResult.OK)//如果有保存路径
            {
                SaveButton.Enabled = false;
                ConfigurationFullName = configSaveFileDialog.FileName;

                FormFile.Project.ChildNodes[0].InnerText = Path.GetFileNameWithoutExtension(configSaveFileDialog.FileName);
                FormFile.Project.ChildNodes[1].InnerText = Path.GetDirectoryName(configSaveFileDialog.FileName);
                FormFile.Project.ChildNodes[2].InnerText =
                    FormFile.Project.ChildNodes[1].InnerText + '\\' + FormFile.Project.FirstChild.InnerText + ".out";
                XmlTool.GetInstance.Root.Attributes["Date"].InnerText = DateTime.Now.ToString("s");

                XmlTool.GetInstance.Doc.Save(configSaveFileDialog.FileName);

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

            RunButton.Enabled=false;
            InformationText.Clear();
            if (!File.Exists(FormFile.MeshCard.FirstChild.InnerText))

            {
                InformationText.Text = @"Mesh File is not exist!";
                FileButton.BackColor = Color.Red;
                return;
            }          


            if (FormMethod.MethodCard.FirstChild.InnerText != "1")
                FormSolution.SolutionMod.FirstChild.ChildNodes[4].InnerText = "0";
            //BFButton.BackColor = int.Parse(FormBasicFunction.OSCard.FirstChild.InnerText) > -1 &&
            //                     File.Exists(FormBasicFunction.OSCard.LastChild.InnerText) ?
            //    Color.Green : Color.Yellow;
            
            
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

            OpenFileDialog configFileDialog = new OpenFileDialog { Filter = @"配置文件 |*.xml" }; //定义新的文件打开位置控件
            if (configFileDialog.ShowDialog() == DialogResult.OK)//如果有选择打开的文件
            {
                InformationText.Clear();
                InformationText.AppendText("Read Configuration File:\n" + configFileDialog.FileName + "\n");
 

                if (XmlTool.GetInstance.Load(configFileDialog.FileName))
                {
                    InformationText.Text = XmlTool.GetInstance.Error;
                    LoadButton.BackColor = Color.Red;
                    return;
                }
                //FormRequest.FarField.Clear();

                InformationText.Text = @"Load Configuration Successfully!";                    
                LoadButton.BackColor = Color.MistyRose;
               
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

            FormBasicFunction.OSCard.FirstChild.InnerText = "-1";
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
