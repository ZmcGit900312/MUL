namespace Configuration
{
    partial class FormMesh
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
            this.GmshRadioButton = new System.Windows.Forms.RadioButton();
            this.NasRadioButton = new System.Windows.Forms.RadioButton();
            this.AddMeshButton = new System.Windows.Forms.Button();
            this.MeshFileRichText = new System.Windows.Forms.RichTextBox();
            this.AddBFButton = new System.Windows.Forms.Button();
            this.BFFileRichText = new System.Windows.Forms.RichTextBox();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.CancelButton = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.MeshFileGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // MeshFileGroupBox
            // 
            this.MeshFileGroupBox.BackgroundImage = global::Configuration.Properties.Resources.Green_Music;
            this.MeshFileGroupBox.Controls.Add(this.GmshRadioButton);
            this.MeshFileGroupBox.Controls.Add(this.NasRadioButton);
            this.MeshFileGroupBox.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MeshFileGroupBox.Location = new System.Drawing.Point(12, 12);
            this.MeshFileGroupBox.Name = "MeshFileGroupBox";
            this.MeshFileGroupBox.Size = new System.Drawing.Size(410, 61);
            this.MeshFileGroupBox.TabIndex = 32;
            this.MeshFileGroupBox.TabStop = false;
            this.MeshFileGroupBox.Text = "MeshFile";
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
            this.AddMeshButton.Location = new System.Drawing.Point(12, 170);
            this.AddMeshButton.Name = "AddMeshButton";
            this.AddMeshButton.Size = new System.Drawing.Size(94, 39);
            this.AddMeshButton.TabIndex = 33;
            this.AddMeshButton.Text = "AddMesh";
            this.AddMeshButton.UseVisualStyleBackColor = false;
            this.AddMeshButton.Click += new System.EventHandler(this.AddMeshButton_Click);
            // 
            // MeshFileRichText
            // 
            this.MeshFileRichText.BackColor = System.Drawing.SystemColors.Info;
            this.MeshFileRichText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MeshFileRichText.Location = new System.Drawing.Point(12, 79);
            this.MeshFileRichText.MaxLength = 200;
            this.MeshFileRichText.Name = "MeshFileRichText";
            this.MeshFileRichText.ReadOnly = true;
            this.MeshFileRichText.Size = new System.Drawing.Size(410, 85);
            this.MeshFileRichText.TabIndex = 34;
            this.MeshFileRichText.Text = "";
            // 
            // AddBFButton
            // 
            this.AddBFButton.BackColor = System.Drawing.Color.MistyRose;
            this.AddBFButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AddBFButton.Location = new System.Drawing.Point(328, 170);
            this.AddBFButton.Name = "AddBFButton";
            this.AddBFButton.Size = new System.Drawing.Size(94, 39);
            this.AddBFButton.TabIndex = 35;
            this.AddBFButton.Text = "AddBF";
            this.AddBFButton.UseVisualStyleBackColor = false;
            this.AddBFButton.Click += new System.EventHandler(this.AddBFButton_Click);
            // 
            // BFFileRichText
            // 
            this.BFFileRichText.BackColor = System.Drawing.SystemColors.Info;
            this.BFFileRichText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.BFFileRichText.Location = new System.Drawing.Point(11, 222);
            this.BFFileRichText.MaxLength = 200;
            this.BFFileRichText.Name = "BFFileRichText";
            this.BFFileRichText.ReadOnly = true;
            this.BFFileRichText.Size = new System.Drawing.Size(411, 85);
            this.BFFileRichText.TabIndex = 34;
            this.BFFileRichText.Text = "";
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(11, 410);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 36;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // CancelButton
            // 
            this.CancelButton.BackColor = System.Drawing.Color.MistyRose;
            this.CancelButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CancelButton.Location = new System.Drawing.Point(328, 410);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(94, 39);
            this.CancelButton.TabIndex = 37;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = false;
            this.CancelButton.Click += new System.EventHandler(this.CancelButton_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            // 
            // FormMesh
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 461);
            this.ControlBox = false;
            this.Controls.Add(this.CancelButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.BFFileRichText);
            this.Controls.Add(this.AddBFButton);
            this.Controls.Add(this.MeshFileRichText);
            this.Controls.Add(this.AddMeshButton);
            this.Controls.Add(this.MeshFileGroupBox);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMesh";
            this.Text = "Mesh&BasicFunction";
            this.MeshFileGroupBox.ResumeLayout(false);
            this.MeshFileGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox MeshFileGroupBox;
        private System.Windows.Forms.RadioButton GmshRadioButton;
        private System.Windows.Forms.RadioButton NasRadioButton;
        private System.Windows.Forms.Button AddMeshButton;
        private System.Windows.Forms.RichTextBox MeshFileRichText;
        private System.Windows.Forms.Button AddBFButton;
        private System.Windows.Forms.RichTextBox BFFileRichText;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Button CancelButton;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}