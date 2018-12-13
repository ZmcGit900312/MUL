namespace Configuration
{
    partial class FormFile
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
            this.MeshFileGroupBox = new System.Windows.Forms.GroupBox();
            this.MeshFileText = new System.Windows.Forms.TextBox();
            this.GmshRadioButton = new System.Windows.Forms.RadioButton();
            this.NasRadioButton = new System.Windows.Forms.RadioButton();
            this.AddMeshButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.CloseButton = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.MeshFileGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // MeshFileGroupBox
            // 
            this.MeshFileGroupBox.Controls.Add(this.MeshFileText);
            this.MeshFileGroupBox.Controls.Add(this.GmshRadioButton);
            this.MeshFileGroupBox.Controls.Add(this.NasRadioButton);
            this.MeshFileGroupBox.Controls.Add(this.AddMeshButton);
            this.MeshFileGroupBox.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MeshFileGroupBox.Location = new System.Drawing.Point(12, 12);
            this.MeshFileGroupBox.Name = "MeshFileGroupBox";
            this.MeshFileGroupBox.Size = new System.Drawing.Size(560, 100);
            this.MeshFileGroupBox.TabIndex = 32;
            this.MeshFileGroupBox.TabStop = false;
            this.MeshFileGroupBox.Text = "MeshFile";
            // 
            // MeshFileText
            // 
            this.MeshFileText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MeshFileText.Location = new System.Drawing.Point(6, 57);
            this.MeshFileText.MaxLength = 200;
            this.MeshFileText.Name = "MeshFileText";
            this.MeshFileText.ReadOnly = true;
            this.MeshFileText.Size = new System.Drawing.Size(450, 27);
            this.MeshFileText.TabIndex = 43;
            // 
            // GmshRadioButton
            // 
            this.GmshRadioButton.AutoSize = true;
            this.GmshRadioButton.Enabled = false;
            this.GmshRadioButton.Location = new System.Drawing.Point(103, 27);
            this.GmshRadioButton.Name = "GmshRadioButton";
            this.GmshRadioButton.Size = new System.Drawing.Size(77, 24);
            this.GmshRadioButton.TabIndex = 1;
            this.GmshRadioButton.TabStop = true;
            this.GmshRadioButton.Text = ".gmsh";
            this.GmshRadioButton.UseVisualStyleBackColor = true;
            // 
            // NasRadioButton
            // 
            this.NasRadioButton.AutoSize = true;
            this.NasRadioButton.Checked = true;
            this.NasRadioButton.Location = new System.Drawing.Point(7, 27);
            this.NasRadioButton.Name = "NasRadioButton";
            this.NasRadioButton.Size = new System.Drawing.Size(62, 24);
            this.NasRadioButton.TabIndex = 0;
            this.NasRadioButton.TabStop = true;
            this.NasRadioButton.Text = ".nas";
            this.NasRadioButton.UseVisualStyleBackColor = true;
            // 
            // AddMeshButton
            // 
            this.AddMeshButton.BackColor = System.Drawing.Color.MistyRose;
            this.AddMeshButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AddMeshButton.Location = new System.Drawing.Point(460, 51);
            this.AddMeshButton.Name = "AddMeshButton";
            this.AddMeshButton.Size = new System.Drawing.Size(94, 39);
            this.AddMeshButton.TabIndex = 33;
            this.AddMeshButton.Text = "Add";
            this.AddMeshButton.UseVisualStyleBackColor = false;
            this.AddMeshButton.Click += new System.EventHandler(this.AddMeshButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(12, 118);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 36;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(478, 118);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 37;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = false;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            // 
            // FormFile
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 161);
            this.ControlBox = false;
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.MeshFileGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormFile";
            this.Text = "ファイル";
            this.MeshFileGroupBox.ResumeLayout(false);
            this.MeshFileGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox MeshFileGroupBox;
        private System.Windows.Forms.RadioButton GmshRadioButton;
        private System.Windows.Forms.RadioButton NasRadioButton;
        private System.Windows.Forms.Button AddMeshButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.TextBox MeshFileText;
    }
}