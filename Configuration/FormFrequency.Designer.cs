namespace Configuration
{
    partial class FormFrequency
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
            this.FrequencyLable = new System.Windows.Forms.Label();
            this.FrequecyText = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(328, 410);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 55;
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
            this.ApplyButton.TabIndex = 54;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // FrequencyLable
            // 
            this.FrequencyLable.AutoSize = true;
            this.FrequencyLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.FrequencyLable.Location = new System.Drawing.Point(8, 15);
            this.FrequencyLable.Name = "FrequencyLable";
            this.FrequencyLable.Size = new System.Drawing.Size(132, 20);
            this.FrequencyLable.TabIndex = 57;
            this.FrequencyLable.Text = "Frequency (Hz)";
            // 
            // FrequecyText
            // 
            this.FrequecyText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FrequecyText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.FrequecyText.Location = new System.Drawing.Point(167, 12);
            this.FrequecyText.Name = "FrequecyText";
            this.FrequecyText.Size = new System.Drawing.Size(255, 27);
            this.FrequecyText.TabIndex = 56;
            this.FrequecyText.Text = "3.0e8";
            // 
            // FormFrequency
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(434, 461);
            this.ControlBox = false;
            this.Controls.Add(this.FrequencyLable);
            this.Controls.Add(this.FrequecyText);
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormFrequency";
            this.Text = "周波数";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.Label FrequencyLable;
        private System.Windows.Forms.TextBox FrequecyText;
    }
}