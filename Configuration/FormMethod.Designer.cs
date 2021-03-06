﻿namespace Configuration
{
    partial class FormMethod
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
            this.AIMPanel = new System.Windows.Forms.Panel();
            this.VirtualGridTechniqueCheckBox = new System.Windows.Forms.CheckBox();
            this.Dimension3D = new System.Windows.Forms.RadioButton();
            this.Dimension2D = new System.Windows.Forms.RadioButton();
            this.DimensionLable = new System.Windows.Forms.Label();
            this.OrderCombox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.ThresholdText = new System.Windows.Forms.TextBox();
            this.AIMThresholdLable = new System.Windows.Forms.Label();
            this.NearTolerance = new System.Windows.Forms.TextBox();
            this.AIMIntervalLable = new System.Windows.Forms.Label();
            this.AIMOrderLable = new System.Windows.Forms.Label();
            this.IntervalText = new System.Windows.Forms.TextBox();
            this.FillingType = new System.Windows.Forms.CheckBox();
            this.MulRadioButton = new System.Windows.Forms.RadioButton();
            this.MoMRadioButton = new System.Windows.Forms.RadioButton();
            this.AIMRadioButton = new System.Windows.Forms.RadioButton();
            this.CloseButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.IEPanel = new System.Windows.Forms.GroupBox();
            this.ZsText = new System.Windows.Forms.TextBox();
            this.ZsLable = new System.Windows.Forms.Label();
            this.EtaText = new System.Windows.Forms.TextBox();
            this.EtaLable = new System.Windows.Forms.Label();
            this.AlphaLable = new System.Windows.Forms.Label();
            this.AlphaText = new System.Windows.Forms.TextBox();
            this.IBCCFIEButton = new System.Windows.Forms.RadioButton();
            this.IBCMFIEButton = new System.Windows.Forms.RadioButton();
            this.IBCEFIEButton = new System.Windows.Forms.RadioButton();
            this.CFIEButton = new System.Windows.Forms.RadioButton();
            this.MFIEButton = new System.Windows.Forms.RadioButton();
            this.EFIEButton = new System.Windows.Forms.RadioButton();
            this.IESelectPanel = new System.Windows.Forms.Panel();
            this.AIMPanel.SuspendLayout();
            this.IEPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // AIMPanel
            // 
            this.AIMPanel.Controls.Add(this.VirtualGridTechniqueCheckBox);
            this.AIMPanel.Controls.Add(this.Dimension3D);
            this.AIMPanel.Controls.Add(this.Dimension2D);
            this.AIMPanel.Controls.Add(this.DimensionLable);
            this.AIMPanel.Controls.Add(this.OrderCombox);
            this.AIMPanel.Controls.Add(this.label1);
            this.AIMPanel.Controls.Add(this.ThresholdText);
            this.AIMPanel.Controls.Add(this.AIMThresholdLable);
            this.AIMPanel.Controls.Add(this.NearTolerance);
            this.AIMPanel.Controls.Add(this.AIMIntervalLable);
            this.AIMPanel.Controls.Add(this.AIMOrderLable);
            this.AIMPanel.Controls.Add(this.IntervalText);
            this.AIMPanel.Enabled = false;
            this.AIMPanel.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AIMPanel.Location = new System.Drawing.Point(13, 45);
            this.AIMPanel.Margin = new System.Windows.Forms.Padding(4);
            this.AIMPanel.Name = "AIMPanel";
            this.AIMPanel.Size = new System.Drawing.Size(408, 307);
            this.AIMPanel.TabIndex = 47;
            // 
            // VirtualGridTechniqueCheckBox
            // 
            this.VirtualGridTechniqueCheckBox.AutoSize = true;
            this.VirtualGridTechniqueCheckBox.Location = new System.Drawing.Point(13, 272);
            this.VirtualGridTechniqueCheckBox.Name = "VirtualGridTechniqueCheckBox";
            this.VirtualGridTechniqueCheckBox.Size = new System.Drawing.Size(210, 24);
            this.VirtualGridTechniqueCheckBox.TabIndex = 47;
            this.VirtualGridTechniqueCheckBox.Text = "Virtual Grid Technique";
            this.VirtualGridTechniqueCheckBox.UseVisualStyleBackColor = true;
            // 
            // Dimension3D
            // 
            this.Dimension3D.AutoSize = true;
            this.Dimension3D.Location = new System.Drawing.Point(291, 225);
            this.Dimension3D.Margin = new System.Windows.Forms.Padding(4);
            this.Dimension3D.Name = "Dimension3D";
            this.Dimension3D.Size = new System.Drawing.Size(49, 24);
            this.Dimension3D.TabIndex = 46;
            this.Dimension3D.Text = "3D";
            this.Dimension3D.UseVisualStyleBackColor = true;
            // 
            // Dimension2D
            // 
            this.Dimension2D.AutoSize = true;
            this.Dimension2D.Enabled = false;
            this.Dimension2D.Location = new System.Drawing.Point(163, 225);
            this.Dimension2D.Margin = new System.Windows.Forms.Padding(4);
            this.Dimension2D.Name = "Dimension2D";
            this.Dimension2D.Size = new System.Drawing.Size(49, 24);
            this.Dimension2D.TabIndex = 45;
            this.Dimension2D.Text = "2D";
            this.Dimension2D.UseVisualStyleBackColor = true;
            // 
            // DimensionLable
            // 
            this.DimensionLable.AutoSize = true;
            this.DimensionLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.DimensionLable.Location = new System.Drawing.Point(9, 227);
            this.DimensionLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.DimensionLable.Name = "DimensionLable";
            this.DimensionLable.Size = new System.Drawing.Size(95, 20);
            this.DimensionLable.TabIndex = 44;
            this.DimensionLable.Text = "Dimension";
            // 
            // OrderCombox
            // 
            this.OrderCombox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.OrderCombox.FormattingEnabled = true;
            this.OrderCombox.Items.AddRange(new object[] {
            "2",
            "3",
            "4"});
            this.OrderCombox.Location = new System.Drawing.Point(230, 13);
            this.OrderCombox.Margin = new System.Windows.Forms.Padding(4);
            this.OrderCombox.Name = "OrderCombox";
            this.OrderCombox.Size = new System.Drawing.Size(174, 28);
            this.OrderCombox.TabIndex = 43;
            this.OrderCombox.Text = "2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.label1.Location = new System.Drawing.Point(9, 175);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(203, 20);
            this.label1.TabIndex = 41;
            this.label1.Text = "NearCorectionTolerance";
            // 
            // ThresholdText
            // 
            this.ThresholdText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ThresholdText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ThresholdText.Location = new System.Drawing.Point(230, 120);
            this.ThresholdText.Margin = new System.Windows.Forms.Padding(4);
            this.ThresholdText.Name = "ThresholdText";
            this.ThresholdText.Size = new System.Drawing.Size(174, 27);
            this.ThresholdText.TabIndex = 40;
            this.ThresholdText.Text = "0.3";
            // 
            // AIMThresholdLable
            // 
            this.AIMThresholdLable.AutoSize = true;
            this.AIMThresholdLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AIMThresholdLable.Location = new System.Drawing.Point(9, 123);
            this.AIMThresholdLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.AIMThresholdLable.Name = "AIMThresholdLable";
            this.AIMThresholdLable.Size = new System.Drawing.Size(112, 20);
            this.AIMThresholdLable.TabIndex = 39;
            this.AIMThresholdLable.Text = "Threshold(λ)";
            // 
            // NearTolerance
            // 
            this.NearTolerance.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.NearTolerance.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.NearTolerance.ForeColor = System.Drawing.Color.Black;
            this.NearTolerance.Location = new System.Drawing.Point(230, 172);
            this.NearTolerance.Margin = new System.Windows.Forms.Padding(4);
            this.NearTolerance.Name = "NearTolerance";
            this.NearTolerance.Size = new System.Drawing.Size(174, 27);
            this.NearTolerance.TabIndex = 19;
            this.NearTolerance.Text = "1.0e-2";
            // 
            // AIMIntervalLable
            // 
            this.AIMIntervalLable.AutoSize = true;
            this.AIMIntervalLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AIMIntervalLable.Location = new System.Drawing.Point(9, 68);
            this.AIMIntervalLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.AIMIntervalLable.Name = "AIMIntervalLable";
            this.AIMIntervalLable.Size = new System.Drawing.Size(96, 20);
            this.AIMIntervalLable.TabIndex = 33;
            this.AIMIntervalLable.Text = "Interval(λ)";
            // 
            // AIMOrderLable
            // 
            this.AIMOrderLable.AutoSize = true;
            this.AIMOrderLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AIMOrderLable.Location = new System.Drawing.Point(9, 16);
            this.AIMOrderLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.AIMOrderLable.Name = "AIMOrderLable";
            this.AIMOrderLable.Size = new System.Drawing.Size(54, 20);
            this.AIMOrderLable.TabIndex = 26;
            this.AIMOrderLable.Text = "Order";
            // 
            // IntervalText
            // 
            this.IntervalText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.IntervalText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.IntervalText.Location = new System.Drawing.Point(230, 65);
            this.IntervalText.Margin = new System.Windows.Forms.Padding(4);
            this.IntervalText.Name = "IntervalText";
            this.IntervalText.Size = new System.Drawing.Size(174, 27);
            this.IntervalText.TabIndex = 23;
            this.IntervalText.Text = "0.05";
            // 
            // FillingType
            // 
            this.FillingType.AutoSize = true;
            this.FillingType.Checked = true;
            this.FillingType.CheckState = System.Windows.Forms.CheckState.Checked;
            this.FillingType.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.FillingType.Location = new System.Drawing.Point(13, 359);
            this.FillingType.Name = "FillingType";
            this.FillingType.Size = new System.Drawing.Size(224, 24);
            this.FillingType.TabIndex = 49;
            this.FillingType.Text = "Triangle Filling Strategy";
            this.FillingType.UseVisualStyleBackColor = true;
            // 
            // MulRadioButton
            // 
            this.MulRadioButton.AutoCheck = false;
            this.MulRadioButton.AutoSize = true;
            this.MulRadioButton.Enabled = false;
            this.MulRadioButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MulRadioButton.Location = new System.Drawing.Point(366, 13);
            this.MulRadioButton.Margin = new System.Windows.Forms.Padding(4);
            this.MulRadioButton.Name = "MulRadioButton";
            this.MulRadioButton.Size = new System.Drawing.Size(55, 24);
            this.MulRadioButton.TabIndex = 46;
            this.MulRadioButton.TabStop = true;
            this.MulRadioButton.Text = "Mul";
            this.MulRadioButton.UseVisualStyleBackColor = true;
            // 
            // MoMRadioButton
            // 
            this.MoMRadioButton.AutoSize = true;
            this.MoMRadioButton.Checked = true;
            this.MoMRadioButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MoMRadioButton.Location = new System.Drawing.Point(13, 13);
            this.MoMRadioButton.Margin = new System.Windows.Forms.Padding(4);
            this.MoMRadioButton.Name = "MoMRadioButton";
            this.MoMRadioButton.Size = new System.Drawing.Size(63, 24);
            this.MoMRadioButton.TabIndex = 44;
            this.MoMRadioButton.TabStop = true;
            this.MoMRadioButton.Text = "MoM";
            this.MoMRadioButton.UseVisualStyleBackColor = true;
            this.MoMRadioButton.CheckedChanged += new System.EventHandler(this.MoMRadioButton_CheckedChanged);
            // 
            // AIMRadioButton
            // 
            this.AIMRadioButton.AutoSize = true;
            this.AIMRadioButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AIMRadioButton.Location = new System.Drawing.Point(179, 13);
            this.AIMRadioButton.Margin = new System.Windows.Forms.Padding(4);
            this.AIMRadioButton.Name = "AIMRadioButton";
            this.AIMRadioButton.Size = new System.Drawing.Size(58, 24);
            this.AIMRadioButton.TabIndex = 45;
            this.AIMRadioButton.Text = "AIM";
            this.AIMRadioButton.UseVisualStyleBackColor = true;
            this.AIMRadioButton.CheckedChanged += new System.EventHandler(this.AIMRadioButton_CheckedChanged);
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(323, 650);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 51;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = false;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(12, 650);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 50;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // IEPanel
            // 
            this.IEPanel.Controls.Add(this.ZsText);
            this.IEPanel.Controls.Add(this.ZsLable);
            this.IEPanel.Controls.Add(this.EtaText);
            this.IEPanel.Controls.Add(this.EtaLable);
            this.IEPanel.Controls.Add(this.AlphaLable);
            this.IEPanel.Controls.Add(this.AlphaText);
            this.IEPanel.Controls.Add(this.IBCCFIEButton);
            this.IEPanel.Controls.Add(this.IBCMFIEButton);
            this.IEPanel.Controls.Add(this.IBCEFIEButton);
            this.IEPanel.Controls.Add(this.CFIEButton);
            this.IEPanel.Controls.Add(this.MFIEButton);
            this.IEPanel.Controls.Add(this.EFIEButton);
            this.IEPanel.Controls.Add(this.IESelectPanel);
            this.IEPanel.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.IEPanel.Location = new System.Drawing.Point(13, 411);
            this.IEPanel.Name = "IEPanel";
            this.IEPanel.Size = new System.Drawing.Size(404, 222);
            this.IEPanel.TabIndex = 52;
            this.IEPanel.TabStop = false;
            this.IEPanel.Text = "Integral Equation";
            // 
            // ZsText
            // 
            this.ZsText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ZsText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ZsText.Location = new System.Drawing.Point(223, 187);
            this.ZsText.Margin = new System.Windows.Forms.Padding(4);
            this.ZsText.Name = "ZsText";
            this.ZsText.Size = new System.Drawing.Size(174, 27);
            this.ZsText.TabIndex = 62;
            this.ZsText.Text = "376.991112";
            // 
            // ZsLable
            // 
            this.ZsLable.AutoSize = true;
            this.ZsLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ZsLable.Location = new System.Drawing.Point(9, 190);
            this.ZsLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.ZsLable.Name = "ZsLable";
            this.ZsLable.Size = new System.Drawing.Size(29, 20);
            this.ZsLable.TabIndex = 61;
            this.ZsLable.Text = "Zs";
            // 
            // EtaText
            // 
            this.EtaText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EtaText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.EtaText.Location = new System.Drawing.Point(223, 152);
            this.EtaText.Margin = new System.Windows.Forms.Padding(4);
            this.EtaText.Name = "EtaText";
            this.EtaText.Size = new System.Drawing.Size(174, 27);
            this.EtaText.TabIndex = 60;
            this.EtaText.Text = "376.991112";
            // 
            // EtaLable
            // 
            this.EtaLable.AutoSize = true;
            this.EtaLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.EtaLable.Location = new System.Drawing.Point(9, 155);
            this.EtaLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.EtaLable.Name = "EtaLable";
            this.EtaLable.Size = new System.Drawing.Size(36, 20);
            this.EtaLable.TabIndex = 59;
            this.EtaLable.Text = "Eta";
            // 
            // AlphaLable
            // 
            this.AlphaLable.AutoSize = true;
            this.AlphaLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AlphaLable.Location = new System.Drawing.Point(9, 121);
            this.AlphaLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.AlphaLable.Name = "AlphaLable";
            this.AlphaLable.Size = new System.Drawing.Size(55, 20);
            this.AlphaLable.TabIndex = 49;
            this.AlphaLable.Text = "Alpha";
            // 
            // AlphaText
            // 
            this.AlphaText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AlphaText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.AlphaText.Location = new System.Drawing.Point(223, 114);
            this.AlphaText.Margin = new System.Windows.Forms.Padding(4);
            this.AlphaText.Name = "AlphaText";
            this.AlphaText.Size = new System.Drawing.Size(174, 27);
            this.AlphaText.TabIndex = 48;
            this.AlphaText.Text = "0.3";
            // 
            // IBCCFIEButton
            // 
            this.IBCCFIEButton.AutoSize = true;
            this.IBCCFIEButton.Enabled = false;
            this.IBCCFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.IBCCFIEButton.Location = new System.Drawing.Point(291, 72);
            this.IBCCFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.IBCCFIEButton.Name = "IBCCFIEButton";
            this.IBCCFIEButton.Size = new System.Drawing.Size(93, 24);
            this.IBCCFIEButton.TabIndex = 58;
            this.IBCCFIEButton.Text = "IBCCFIE";
            this.IBCCFIEButton.UseVisualStyleBackColor = true;
            // 
            // IBCMFIEButton
            // 
            this.IBCMFIEButton.AutoSize = true;
            this.IBCMFIEButton.Enabled = false;
            this.IBCMFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.IBCMFIEButton.Location = new System.Drawing.Point(149, 72);
            this.IBCMFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.IBCMFIEButton.Name = "IBCMFIEButton";
            this.IBCMFIEButton.Size = new System.Drawing.Size(95, 24);
            this.IBCMFIEButton.TabIndex = 57;
            this.IBCMFIEButton.Text = "IBCMFIE";
            this.IBCMFIEButton.UseVisualStyleBackColor = true;
            // 
            // IBCEFIEButton
            // 
            this.IBCEFIEButton.AutoSize = true;
            this.IBCEFIEButton.Enabled = false;
            this.IBCEFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.IBCEFIEButton.Location = new System.Drawing.Point(13, 72);
            this.IBCEFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.IBCEFIEButton.Name = "IBCEFIEButton";
            this.IBCEFIEButton.Size = new System.Drawing.Size(92, 24);
            this.IBCEFIEButton.TabIndex = 56;
            this.IBCEFIEButton.Text = "IBCEFIE";
            this.IBCEFIEButton.UseVisualStyleBackColor = true;
            // 
            // CFIEButton
            // 
            this.CFIEButton.AutoSize = true;
            this.CFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CFIEButton.Location = new System.Drawing.Point(291, 27);
            this.CFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.CFIEButton.Name = "CFIEButton";
            this.CFIEButton.Size = new System.Drawing.Size(64, 24);
            this.CFIEButton.TabIndex = 55;
            this.CFIEButton.Text = "CFIE";
            this.CFIEButton.UseVisualStyleBackColor = true;
            // 
            // MFIEButton
            // 
            this.MFIEButton.AutoSize = true;
            this.MFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MFIEButton.Location = new System.Drawing.Point(149, 27);
            this.MFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.MFIEButton.Name = "MFIEButton";
            this.MFIEButton.Size = new System.Drawing.Size(66, 24);
            this.MFIEButton.TabIndex = 54;
            this.MFIEButton.Text = "MFIE";
            this.MFIEButton.UseVisualStyleBackColor = true;
            // 
            // EFIEButton
            // 
            this.EFIEButton.AutoSize = true;
            this.EFIEButton.Checked = true;
            this.EFIEButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.EFIEButton.Location = new System.Drawing.Point(13, 27);
            this.EFIEButton.Margin = new System.Windows.Forms.Padding(4);
            this.EFIEButton.Name = "EFIEButton";
            this.EFIEButton.Size = new System.Drawing.Size(63, 24);
            this.EFIEButton.TabIndex = 53;
            this.EFIEButton.TabStop = true;
            this.EFIEButton.Text = "EFIE";
            this.EFIEButton.UseVisualStyleBackColor = true;
            // 
            // IESelectPanel
            // 
            this.IESelectPanel.Location = new System.Drawing.Point(6, 21);
            this.IESelectPanel.Name = "IESelectPanel";
            this.IESelectPanel.Size = new System.Drawing.Size(391, 86);
            this.IESelectPanel.TabIndex = 53;
            // 
            // FormMethod
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(431, 704);
            this.ControlBox = false;
            this.Controls.Add(this.IEPanel);
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.FillingType);
            this.Controls.Add(this.AIMPanel);
            this.Controls.Add(this.MulRadioButton);
            this.Controls.Add(this.MoMRadioButton);
            this.Controls.Add(this.AIMRadioButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormMethod";
            this.Text = "インピーダンス";
            this.AIMPanel.ResumeLayout(false);
            this.AIMPanel.PerformLayout();
            this.IEPanel.ResumeLayout(false);
            this.IEPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel AIMPanel;
        private System.Windows.Forms.CheckBox FillingType;
        private System.Windows.Forms.RadioButton Dimension3D;
        private System.Windows.Forms.RadioButton Dimension2D;
        private System.Windows.Forms.Label DimensionLable;
        private System.Windows.Forms.ComboBox OrderCombox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox ThresholdText;
        private System.Windows.Forms.Label AIMThresholdLable;
        private System.Windows.Forms.TextBox NearTolerance;
        private System.Windows.Forms.Label AIMIntervalLable;
        private System.Windows.Forms.Label AIMOrderLable;
        private System.Windows.Forms.TextBox IntervalText;
        private System.Windows.Forms.RadioButton MulRadioButton;
        private System.Windows.Forms.RadioButton MoMRadioButton;
        private System.Windows.Forms.RadioButton AIMRadioButton;
        private System.Windows.Forms.Button CloseButton ;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.CheckBox VirtualGridTechniqueCheckBox;
        private System.Windows.Forms.GroupBox IEPanel;
        private System.Windows.Forms.TextBox ZsText;
        private System.Windows.Forms.Label ZsLable;
        private System.Windows.Forms.TextBox EtaText;
        private System.Windows.Forms.Label EtaLable;
        private System.Windows.Forms.Label AlphaLable;
        private System.Windows.Forms.TextBox AlphaText;
        private System.Windows.Forms.RadioButton IBCCFIEButton;
        private System.Windows.Forms.RadioButton IBCMFIEButton;
        private System.Windows.Forms.RadioButton IBCEFIEButton;
        private System.Windows.Forms.RadioButton CFIEButton;
        private System.Windows.Forms.RadioButton MFIEButton;
        private System.Windows.Forms.RadioButton EFIEButton;
        private System.Windows.Forms.Panel IESelectPanel;
    }
}