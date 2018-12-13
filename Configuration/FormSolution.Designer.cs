namespace Configuration
{
    partial class FormSolution
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
            this.PreconditionComboBox = new System.Windows.Forms.ComboBox();
            this.IterationPanel = new System.Windows.Forms.Panel();
            this.StopMaxText = new System.Windows.Forms.TextBox();
            this.StopCriterionText = new System.Windows.Forms.TextBox();
            this.MaxIterationsText = new System.Windows.Forms.TextBox();
            this.StopMaxLable = new System.Windows.Forms.Label();
            this.StopCriterionLable = new System.Windows.Forms.Label();
            this.MaxIterationsLable = new System.Windows.Forms.Label();
            this.SolutionTypeComboBox = new System.Windows.Forms.ComboBox();
            this.CloseButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.IterationPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // PreconditionComboBox
            // 
            this.PreconditionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.PreconditionComboBox.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PreconditionComboBox.FormattingEnabled = true;
            this.PreconditionComboBox.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.PreconditionComboBox.Items.AddRange(new object[] {
            "No preconditioning (0)",
            "Block-Jacobi preconditioning using inverses (64)",
            "Incomplete LU decomposition (ILU) (128)"});
            this.PreconditionComboBox.Location = new System.Drawing.Point(12, 165);
            this.PreconditionComboBox.Name = "PreconditionComboBox";
            this.PreconditionComboBox.Size = new System.Drawing.Size(400, 28);
            this.PreconditionComboBox.TabIndex = 24;
            // 
            // IterationPanel
            // 
            this.IterationPanel.Controls.Add(this.StopMaxText);
            this.IterationPanel.Controls.Add(this.StopCriterionText);
            this.IterationPanel.Controls.Add(this.MaxIterationsText);
            this.IterationPanel.Controls.Add(this.StopMaxLable);
            this.IterationPanel.Controls.Add(this.StopCriterionLable);
            this.IterationPanel.Controls.Add(this.MaxIterationsLable);
            this.IterationPanel.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.IterationPanel.Location = new System.Drawing.Point(12, 48);
            this.IterationPanel.Name = "IterationPanel";
            this.IterationPanel.Size = new System.Drawing.Size(400, 111);
            this.IterationPanel.TabIndex = 23;
            // 
            // StopMaxText
            // 
            this.StopMaxText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.StopMaxText.Location = new System.Drawing.Point(271, 76);
            this.StopMaxText.Name = "StopMaxText";
            this.StopMaxText.Size = new System.Drawing.Size(125, 27);
            this.StopMaxText.TabIndex = 24;
            this.StopMaxText.Text = "100";
            // 
            // StopCriterionText
            // 
            this.StopCriterionText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.StopCriterionText.Location = new System.Drawing.Point(271, 43);
            this.StopCriterionText.Name = "StopCriterionText";
            this.StopCriterionText.Size = new System.Drawing.Size(125, 27);
            this.StopCriterionText.TabIndex = 23;
            this.StopCriterionText.Text = "1.0e-4";
            // 
            // MaxIterationsText
            // 
            this.MaxIterationsText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxIterationsText.Location = new System.Drawing.Point(271, 10);
            this.MaxIterationsText.Name = "MaxIterationsText";
            this.MaxIterationsText.Size = new System.Drawing.Size(125, 27);
            this.MaxIterationsText.TabIndex = 22;
            this.MaxIterationsText.Text = "0";
            // 
            // StopMaxLable
            // 
            this.StopMaxLable.AutoSize = true;
            this.StopMaxLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.StopMaxLable.Location = new System.Drawing.Point(4, 78);
            this.StopMaxLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StopMaxLable.Name = "StopMaxLable";
            this.StopMaxLable.Size = new System.Drawing.Size(225, 20);
            this.StopMaxLable.TabIndex = 21;
            this.StopMaxLable.Text = "Stop at maximum residum";
            // 
            // StopCriterionLable
            // 
            this.StopCriterionLable.AutoSize = true;
            this.StopCriterionLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.StopCriterionLable.Location = new System.Drawing.Point(4, 46);
            this.StopCriterionLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.StopCriterionLable.Name = "StopCriterionLable";
            this.StopCriterionLable.Size = new System.Drawing.Size(253, 20);
            this.StopCriterionLable.TabIndex = 20;
            this.StopCriterionLable.Text = "Stopping criterion for residum";
            // 
            // MaxIterationsLable
            // 
            this.MaxIterationsLable.AutoSize = true;
            this.MaxIterationsLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MaxIterationsLable.Location = new System.Drawing.Point(4, 13);
            this.MaxIterationsLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.MaxIterationsLable.Name = "MaxIterationsLable";
            this.MaxIterationsLable.Size = new System.Drawing.Size(260, 20);
            this.MaxIterationsLable.TabIndex = 19;
            this.MaxIterationsLable.Text = "Maximum number of iterations";
            // 
            // SolutionTypeComboBox
            // 
            this.SolutionTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.SolutionTypeComboBox.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.SolutionTypeComboBox.FormattingEnabled = true;
            this.SolutionTypeComboBox.Items.AddRange(new object[] {
            "Bi-CGSTAB",
            "LU-decomp"});
            this.SolutionTypeComboBox.Location = new System.Drawing.Point(12, 14);
            this.SolutionTypeComboBox.Name = "SolutionTypeComboBox";
            this.SolutionTypeComboBox.Size = new System.Drawing.Size(400, 28);
            this.SolutionTypeComboBox.TabIndex = 22;
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(328, 410);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 53;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = false;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(12, 410);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 52;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // FormSolution
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 461);
            this.ControlBox = false;
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.PreconditionComboBox);
            this.Controls.Add(this.IterationPanel);
            this.Controls.Add(this.SolutionTypeComboBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormSolution";
            this.Text = "解決";
            this.IterationPanel.ResumeLayout(false);
            this.IterationPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox PreconditionComboBox;
        private System.Windows.Forms.Panel IterationPanel;
        private System.Windows.Forms.TextBox StopMaxText;
        private System.Windows.Forms.TextBox StopCriterionText;
        private System.Windows.Forms.TextBox MaxIterationsText;
        private System.Windows.Forms.Label StopMaxLable;
        private System.Windows.Forms.Label StopCriterionLable;
        private System.Windows.Forms.Label MaxIterationsLable;
        private System.Windows.Forms.ComboBox SolutionTypeComboBox;
        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.Button ApplyButton;
    }
}