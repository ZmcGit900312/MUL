namespace Configuration
{
    partial class FormBasicFunction
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.CloseButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.AddButton = new System.Windows.Forms.Button();
            this.BFFileText = new System.Windows.Forms.TextBox();
            this.BasicFunctionGroupBox = new System.Windows.Forms.GroupBox();
            this.RWGRadioButton = new System.Windows.Forms.RadioButton();
            this.BasicFunctionCheck = new System.Windows.Forms.CheckBox();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.BasicFunctionGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(478, 180);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 41;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = false;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(12, 180);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 40;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // AddButton
            // 
            this.AddButton.BackColor = System.Drawing.Color.MistyRose;
            this.AddButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AddButton.Location = new System.Drawing.Point(462, 53);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(94, 39);
            this.AddButton.TabIndex = 39;
            this.AddButton.Text = "Add";
            this.AddButton.UseVisualStyleBackColor = false;
            this.AddButton.Click += new System.EventHandler(this.AddButton_Click);
            // 
            // BFFileText
            // 
            this.BFFileText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.BFFileText.Location = new System.Drawing.Point(6, 65);
            this.BFFileText.MaxLength = 200;
            this.BFFileText.Name = "BFFileText";
            this.BFFileText.ReadOnly = true;
            this.BFFileText.Size = new System.Drawing.Size(450, 27);
            this.BFFileText.TabIndex = 42;
            // 
            // BasicFunctionGroupBox
            // 
            this.BasicFunctionGroupBox.Controls.Add(this.RWGRadioButton);
            this.BasicFunctionGroupBox.Controls.Add(this.BFFileText);
            this.BasicFunctionGroupBox.Controls.Add(this.AddButton);
            this.BasicFunctionGroupBox.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.BasicFunctionGroupBox.Location = new System.Drawing.Point(12, 67);
            this.BasicFunctionGroupBox.Name = "BasicFunctionGroupBox";
            this.BasicFunctionGroupBox.Size = new System.Drawing.Size(560, 100);
            this.BasicFunctionGroupBox.TabIndex = 43;
            this.BasicFunctionGroupBox.TabStop = false;
            this.BasicFunctionGroupBox.Text = "BasicFunction";
            // 
            // RWGRadioButton
            // 
            this.RWGRadioButton.AutoSize = true;
            this.RWGRadioButton.Checked = true;
            this.RWGRadioButton.Enabled = false;
            this.RWGRadioButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.RWGRadioButton.Location = new System.Drawing.Point(6, 26);
            this.RWGRadioButton.Name = "RWGRadioButton";
            this.RWGRadioButton.Size = new System.Drawing.Size(67, 24);
            this.RWGRadioButton.TabIndex = 43;
            this.RWGRadioButton.TabStop = true;
            this.RWGRadioButton.Text = "RWG";
            this.RWGRadioButton.UseVisualStyleBackColor = true;
            // 
            // BasicFunctionCheck
            // 
            this.BasicFunctionCheck.AutoSize = true;
            this.BasicFunctionCheck.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.BasicFunctionCheck.Location = new System.Drawing.Point(12, 37);
            this.BasicFunctionCheck.Name = "BasicFunctionCheck";
            this.BasicFunctionCheck.Size = new System.Drawing.Size(190, 24);
            this.BasicFunctionCheck.TabIndex = 44;
            this.BasicFunctionCheck.Text = "ActiveBasicFunction";
            this.BasicFunctionCheck.UseVisualStyleBackColor = true;
            this.BasicFunctionCheck.CheckedChanged += new System.EventHandler(this.BasicFunctionCheck_CheckedChanged);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            // 
            // FormBasicFunction
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 231);
            this.ControlBox = false;
            this.Controls.Add(this.BasicFunctionCheck);
            this.Controls.Add(this.BasicFunctionGroupBox);
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormBasicFunction";
            this.Text = "関数";
            this.BasicFunctionGroupBox.ResumeLayout(false);
            this.BasicFunctionGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.TextBox BFFileText;
        private System.Windows.Forms.GroupBox BasicFunctionGroupBox;
        private System.Windows.Forms.RadioButton RWGRadioButton;
        private System.Windows.Forms.CheckBox BasicFunctionCheck;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}