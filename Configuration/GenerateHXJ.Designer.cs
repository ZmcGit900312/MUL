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
            this.HxjLocation = new System.Windows.Forms.ToolStripStatusLabel();
            this.FileButton = new System.Windows.Forms.Button();
            this.RunButton = new System.Windows.Forms.Button();
            this.ImpedanceButton = new System.Windows.Forms.Button();
            this.FRButton = new System.Windows.Forms.Button();
            this.ExcitationButton = new System.Windows.Forms.Button();
            this.SolutionButton = new System.Windows.Forms.Button();
            this.RequestButton = new System.Windows.Forms.Button();
            this.PostButton = new System.Windows.Forms.Button();
            this.ResultButton = new System.Windows.Forms.Button();
            this.BFButton = new System.Windows.Forms.Button();
            this.CardGroup = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.CommandGroup = new System.Windows.Forms.GroupBox();
            this.statusStrip.SuspendLayout();
            this.CardGroup.SuspendLayout();
            this.CommandGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // InformationText
            // 
            this.InformationText.BackColor = System.Drawing.SystemColors.Info;
            this.InformationText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.InformationText.Location = new System.Drawing.Point(12, 179);
            this.InformationText.Margin = new System.Windows.Forms.Padding(4);
            this.InformationText.Name = "InformationText";
            this.InformationText.ReadOnly = true;
            this.InformationText.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
            this.InformationText.Size = new System.Drawing.Size(560, 144);
            this.InformationText.TabIndex = 14;
            this.InformationText.Text = "";
            // 
            // ExitButton
            // 
            this.ExitButton.BackColor = System.Drawing.Color.MistyRose;
            this.ExitButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ExitButton.Location = new System.Drawing.Point(281, 85);
            this.ExitButton.Margin = new System.Windows.Forms.Padding(4);
            this.ExitButton.Name = "ExitButton";
            this.ExitButton.Size = new System.Drawing.Size(125, 50);
            this.ExitButton.TabIndex = 16;
            this.ExitButton.Text = "終わり";
            this.ExitButton.UseVisualStyleBackColor = false;
            this.ExitButton.Click += new System.EventHandler(this.ExitButton_Click);
            // 
            // ValidateButton
            // 
            this.ValidateButton.BackColor = System.Drawing.Color.MistyRose;
            this.ValidateButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ValidateButton.Location = new System.Drawing.Point(281, 27);
            this.ValidateButton.Margin = new System.Windows.Forms.Padding(4);
            this.ValidateButton.Name = "ValidateButton";
            this.ValidateButton.Size = new System.Drawing.Size(125, 50);
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
            this.SaveButton.Location = new System.Drawing.Point(19, 85);
            this.SaveButton.Margin = new System.Windows.Forms.Padding(4);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(125, 50);
            this.SaveButton.TabIndex = 24;
            this.SaveButton.Text = "セーブ";
            this.SaveButton.UseVisualStyleBackColor = false;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // LoadButton
            // 
            this.LoadButton.BackColor = System.Drawing.Color.MistyRose;
            this.LoadButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LoadButton.Location = new System.Drawing.Point(19, 27);
            this.LoadButton.Margin = new System.Windows.Forms.Padding(4);
            this.LoadButton.Name = "LoadButton";
            this.LoadButton.Size = new System.Drawing.Size(125, 50);
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
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.HxjLocation});
            this.statusStrip.Location = new System.Drawing.Point(0, 507);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Padding = new System.Windows.Forms.Padding(2, 0, 18, 0);
            this.statusStrip.Size = new System.Drawing.Size(580, 22);
            this.statusStrip.TabIndex = 28;
            this.statusStrip.Text = "statusStrip1";
            // 
            // HxjLocation
            // 
            this.HxjLocation.Name = "HxjLocation";
            this.HxjLocation.Size = new System.Drawing.Size(19, 17);
            this.HxjLocation.Text = "$ ";
            // 
            // FileButton
            // 
            this.FileButton.BackColor = System.Drawing.Color.MistyRose;
            this.FileButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FileButton.Location = new System.Drawing.Point(19, 32);
            this.FileButton.Name = "FileButton";
            this.FileButton.Size = new System.Drawing.Size(125, 50);
            this.FileButton.TabIndex = 32;
            this.FileButton.Text = "ファイル";
            this.FileButton.UseVisualStyleBackColor = false;
            this.FileButton.Click += new System.EventHandler(this.Mesh_Click);
            // 
            // RunButton
            // 
            this.RunButton.BackColor = System.Drawing.Color.MistyRose;
            this.RunButton.Enabled = false;
            this.RunButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RunButton.ForeColor = System.Drawing.SystemColors.ControlText;
            this.RunButton.Location = new System.Drawing.Point(152, 27);
            this.RunButton.Margin = new System.Windows.Forms.Padding(4);
            this.RunButton.Name = "RunButton";
            this.RunButton.Size = new System.Drawing.Size(121, 108);
            this.RunButton.TabIndex = 31;
            this.RunButton.Text = "スタート";
            this.RunButton.UseVisualStyleBackColor = false;
            this.RunButton.Click += new System.EventHandler(this.RunButton_Click);
            // 
            // ImpedanceButton
            // 
            this.ImpedanceButton.BackColor = System.Drawing.Color.MistyRose;
            this.ImpedanceButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.ImpedanceButton.Location = new System.Drawing.Point(281, 32);
            this.ImpedanceButton.Name = "ImpedanceButton";
            this.ImpedanceButton.Size = new System.Drawing.Size(125, 50);
            this.ImpedanceButton.TabIndex = 33;
            this.ImpedanceButton.Text = "インピーダンス";
            this.ImpedanceButton.UseVisualStyleBackColor = false;
            this.ImpedanceButton.Click += new System.EventHandler(this.ImpedanceButton_Click);
            // 
            // FRButton
            // 
            this.FRButton.BackColor = System.Drawing.Color.MistyRose;
            this.FRButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FRButton.Location = new System.Drawing.Point(281, 92);
            this.FRButton.Name = "FRButton";
            this.FRButton.Size = new System.Drawing.Size(125, 50);
            this.FRButton.TabIndex = 34;
            this.FRButton.Text = "周波数";
            this.FRButton.UseVisualStyleBackColor = false;
            this.FRButton.Click += new System.EventHandler(this.FRButton_Click);
            // 
            // ExcitationButton
            // 
            this.ExcitationButton.BackColor = System.Drawing.Color.MistyRose;
            this.ExcitationButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ExcitationButton.Location = new System.Drawing.Point(412, 32);
            this.ExcitationButton.Name = "ExcitationButton";
            this.ExcitationButton.Size = new System.Drawing.Size(125, 50);
            this.ExcitationButton.TabIndex = 35;
            this.ExcitationButton.Text = "励起";
            this.ExcitationButton.UseVisualStyleBackColor = false;
            this.ExcitationButton.Click += new System.EventHandler(this.ExcitationButton_Click);
            // 
            // SolutionButton
            // 
            this.SolutionButton.BackColor = System.Drawing.Color.MistyRose;
            this.SolutionButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SolutionButton.Location = new System.Drawing.Point(19, 92);
            this.SolutionButton.Name = "SolutionButton";
            this.SolutionButton.Size = new System.Drawing.Size(125, 50);
            this.SolutionButton.TabIndex = 36;
            this.SolutionButton.Text = "解決";
            this.SolutionButton.UseVisualStyleBackColor = false;
            this.SolutionButton.Click += new System.EventHandler(this.SolutionButton_Click);
            // 
            // RequestButton
            // 
            this.RequestButton.BackColor = System.Drawing.Color.MistyRose;
            this.RequestButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RequestButton.Location = new System.Drawing.Point(412, 92);
            this.RequestButton.Name = "RequestButton";
            this.RequestButton.Size = new System.Drawing.Size(125, 50);
            this.RequestButton.TabIndex = 37;
            this.RequestButton.Text = "リクエスト";
            this.RequestButton.UseVisualStyleBackColor = false;
            this.RequestButton.Click += new System.EventHandler(this.RequestButton_Click);
            // 
            // PostButton
            // 
            this.PostButton.BackColor = System.Drawing.Color.MistyRose;
            this.PostButton.Enabled = false;
            this.PostButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PostButton.Location = new System.Drawing.Point(412, 27);
            this.PostButton.Name = "PostButton";
            this.PostButton.Size = new System.Drawing.Size(125, 50);
            this.PostButton.TabIndex = 38;
            this.PostButton.Text = "後処理";
            this.PostButton.UseVisualStyleBackColor = false;
            this.PostButton.Click += new System.EventHandler(this.PostButton_Click);
            // 
            // ResultButton
            // 
            this.ResultButton.BackColor = System.Drawing.Color.MistyRose;
            this.ResultButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ResultButton.Location = new System.Drawing.Point(412, 85);
            this.ResultButton.Name = "ResultButton";
            this.ResultButton.Size = new System.Drawing.Size(125, 50);
            this.ResultButton.TabIndex = 39;
            this.ResultButton.Text = "結果";
            this.ResultButton.UseVisualStyleBackColor = false;
            this.ResultButton.Click += new System.EventHandler(this.ResultButton_Click);
            // 
            // BFButton
            // 
            this.BFButton.BackColor = System.Drawing.Color.MistyRose;
            this.BFButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.BFButton.Location = new System.Drawing.Point(150, 32);
            this.BFButton.Name = "BFButton";
            this.BFButton.Size = new System.Drawing.Size(125, 50);
            this.BFButton.TabIndex = 40;
            this.BFButton.Text = "関数";
            this.BFButton.UseVisualStyleBackColor = false;
            this.BFButton.Click += new System.EventHandler(this.BFButton_Click);
            // 
            // CardGroup
            // 
            this.CardGroup.Controls.Add(this.button1);
            this.CardGroup.Controls.Add(this.BFButton);
            this.CardGroup.Controls.Add(this.RequestButton);
            this.CardGroup.Controls.Add(this.SolutionButton);
            this.CardGroup.Controls.Add(this.ExcitationButton);
            this.CardGroup.Controls.Add(this.FRButton);
            this.CardGroup.Controls.Add(this.ImpedanceButton);
            this.CardGroup.Controls.Add(this.FileButton);
            this.CardGroup.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.CardGroup.Location = new System.Drawing.Point(12, 12);
            this.CardGroup.Name = "CardGroup";
            this.CardGroup.Size = new System.Drawing.Size(560, 160);
            this.CardGroup.TabIndex = 41;
            this.CardGroup.TabStop = false;
            this.CardGroup.Text = "カード";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.MistyRose;
            this.button1.Enabled = false;
            this.button1.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.button1.Location = new System.Drawing.Point(150, 92);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(125, 50);
            this.button1.TabIndex = 41;
            this.button1.Text = "NotUse";
            this.button1.UseVisualStyleBackColor = false;
            // 
            // CommandGroup
            // 
            this.CommandGroup.Controls.Add(this.ResultButton);
            this.CommandGroup.Controls.Add(this.PostButton);
            this.CommandGroup.Controls.Add(this.RunButton);
            this.CommandGroup.Controls.Add(this.LoadButton);
            this.CommandGroup.Controls.Add(this.SaveButton);
            this.CommandGroup.Controls.Add(this.ValidateButton);
            this.CommandGroup.Controls.Add(this.ExitButton);
            this.CommandGroup.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Bold);
            this.CommandGroup.Location = new System.Drawing.Point(12, 342);
            this.CommandGroup.Name = "CommandGroup";
            this.CommandGroup.Size = new System.Drawing.Size(560, 160);
            this.CommandGroup.TabIndex = 42;
            this.CommandGroup.TabStop = false;
            this.CommandGroup.Text = "コマンド";
            // 
            // Start
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(580, 529);
            this.ControlBox = false;
            this.Controls.Add(this.CommandGroup);
            this.Controls.Add(this.CardGroup);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.InformationText);
            this.Font = new System.Drawing.Font("MS Reference Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Start";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CEM設定ツール";
            this.Load += new System.EventHandler(this.Start_Load);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.CardGroup.ResumeLayout(false);
            this.CommandGroup.ResumeLayout(false);
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
        private System.Windows.Forms.Button FileButton;
        private System.Windows.Forms.Button ImpedanceButton;
        private System.Windows.Forms.Button FRButton;
        private System.Windows.Forms.Button ExcitationButton;
        private System.Windows.Forms.Button SolutionButton;
        private System.Windows.Forms.Button RequestButton;
        private System.Windows.Forms.Button PostButton;
        private System.Windows.Forms.Button ResultButton;
        private System.Windows.Forms.Button BFButton;
        private System.Windows.Forms.ToolStripStatusLabel HxjLocation;
        private System.Windows.Forms.GroupBox CardGroup;
        private System.Windows.Forms.GroupBox CommandGroup;
        private System.Windows.Forms.Button button1;
    }
}

