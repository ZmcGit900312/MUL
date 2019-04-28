namespace Configuration
{
    partial class FormExcitation
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
            this.SourceCard = new System.Windows.Forms.TabControl();
            this.PlanewaveTablePage = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.EllipitcityText = new System.Windows.Forms.TextBox();
            this.EllipticityLable = new System.Windows.Forms.Label();
            this.EtaText = new System.Windows.Forms.TextBox();
            this.PhaseText = new System.Windows.Forms.TextBox();
            this.MagenitudeText = new System.Windows.Forms.TextBox();
            this.PolarisationLable = new System.Windows.Forms.Label();
            this.PhaseLable = new System.Windows.Forms.Label();
            this.MagnitudeLable = new System.Windows.Forms.Label();
            this.LoopPWGroupBox = new System.Windows.Forms.GroupBox();
            this.PWPhiIncrementText = new System.Windows.Forms.TextBox();
            this.PWThetaIncrementText = new System.Windows.Forms.TextBox();
            this.PWPhiStartText = new System.Windows.Forms.TextBox();
            this.PWThetaStartText = new System.Windows.Forms.TextBox();
            this.PWPhiIncrementLable = new System.Windows.Forms.Label();
            this.PWThetaIncrementLable = new System.Windows.Forms.Label();
            this.PWPhiStartLable = new System.Windows.Forms.Label();
            this.PWPhiNumberLable = new System.Windows.Forms.Label();
            this.PWThetaNumberLable = new System.Windows.Forms.Label();
            this.PWPhiNumberText = new System.Windows.Forms.TextBox();
            this.PWThetaStartLable = new System.Windows.Forms.Label();
            this.PWThetaNumberText = new System.Windows.Forms.TextBox();
            this.RotationGroupBox = new System.Windows.Forms.GroupBox();
            this.RZText = new System.Windows.Forms.TextBox();
            this.RYText = new System.Windows.Forms.TextBox();
            this.RXText = new System.Windows.Forms.TextBox();
            this.RZLable = new System.Windows.Forms.Label();
            this.RYLable = new System.Windows.Forms.Label();
            this.RXLabel = new System.Windows.Forms.Label();
            this.PolarisationGroupBox = new System.Windows.Forms.GroupBox();
            this.RightRadioButton = new System.Windows.Forms.RadioButton();
            this.LinearRadioButton = new System.Windows.Forms.RadioButton();
            this.LeftRadioButton = new System.Windows.Forms.RadioButton();
            this.PWNameText = new System.Windows.Forms.TextBox();
            this.SourceNameLabel = new System.Windows.Forms.Label();
            this.CloseButton = new System.Windows.Forms.Button();
            this.ApplyButton = new System.Windows.Forms.Button();
            this.VoltageTablePage = new System.Windows.Forms.TabPage();
            this.ActivityButton = new System.Windows.Forms.RadioButton();
            this.SourceCard.SuspendLayout();
            this.PlanewaveTablePage.SuspendLayout();
            this.panel2.SuspendLayout();
            this.LoopPWGroupBox.SuspendLayout();
            this.RotationGroupBox.SuspendLayout();
            this.PolarisationGroupBox.SuspendLayout();
            this.VoltageTablePage.SuspendLayout();
            this.SuspendLayout();
            // 
            // SourceCard
            // 
            this.SourceCard.Controls.Add(this.PlanewaveTablePage);
            this.SourceCard.Controls.Add(this.VoltageTablePage);
            this.SourceCard.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.SourceCard.Location = new System.Drawing.Point(12, 12);
            this.SourceCard.Name = "SourceCard";
            this.SourceCard.SelectedIndex = 0;
            this.SourceCard.Size = new System.Drawing.Size(514, 792);
            this.SourceCard.TabIndex = 0;
            // 
            // PlanewaveTablePage
            // 
            this.PlanewaveTablePage.Controls.Add(this.panel2);
            this.PlanewaveTablePage.Controls.Add(this.LoopPWGroupBox);
            this.PlanewaveTablePage.Controls.Add(this.RotationGroupBox);
            this.PlanewaveTablePage.Controls.Add(this.PolarisationGroupBox);
            this.PlanewaveTablePage.Controls.Add(this.PWNameText);
            this.PlanewaveTablePage.Controls.Add(this.SourceNameLabel);
            this.PlanewaveTablePage.Location = new System.Drawing.Point(4, 29);
            this.PlanewaveTablePage.Name = "PlanewaveTablePage";
            this.PlanewaveTablePage.Padding = new System.Windows.Forms.Padding(3);
            this.PlanewaveTablePage.Size = new System.Drawing.Size(506, 759);
            this.PlanewaveTablePage.TabIndex = 6;
            this.PlanewaveTablePage.Text = "Planewave";
            this.PlanewaveTablePage.UseVisualStyleBackColor = true;
            // 
            // panel2
            // 
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel2.Controls.Add(this.EllipitcityText);
            this.panel2.Controls.Add(this.EllipticityLable);
            this.panel2.Controls.Add(this.EtaText);
            this.panel2.Controls.Add(this.PhaseText);
            this.panel2.Controls.Add(this.MagenitudeText);
            this.panel2.Controls.Add(this.PolarisationLable);
            this.panel2.Controls.Add(this.PhaseLable);
            this.panel2.Controls.Add(this.MagnitudeLable);
            this.panel2.Location = new System.Drawing.Point(3, 533);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(497, 142);
            this.panel2.TabIndex = 5;
            // 
            // EllipitcityText
            // 
            this.EllipitcityText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EllipitcityText.Enabled = false;
            this.EllipitcityText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.EllipitcityText.Location = new System.Drawing.Point(268, 104);
            this.EllipitcityText.Margin = new System.Windows.Forms.Padding(4);
            this.EllipitcityText.Name = "EllipitcityText";
            this.EllipitcityText.Size = new System.Drawing.Size(222, 27);
            this.EllipitcityText.TabIndex = 69;
            this.EllipitcityText.Text = "0.0";
            // 
            // EllipticityLable
            // 
            this.EllipticityLable.AutoSize = true;
            this.EllipticityLable.Enabled = false;
            this.EllipticityLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EllipticityLable.Location = new System.Drawing.Point(9, 107);
            this.EllipticityLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.EllipticityLable.Name = "EllipticityLable";
            this.EllipticityLable.Size = new System.Drawing.Size(243, 20);
            this.EllipticityLable.TabIndex = 68;
            this.EllipticityLable.Text = "Ellipticity (between 0 and 1)";
            // 
            // EtaText
            // 
            this.EtaText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EtaText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.EtaText.Location = new System.Drawing.Point(268, 73);
            this.EtaText.Margin = new System.Windows.Forms.Padding(4);
            this.EtaText.Name = "EtaText";
            this.EtaText.Size = new System.Drawing.Size(222, 27);
            this.EtaText.TabIndex = 67;
            this.EtaText.Text = "0.0";
            // 
            // PhaseText
            // 
            this.PhaseText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PhaseText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PhaseText.Location = new System.Drawing.Point(268, 41);
            this.PhaseText.Margin = new System.Windows.Forms.Padding(4);
            this.PhaseText.Name = "PhaseText";
            this.PhaseText.Size = new System.Drawing.Size(222, 27);
            this.PhaseText.TabIndex = 66;
            this.PhaseText.Text = "0.0";
            // 
            // MagenitudeText
            // 
            this.MagenitudeText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MagenitudeText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.MagenitudeText.Location = new System.Drawing.Point(268, 10);
            this.MagenitudeText.Margin = new System.Windows.Forms.Padding(4);
            this.MagenitudeText.Name = "MagenitudeText";
            this.MagenitudeText.Size = new System.Drawing.Size(222, 27);
            this.MagenitudeText.TabIndex = 63;
            this.MagenitudeText.Text = "1.0";
            // 
            // PolarisationLable
            // 
            this.PolarisationLable.AutoSize = true;
            this.PolarisationLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PolarisationLable.Location = new System.Drawing.Point(9, 76);
            this.PolarisationLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PolarisationLable.Name = "PolarisationLable";
            this.PolarisationLable.Size = new System.Drawing.Size(258, 20);
            this.PolarisationLable.TabIndex = 65;
            this.PolarisationLable.Text = "Polarisation angle η (degrees)";
            // 
            // PhaseLable
            // 
            this.PhaseLable.AutoSize = true;
            this.PhaseLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PhaseLable.Location = new System.Drawing.Point(9, 44);
            this.PhaseLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PhaseLable.Name = "PhaseLable";
            this.PhaseLable.Size = new System.Drawing.Size(143, 20);
            this.PhaseLable.TabIndex = 64;
            this.PhaseLable.Text = "Phase (degrees)";
            // 
            // MagnitudeLable
            // 
            this.MagnitudeLable.AutoSize = true;
            this.MagnitudeLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MagnitudeLable.Location = new System.Drawing.Point(9, 10);
            this.MagnitudeLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.MagnitudeLable.Name = "MagnitudeLable";
            this.MagnitudeLable.Size = new System.Drawing.Size(157, 20);
            this.MagnitudeLable.TabIndex = 63;
            this.MagnitudeLable.Text = "Magenitude (V/m)";
            // 
            // LoopPWGroupBox
            // 
            this.LoopPWGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LoopPWGroupBox.Controls.Add(this.PWPhiIncrementText);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaIncrementText);
            this.LoopPWGroupBox.Controls.Add(this.PWPhiStartText);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaStartText);
            this.LoopPWGroupBox.Controls.Add(this.PWPhiIncrementLable);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaIncrementLable);
            this.LoopPWGroupBox.Controls.Add(this.PWPhiStartLable);
            this.LoopPWGroupBox.Controls.Add(this.PWPhiNumberLable);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaNumberLable);
            this.LoopPWGroupBox.Controls.Add(this.PWPhiNumberText);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaStartLable);
            this.LoopPWGroupBox.Controls.Add(this.PWThetaNumberText);
            this.LoopPWGroupBox.Location = new System.Drawing.Point(6, 304);
            this.LoopPWGroupBox.Name = "LoopPWGroupBox";
            this.LoopPWGroupBox.Size = new System.Drawing.Size(497, 223);
            this.LoopPWGroupBox.TabIndex = 4;
            this.LoopPWGroupBox.TabStop = false;
            // 
            // PWPhiIncrementText
            // 
            this.PWPhiIncrementText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWPhiIncrementText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWPhiIncrementText.Location = new System.Drawing.Point(265, 187);
            this.PWPhiIncrementText.Margin = new System.Windows.Forms.Padding(4);
            this.PWPhiIncrementText.MaxLength = 40;
            this.PWPhiIncrementText.Name = "PWPhiIncrementText";
            this.PWPhiIncrementText.Size = new System.Drawing.Size(222, 27);
            this.PWPhiIncrementText.TabIndex = 62;
            this.PWPhiIncrementText.Text = "1.0";
            // 
            // PWThetaIncrementText
            // 
            this.PWThetaIncrementText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWThetaIncrementText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWThetaIncrementText.Location = new System.Drawing.Point(265, 156);
            this.PWThetaIncrementText.Margin = new System.Windows.Forms.Padding(4);
            this.PWThetaIncrementText.MaxLength = 40;
            this.PWThetaIncrementText.Name = "PWThetaIncrementText";
            this.PWThetaIncrementText.Size = new System.Drawing.Size(222, 27);
            this.PWThetaIncrementText.TabIndex = 61;
            this.PWThetaIncrementText.Text = "1.0";
            // 
            // PWPhiStartText
            // 
            this.PWPhiStartText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWPhiStartText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWPhiStartText.Location = new System.Drawing.Point(266, 123);
            this.PWPhiStartText.Margin = new System.Windows.Forms.Padding(4);
            this.PWPhiStartText.MaxLength = 40;
            this.PWPhiStartText.Name = "PWPhiStartText";
            this.PWPhiStartText.Size = new System.Drawing.Size(221, 27);
            this.PWPhiStartText.TabIndex = 60;
            this.PWPhiStartText.Text = "0.0";
            // 
            // PWThetaStartText
            // 
            this.PWThetaStartText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWThetaStartText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWThetaStartText.Location = new System.Drawing.Point(266, 92);
            this.PWThetaStartText.Margin = new System.Windows.Forms.Padding(4);
            this.PWThetaStartText.MaxLength = 40;
            this.PWThetaStartText.Name = "PWThetaStartText";
            this.PWThetaStartText.Size = new System.Drawing.Size(221, 27);
            this.PWThetaStartText.TabIndex = 59;
            this.PWThetaStartText.Text = "0.0";
            // 
            // PWPhiIncrementLable
            // 
            this.PWPhiIncrementLable.AutoSize = true;
            this.PWPhiIncrementLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWPhiIncrementLable.Location = new System.Drawing.Point(9, 190);
            this.PWPhiIncrementLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWPhiIncrementLable.Name = "PWPhiIncrementLable";
            this.PWPhiIncrementLable.Size = new System.Drawing.Size(194, 20);
            this.PWPhiIncrementLable.TabIndex = 56;
            this.PWPhiIncrementLable.Text = "φ increment (degrees)";
            // 
            // PWThetaIncrementLable
            // 
            this.PWThetaIncrementLable.AutoSize = true;
            this.PWThetaIncrementLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWThetaIncrementLable.Location = new System.Drawing.Point(9, 159);
            this.PWThetaIncrementLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWThetaIncrementLable.Name = "PWThetaIncrementLable";
            this.PWThetaIncrementLable.Size = new System.Drawing.Size(191, 20);
            this.PWThetaIncrementLable.TabIndex = 55;
            this.PWThetaIncrementLable.Text = "θ increment (degrees)";
            // 
            // PWPhiStartLable
            // 
            this.PWPhiStartLable.AutoSize = true;
            this.PWPhiStartLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWPhiStartLable.Location = new System.Drawing.Point(9, 126);
            this.PWPhiStartLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWPhiStartLable.Name = "PWPhiStartLable";
            this.PWPhiStartLable.Size = new System.Drawing.Size(212, 20);
            this.PWPhiStartLable.TabIndex = 54;
            this.PWPhiStartLable.Text = "Initial φ value (degrees)";
            // 
            // PWPhiNumberLable
            // 
            this.PWPhiNumberLable.AutoSize = true;
            this.PWPhiNumberLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWPhiNumberLable.Location = new System.Drawing.Point(9, 64);
            this.PWPhiNumberLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWPhiNumberLable.Name = "PWPhiNumberLable";
            this.PWPhiNumberLable.Size = new System.Drawing.Size(173, 20);
            this.PWPhiNumberLable.TabIndex = 53;
            this.PWPhiNumberLable.Text = "Number of φ angles";
            // 
            // PWThetaNumberLable
            // 
            this.PWThetaNumberLable.AutoSize = true;
            this.PWThetaNumberLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWThetaNumberLable.Location = new System.Drawing.Point(9, 31);
            this.PWThetaNumberLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWThetaNumberLable.Name = "PWThetaNumberLable";
            this.PWThetaNumberLable.Size = new System.Drawing.Size(170, 20);
            this.PWThetaNumberLable.TabIndex = 52;
            this.PWThetaNumberLable.Text = "Number of θ angles";
            // 
            // PWPhiNumberText
            // 
            this.PWPhiNumberText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWPhiNumberText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWPhiNumberText.Location = new System.Drawing.Point(266, 61);
            this.PWPhiNumberText.Margin = new System.Windows.Forms.Padding(4);
            this.PWPhiNumberText.Name = "PWPhiNumberText";
            this.PWPhiNumberText.Size = new System.Drawing.Size(221, 27);
            this.PWPhiNumberText.TabIndex = 58;
            this.PWPhiNumberText.Text = "1";
            // 
            // PWThetaStartLable
            // 
            this.PWThetaStartLable.AutoSize = true;
            this.PWThetaStartLable.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PWThetaStartLable.Location = new System.Drawing.Point(9, 95);
            this.PWThetaStartLable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.PWThetaStartLable.Name = "PWThetaStartLable";
            this.PWThetaStartLable.Size = new System.Drawing.Size(209, 20);
            this.PWThetaStartLable.TabIndex = 51;
            this.PWThetaStartLable.Text = "Initial θ value (degrees)";
            // 
            // PWThetaNumberText
            // 
            this.PWThetaNumberText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWThetaNumberText.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.PWThetaNumberText.Location = new System.Drawing.Point(266, 28);
            this.PWThetaNumberText.Margin = new System.Windows.Forms.Padding(4);
            this.PWThetaNumberText.Name = "PWThetaNumberText";
            this.PWThetaNumberText.Size = new System.Drawing.Size(221, 27);
            this.PWThetaNumberText.TabIndex = 57;
            this.PWThetaNumberText.Text = "1";
            // 
            // RotationGroupBox
            // 
            this.RotationGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RotationGroupBox.Controls.Add(this.RZText);
            this.RotationGroupBox.Controls.Add(this.RYText);
            this.RotationGroupBox.Controls.Add(this.RXText);
            this.RotationGroupBox.Controls.Add(this.RZLable);
            this.RotationGroupBox.Controls.Add(this.RYLable);
            this.RotationGroupBox.Controls.Add(this.RXLabel);
            this.RotationGroupBox.Location = new System.Drawing.Point(6, 176);
            this.RotationGroupBox.Name = "RotationGroupBox";
            this.RotationGroupBox.Size = new System.Drawing.Size(494, 122);
            this.RotationGroupBox.TabIndex = 3;
            this.RotationGroupBox.TabStop = false;
            this.RotationGroupBox.Text = "Rotation about axes";
            // 
            // RZText
            // 
            this.RZText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RZText.Enabled = false;
            this.RZText.Location = new System.Drawing.Point(266, 89);
            this.RZText.Name = "RZText";
            this.RZText.Size = new System.Drawing.Size(222, 27);
            this.RZText.TabIndex = 6;
            this.RZText.Text = "0.0";
            // 
            // RYText
            // 
            this.RYText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RYText.Enabled = false;
            this.RYText.Location = new System.Drawing.Point(266, 59);
            this.RYText.Name = "RYText";
            this.RYText.Size = new System.Drawing.Size(222, 27);
            this.RYText.TabIndex = 5;
            this.RYText.Text = "0.0";
            // 
            // RXText
            // 
            this.RXText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RXText.Enabled = false;
            this.RXText.Location = new System.Drawing.Point(266, 28);
            this.RXText.Name = "RXText";
            this.RXText.Size = new System.Drawing.Size(222, 27);
            this.RXText.TabIndex = 4;
            this.RXText.Text = "0.0";
            // 
            // RZLable
            // 
            this.RZLable.AutoSize = true;
            this.RZLable.Location = new System.Drawing.Point(6, 93);
            this.RZLable.Name = "RZLable";
            this.RZLable.Size = new System.Drawing.Size(20, 20);
            this.RZLable.TabIndex = 3;
            this.RZLable.Text = "Z";
            // 
            // RYLable
            // 
            this.RYLable.AutoSize = true;
            this.RYLable.Location = new System.Drawing.Point(6, 62);
            this.RYLable.Name = "RYLable";
            this.RYLable.Size = new System.Drawing.Size(20, 20);
            this.RYLable.TabIndex = 2;
            this.RYLable.Text = "Y";
            // 
            // RXLabel
            // 
            this.RXLabel.AutoSize = true;
            this.RXLabel.Location = new System.Drawing.Point(6, 31);
            this.RXLabel.Name = "RXLabel";
            this.RXLabel.Size = new System.Drawing.Size(20, 20);
            this.RXLabel.TabIndex = 1;
            this.RXLabel.Text = "X";
            // 
            // PolarisationGroupBox
            // 
            this.PolarisationGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PolarisationGroupBox.Controls.Add(this.RightRadioButton);
            this.PolarisationGroupBox.Controls.Add(this.LinearRadioButton);
            this.PolarisationGroupBox.Controls.Add(this.LeftRadioButton);
            this.PolarisationGroupBox.Location = new System.Drawing.Point(6, 50);
            this.PolarisationGroupBox.Name = "PolarisationGroupBox";
            this.PolarisationGroupBox.Size = new System.Drawing.Size(494, 120);
            this.PolarisationGroupBox.TabIndex = 2;
            this.PolarisationGroupBox.TabStop = false;
            this.PolarisationGroupBox.Text = "Polarisation";
            // 
            // RightRadioButton
            // 
            this.RightRadioButton.AutoSize = true;
            this.RightRadioButton.Enabled = false;
            this.RightRadioButton.Location = new System.Drawing.Point(4, 86);
            this.RightRadioButton.Name = "RightRadioButton";
            this.RightRadioButton.Size = new System.Drawing.Size(263, 24);
            this.RightRadioButton.TabIndex = 2;
            this.RightRadioButton.Text = "Right hand rotating elliptical";
            this.RightRadioButton.UseVisualStyleBackColor = true;
            // 
            // LinearRadioButton
            // 
            this.LinearRadioButton.AutoSize = true;
            this.LinearRadioButton.Checked = true;
            this.LinearRadioButton.Location = new System.Drawing.Point(4, 56);
            this.LinearRadioButton.Name = "LinearRadioButton";
            this.LinearRadioButton.Size = new System.Drawing.Size(77, 24);
            this.LinearRadioButton.TabIndex = 1;
            this.LinearRadioButton.TabStop = true;
            this.LinearRadioButton.Text = "Linear";
            this.LinearRadioButton.UseVisualStyleBackColor = true;
            // 
            // LeftRadioButton
            // 
            this.LeftRadioButton.AutoSize = true;
            this.LeftRadioButton.Enabled = false;
            this.LeftRadioButton.Location = new System.Drawing.Point(4, 26);
            this.LeftRadioButton.Name = "LeftRadioButton";
            this.LeftRadioButton.Size = new System.Drawing.Size(252, 24);
            this.LeftRadioButton.TabIndex = 0;
            this.LeftRadioButton.Text = "Left hand rotating elliptical";
            this.LeftRadioButton.UseVisualStyleBackColor = true;
            // 
            // PWNameText
            // 
            this.PWNameText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PWNameText.Location = new System.Drawing.Point(278, 11);
            this.PWNameText.Name = "PWNameText";
            this.PWNameText.Size = new System.Drawing.Size(222, 27);
            this.PWNameText.TabIndex = 1;
            this.PWNameText.Text = "Planewave";
            // 
            // SourceNameLabel
            // 
            this.SourceNameLabel.AutoSize = true;
            this.SourceNameLabel.Location = new System.Drawing.Point(6, 14);
            this.SourceNameLabel.Name = "SourceNameLabel";
            this.SourceNameLabel.Size = new System.Drawing.Size(114, 20);
            this.SourceNameLabel.TabIndex = 0;
            this.SourceNameLabel.Text = "Source name";
            // 
            // CloseButton
            // 
            this.CloseButton.BackColor = System.Drawing.Color.MistyRose;
            this.CloseButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.CloseButton.Location = new System.Drawing.Point(432, 810);
            this.CloseButton.Name = "CloseButton";
            this.CloseButton.Size = new System.Drawing.Size(94, 39);
            this.CloseButton.TabIndex = 57;
            this.CloseButton.Text = "Close";
            this.CloseButton.UseVisualStyleBackColor = false;
            this.CloseButton.Click += new System.EventHandler(this.CloseButton_Click);
            // 
            // ApplyButton
            // 
            this.ApplyButton.BackColor = System.Drawing.Color.MistyRose;
            this.ApplyButton.Font = new System.Drawing.Font("MS Reference Sans Serif", 12F);
            this.ApplyButton.Location = new System.Drawing.Point(12, 810);
            this.ApplyButton.Name = "ApplyButton";
            this.ApplyButton.Size = new System.Drawing.Size(94, 39);
            this.ApplyButton.TabIndex = 56;
            this.ApplyButton.Text = "Apply";
            this.ApplyButton.UseVisualStyleBackColor = false;
            this.ApplyButton.Click += new System.EventHandler(this.ApplyButton_Click);
            // 
            // VoltageTablePage
            // 
            this.VoltageTablePage.Controls.Add(this.ActivityButton);
            this.VoltageTablePage.Location = new System.Drawing.Point(4, 29);
            this.VoltageTablePage.Name = "VoltageTablePage";
            this.VoltageTablePage.Padding = new System.Windows.Forms.Padding(3);
            this.VoltageTablePage.Size = new System.Drawing.Size(506, 759);
            this.VoltageTablePage.TabIndex = 7;
            this.VoltageTablePage.Text = "Voltage";
            this.VoltageTablePage.UseVisualStyleBackColor = true;
            // 
            // ActivityButton
            // 
            this.ActivityButton.AutoSize = true;
            this.ActivityButton.Location = new System.Drawing.Point(22, 24);
            this.ActivityButton.Name = "ActivityButton";
            this.ActivityButton.Size = new System.Drawing.Size(88, 24);
            this.ActivityButton.TabIndex = 0;
            this.ActivityButton.TabStop = true;
            this.ActivityButton.Text = "Activity";
            this.ActivityButton.UseVisualStyleBackColor = true;
            // 
            // FormExcitation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 861);
            this.ControlBox = false;
            this.Controls.Add(this.CloseButton);
            this.Controls.Add(this.ApplyButton);
            this.Controls.Add(this.SourceCard);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormExcitation";
            this.Text = "励起";
            this.SourceCard.ResumeLayout(false);
            this.PlanewaveTablePage.ResumeLayout(false);
            this.PlanewaveTablePage.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.LoopPWGroupBox.ResumeLayout(false);
            this.LoopPWGroupBox.PerformLayout();
            this.RotationGroupBox.ResumeLayout(false);
            this.RotationGroupBox.PerformLayout();
            this.PolarisationGroupBox.ResumeLayout(false);
            this.PolarisationGroupBox.PerformLayout();
            this.VoltageTablePage.ResumeLayout(false);
            this.VoltageTablePage.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl SourceCard;
        private System.Windows.Forms.TabPage PlanewaveTablePage;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox EllipitcityText;
        private System.Windows.Forms.Label EllipticityLable;
        private System.Windows.Forms.TextBox EtaText;
        private System.Windows.Forms.TextBox PhaseText;
        private System.Windows.Forms.TextBox MagenitudeText;
        private System.Windows.Forms.Label PolarisationLable;
        private System.Windows.Forms.Label PhaseLable;
        private System.Windows.Forms.Label MagnitudeLable;
        private System.Windows.Forms.GroupBox LoopPWGroupBox;
        private System.Windows.Forms.TextBox PWPhiIncrementText;
        private System.Windows.Forms.TextBox PWThetaIncrementText;
        private System.Windows.Forms.TextBox PWPhiStartText;
        private System.Windows.Forms.TextBox PWThetaStartText;
        private System.Windows.Forms.Label PWPhiIncrementLable;
        private System.Windows.Forms.Label PWThetaIncrementLable;
        private System.Windows.Forms.Label PWPhiStartLable;
        private System.Windows.Forms.Label PWPhiNumberLable;
        private System.Windows.Forms.Label PWThetaNumberLable;
        private System.Windows.Forms.TextBox PWPhiNumberText;
        private System.Windows.Forms.Label PWThetaStartLable;
        private System.Windows.Forms.TextBox PWThetaNumberText;
        private System.Windows.Forms.GroupBox RotationGroupBox;
        private System.Windows.Forms.TextBox RZText;
        private System.Windows.Forms.TextBox RYText;
        private System.Windows.Forms.TextBox RXText;
        private System.Windows.Forms.Label RZLable;
        private System.Windows.Forms.Label RYLable;
        private System.Windows.Forms.Label RXLabel;
        private System.Windows.Forms.GroupBox PolarisationGroupBox;
        private System.Windows.Forms.RadioButton RightRadioButton;
        private System.Windows.Forms.RadioButton LinearRadioButton;
        private System.Windows.Forms.RadioButton LeftRadioButton;
        private System.Windows.Forms.TextBox PWNameText;
        private System.Windows.Forms.Label SourceNameLabel;
        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.Button ApplyButton;
        private System.Windows.Forms.TabPage VoltageTablePage;
        private System.Windows.Forms.RadioButton ActivityButton;
    }
}