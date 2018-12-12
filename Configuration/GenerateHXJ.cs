using System;
using System.Collections.Generic;
using System.Diagnostics;
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
        private void ExitButton_Click(object sender, EventArgs e)
        {
            MessageBox.Show(@"Exit The System, See You~~~");
            Application.Exit();
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog configSaveFileDialog = new SaveFileDialog {Filter = @"小雪球 |*.hxj" };//定义文件保存位置
            if (configSaveFileDialog.ShowDialog() == DialogResult.OK)//如果有保存路径
            {
                SaveButton.Enabled = false;
                ConfigurationFullName = configSaveFileDialog.FileName;

                FormMesh.CardIN.ProjectDir = Path.GetDirectoryName(configSaveFileDialog.FileName);
                FormMesh.CardIN.ProjectName = Path.GetFileNameWithoutExtension(configSaveFileDialog.FileName);
                StreamWriter sw = File.CreateText(configSaveFileDialog.FileName);

                //Write
                FormMesh.CardIN.Write(sw);
                FormMethod.CardMD.Write(sw);
                FormParameter.Para.Write(sw);
                FormExcitation.Planewave.Write(sw);
                FormSolution.Sol.Write(sw);
                //FormRequest..Write(sw);               
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
                do
                {
                    var buffer = sd.ReadLine()?.Split();
                    if (buffer != null)
                    {
                        hxj = (Card) int.Parse(buffer[0]);
                        switch (hxj)
                        {
                            case Card.IN:
                               
                                break;
                            case Card.A0:
                                
                                break;
                            case Card.CG:
                                
                                break;
                            case Card.FF:
                                
                                break;
                            case Card.FR:
                               
                                break;
                            case Card.AM:
                               
                                break;
                        }
                    }
                } while (hxj!=Card.EN);
                sd.Close();
               
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

        //Item API
        private void Mesh_Click(object sender, EventArgs e)
        {
            Form mesh = new FormMesh();
            mesh.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Mesh";

        }

        private void ImpedanceButton_Click(object sender, EventArgs e)
        {
            Form imp=new FormMethod();
            imp.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Impedance";
        }

        private void ParaButton_Click(object sender, EventArgs e)
        {
            Form para=new FormParameter();
            para.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Parameter";
        }

        private void ExcitationButton_Click(object sender, EventArgs e)
        {
            Form excitation = new FormExcitation();
            excitation.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Excitation";
        }

        private void SolutionButton_Click(object sender, EventArgs e)
        {
            Form sol = new FormSolution();
            sol.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Solution";
        }

        private void RequestButton_Click(object sender, EventArgs e)
        {
            Form re = new FormRequest();
            re.ShowDialog();
            InformationText.Clear();
            InformationText.Text = @"Click Request";
        }
    }
}
