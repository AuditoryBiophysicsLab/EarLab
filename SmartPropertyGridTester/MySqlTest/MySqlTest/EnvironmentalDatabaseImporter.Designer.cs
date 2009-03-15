namespace MySqlTest
{
    partial class EnvironmentalDatabaseImporter
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
            this.DataType = new CustomControls.SimpleBoundDropdown();
            this.DataSet = new CustomControls.SimpleBoundDropdown();
            this.DataSubset = new CustomControls.SimpleBoundDropdown();
            this.btnEditTypes = new System.Windows.Forms.Button();
            this.btnEditSets = new System.Windows.Forms.Button();
            this.btnEditSubsets = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // DataType
            // 
            this.DataType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataType.BackColor = System.Drawing.SystemColors.Control;
            this.DataType.Caption = "Data Type";
            this.DataType.Location = new System.Drawing.Point(12, 12);
            this.DataType.MySqlConnection = null;
            this.DataType.Name = "DataType";
            this.DataType.Size = new System.Drawing.Size(213, 37);
            this.DataType.TabIndex = 0;
            this.DataType.SelectedValueChanged += new System.EventHandler(this.DataType_SelectedValueChanged);
            // 
            // DataSet
            // 
            this.DataSet.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataSet.BackColor = System.Drawing.SystemColors.Control;
            this.DataSet.Caption = "Data Set";
            this.DataSet.Location = new System.Drawing.Point(13, 56);
            this.DataSet.MySqlConnection = null;
            this.DataSet.Name = "DataSet";
            this.DataSet.Size = new System.Drawing.Size(212, 37);
            this.DataSet.TabIndex = 1;
            this.DataSet.SelectedValueChanged += new System.EventHandler(this.DataSet_SelectedValueChanged);
            // 
            // DataSubset
            // 
            this.DataSubset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataSubset.BackColor = System.Drawing.SystemColors.Control;
            this.DataSubset.Caption = "Data Subset";
            this.DataSubset.Location = new System.Drawing.Point(13, 100);
            this.DataSubset.MySqlConnection = null;
            this.DataSubset.Name = "DataSubset";
            this.DataSubset.Size = new System.Drawing.Size(212, 37);
            this.DataSubset.TabIndex = 2;
            this.DataSubset.SelectedValueChanged += new System.EventHandler(this.DataSubset_SelectedValueChanged);
            // 
            // btnEditTypes
            // 
            this.btnEditTypes.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnEditTypes.Location = new System.Drawing.Point(231, 27);
            this.btnEditTypes.Name = "btnEditTypes";
            this.btnEditTypes.Size = new System.Drawing.Size(75, 23);
            this.btnEditTypes.TabIndex = 3;
            this.btnEditTypes.Text = "Edit...";
            this.btnEditTypes.UseVisualStyleBackColor = true;
            this.btnEditTypes.Click += new System.EventHandler(this.btnEditTypes_Click);
            // 
            // btnEditSets
            // 
            this.btnEditSets.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnEditSets.Location = new System.Drawing.Point(231, 71);
            this.btnEditSets.Name = "btnEditSets";
            this.btnEditSets.Size = new System.Drawing.Size(75, 23);
            this.btnEditSets.TabIndex = 4;
            this.btnEditSets.Text = "Edit...";
            this.btnEditSets.UseVisualStyleBackColor = true;
            this.btnEditSets.Click += new System.EventHandler(this.btnEditSets_Click);
            // 
            // btnEditSubsets
            // 
            this.btnEditSubsets.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnEditSubsets.Location = new System.Drawing.Point(231, 115);
            this.btnEditSubsets.Name = "btnEditSubsets";
            this.btnEditSubsets.Size = new System.Drawing.Size(75, 23);
            this.btnEditSubsets.TabIndex = 5;
            this.btnEditSubsets.Text = "Edit...";
            this.btnEditSubsets.UseVisualStyleBackColor = true;
            // 
            // EnvironmentalDatabaseImporter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(318, 266);
            this.Controls.Add(this.btnEditSubsets);
            this.Controls.Add(this.btnEditSets);
            this.Controls.Add(this.btnEditTypes);
            this.Controls.Add(this.DataSubset);
            this.Controls.Add(this.DataSet);
            this.Controls.Add(this.DataType);
            this.Name = "EnvironmentalDatabaseImporter";
            this.Text = "ESME - Import Environmental Data";
            this.Load += new System.EventHandler(this.EnvironmentalDatabaseImporter_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private CustomControls.SimpleBoundDropdown DataType;
        private CustomControls.SimpleBoundDropdown DataSet;
        private CustomControls.SimpleBoundDropdown DataSubset;
        private System.Windows.Forms.Button btnEditTypes;
        private System.Windows.Forms.Button btnEditSets;
        private System.Windows.Forms.Button btnEditSubsets;
    }
}