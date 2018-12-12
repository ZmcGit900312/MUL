namespace Configuration
{
    partial class Start
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.InformationText = new System.Windows.Forms.RichTextBox();
            this.ExitButton = new System.Windows.Forms.Button();
            this.ValidateButton = new System.Windows.Forms.Button();
            this.SaveButton = new System.Windows.Forms.Button();
            this.LoadButton = new System.Windows.Forms.Button();
            this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.MeshButton = new System.Windows.Forms.Button();
            this.RunButton = new System.Windows.Forms.Button();
            this.ImpedanceButton = new System.Windows.Forms.Button();
            this.ParaButton = new System.Windows.Forms.Button();
            this.ExcitationButton = new System.Windows.Forms.Button();
            this.SolutionButton = new System.Windows.Forms.Button();
            this.RequestButton = new System.Windows.Forms.Button();
            this.PostButton = new System.Windows.Forms.Button();
            this.ResultButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // InformationText
            // 
            this.InformationText.BackColor = System.Drawing.SystemColors.Info;
            this.InformationText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.InformationText.Location = new System.Drawing.Point(15, 375);
            this.InformationText.Margin = new System.Windows.Forms.Padding(4);
            this.InformationText.Name = "InformationText";
            this.InformationText.ReadOnly = true;
            this.InformationText.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.InformationText.Size = new System.Drawing.Size(538, 209);
            this.InformationText.TabIndex = 14;
            this.InformationText.Text = "";
            // 
            // ExitButton
            // 
            this.ExitButton.BackColor = System.Drawing.Color.MistyRose;
            this.ExitButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ExitButton.Location = new System.Drawing.Point(263, 656);
            this.ExitButton.Margin = new System.Windows.Forms.Padding(4);
            this.ExitButton.Name = "ExitButton";
            this.ExitButton.Size = new System.Drawing.Size(122, 46);
            this.ExitButton.TabIndex = 16;
            this.ExitButton.Text = "終わり";
            this.ExitButton.UseVisualStyleBackColor = false;
            this.ExitButton.Click += new System.EventHandler(this.ExitButton_Click);
            // 
            // ValidateButton
            // 
            this.ValidateButton.BackColor = System.Drawing.Color.MistyRose;
            this.ValidateButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ValidateButton.Location = new System.Drawing.Point(263, 592);
            this.ValidateButton.Margin = new System.Windows.Forms.Padding(4);
            this.ValidateButton.Name = "ValidateButton";
            this.ValidateButton.Size = new System.Drawing.Size(122, 46);
            this.ValidateButton.TabIndex = 22;
            this.ValidateButton.Text = "検証";
            this.ValidateButton.UseVisualStyleBackColor = false;
            this.ValidateButton.Click += new System.EventHandler(this.ValidateButton_Click);
            // 
            // SaveButton
            // 
            this.SaveButton.BackColor = System.Drawing.Color.MistyRose;
            this.SaveButton.Enabled = false;
            this.SaveButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SaveButton.Location = new System.Drawing.Point(15, 656);
            this.SaveButton.Margin = new System.Windows.Forms.Padding(4);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(122, 46);
            this.SaveButton.TabIndex = 24;
            this.SaveButton.Text = "セーブ";
            this.SaveButton.UseVisualStyleBackColor = false;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // LoadButton
            // 
            this.LoadButton.BackColor = System.Drawing.Color.MistyRose;
            this.LoadButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LoadButton.Location = new System.Drawing.Point(15, 592);
            this.LoadButton.Margin = new System.Windows.Forms.Padding(4);
            this.LoadButton.Name = "LoadButton";
            this.LoadButton.Size = new System.Drawing.Size(122, 46);
            this.LoadButton.TabIndex = 25;
            this.LoadButton.Text = "ロード";
            this.LoadButton.UseVisualStyleBackColor = false;
            this.LoadButton.Click += new System.EventHandler(this.LoadButton_Click);
            // 
            // OpenFileDialog
            // 
            this.OpenFileDialog.FileName = "openFileDialog";
            // 
            // statusStrip
            // 
            this.statusStrip.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F);
            this.statusStrip.Location = new System.Drawing.Point(0, 725);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Padding = new System.Windows.Forms.Padding(2, 0, 18, 0);
            this.statusStrip.Size = new System.Drawing.Size(571, 22);
            this.statusStrip.TabIndex = 28;
            this.statusStrip.Text = "statusStrip1";
            // 
            // MeshButton
            // 
            this.MeshButton.BackColor = System.Drawing.Color.MistyRose;
            this.MeshButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MeshButton.Location = new System.Drawing.Point(15, 258);
            this.MeshButton.Name = "MeshButton";
            this.MeshButton.Size = new System.Drawing.Size(125, 50);
            this.MeshButton.TabIndex = 32;
            this.MeshButton.Text = "Mesh";
            this.MeshButton.UseVisualStyleBackColor = false;
            this.MeshButton.Click += new System.EventHandler(this.Mesh_Click);
            // 
            // RunButton
            // 
            this.RunButton.BackColor = System.Drawing.Color.MistyRose;
            this.RunButton.Enabled = false;
            this.RunButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RunButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.RunButton.Location = new System.Drawing.Point(145, 592);
            this.RunButton.Margin = new System.Windows.Forms.Padding(4);
            this.RunButton.Name = "RunButton";
            this.RunButton.Size = new System.Drawing.Size(110, 110);
            this.RunButton.TabIndex = 31;
            this.RunButton.Text = "スタート";
            this.RunButton.UseVisualStyleBackColor = false;
            this.RunButton.Click += new System.EventHandler(this.RunButton_Click);
            // 
            // ImpedanceButton
            // 
            this.ImpedanceButton.BackColor = System.Drawing.Color.MistyRose;
            this.ImpedanceButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.ImpedanceButton.Location = new System.Drawing.Point(146, 258);
            this.ImpedanceButton.Name = "ImpedanceButton";
            this.ImpedanceButton.Size = new System.Drawing.Size(125, 50);
            this.ImpedanceButton.TabIndex = 33;
            this.ImpedanceButton.Text = "Impedance";
            this.ImpedanceButton.UseVisualStyleBackColor = false;
            this.ImpedanceButton.Click += new System.EventHandler(this.ImpedanceButton_Click);
            // 
            // ParaButton
            // 
            this.ParaButton.BackColor = System.Drawing.Color.MistyRose;
            this.ParaButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ParaButton.Location = new System.Drawing.Point(277, 258);
            this.ParaButton.Name = "ParaButton";
            this.ParaButton.Size = new System.Drawing.Size(125, 50);
            this.ParaButton.TabIndex = 34;
            this.ParaButton.Text = "Parameter";
            this.ParaButton.UseVisualStyleBackColor = false;
            this.ParaButton.Click += new System.EventHandler(this.ParaButton_Click);
            // 
            // ExcitationButton
            // 
            this.ExcitationButton.BackColor = System.Drawing.Color.MistyRose;
            this.ExcitationButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ExcitationButton.Location = new System.Drawing.Point(408, 258);
            this.ExcitationButton.Name = "ExcitationButton";
            this.ExcitationButton.Size = new System.Drawing.Size(125, 50);
            this.ExcitationButton.TabIndex = 35;
            this.ExcitationButton.Text = "Excitation";
            this.ExcitationButton.UseVisualStyleBackColor = false;
            this.ExcitationButton.Click += new System.EventHandler(this.ExcitationButton_Click);
            // 
            // SolutionButton
            // 
            this.SolutionButton.BackColor = System.Drawing.Color.MistyRose;
            this.SolutionButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SolutionButton.Location = new System.Drawing.Point(15, 318);
            this.SolutionButton.Name = "SolutionButton";
            this.SolutionButton.Size = new System.Drawing.Size(125, 50);
            this.SolutionButton.TabIndex = 36;
            this.SolutionButton.Text = "Solution";
            this.SolutionButton.UseVisualStyleBackColor = false;
            this.SolutionButton.Click += new System.EventHandler(this.SolutionButton_Click);
            // 
            // RequestButton
            // 
            this.RequestButton.BackColor = System.Drawing.Color.MistyRose;
            this.RequestButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RequestButton.Location = new System.Drawing.Point(146, 318);
            this.RequestButton.Name = "RequestButton";
            this.RequestButton.Size = new System.Drawing.Size(125, 50);
            this.RequestButton.TabIndex = 37;
            this.RequestButton.Text = "Request";
            this.RequestButton.UseVisualStyleBackColor = false;
            this.RequestButton.Click += new System.EventHandler(this.RequestButton_Click);
            // 
            // PostButton
            // 
            this.PostButton.BackColor = System.Drawing.Color.MistyRose;
            this.PostButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PostButton.Location = new System.Drawing.Point(428, 592);
            this.PostButton.Name = "PostButton";
            this.PostButton.Size = new System.Drawing.Size(125, 50);
            this.PostButton.TabIndex = 38;
            this.PostButton.Text = "Post";
            this.PostButton.UseVisualStyleBackColor = false;
            // 
            // ResultButton
            // 
            this.ResultButton.BackColor = System.Drawing.Color.MistyRose;
            this.ResultButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ResultButton.Location = new System.Drawing.Point(428, 652);
            this.ResultButton.Name = "ResultButton";
            this.ResultButton.Size = new System.Drawing.Size(125, 50);
            this.ResultButton.TabIndex = 39;
            this.ResultButton.Text = "Result";
            this.ResultButton.UseVisualStyleBackColor = false;
            // 
            // Start
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(571, 747);
            this.ControlBox = false;
            this.Controls.Add(this.ResultButton);
            this.Controls.Add(this.PostButton);
            this.Controls.Add(this.RequestButton);
            this.Controls.Add(this.SolutionButton);
            this.Controls.Add(this.ExcitationButton);
            this.Controls.Add(this.ParaButton);
            this.Controls.Add(this.ImpedanceButton);
            this.Controls.Add(this.MeshButton);
            this.Controls.Add(this.RunButton);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.LoadButton);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.InformationText);
            this.Controls.Add(this.ValidateButton);
            this.Controls.Add(this.ExitButton);
            this.Font = new System.Drawing.Font("MS Reference Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Start";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CEMConfigurationTool";
            this.Load += new System.EventHandler(this.Start_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.RichTextBox InformationText;
        private System.Windows.Forms.Button ExitButton;
        private System.Windows.Forms.Button ValidateButton;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.Button LoadButton;
        private System.Windows.Forms.OpenFileDialog OpenFileDialog;
        private System.Windows.Forms.SaveFileDialog SaveFileDialog;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.Button RunButton;
        private System.Windows.Forms.Button MeshButton;
        private System.Windows.Forms.Button ImpedanceButton;
        private System.Windows.Forms.Button ParaButton;
        private System.Windows.Forms.Button ExcitationButton;
        private System.Windows.Forms.Button SolutionButton;
        private System.Windows.Forms.Button RequestButton;
        private System.Windows.Forms.Button PostButton;
        private System.Windows.Forms.Button ResultButton;
    }
}

