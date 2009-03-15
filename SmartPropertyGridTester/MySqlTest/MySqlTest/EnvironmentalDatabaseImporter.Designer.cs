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
            this.btnEditTypes = new System.Windows.Forms.Button();
            this.btnEditSets = new System.Windows.Forms.Button();
            this.btnEditSubsets = new System.Windows.Forms.Button();
            this.DataSubset = new CustomControls.SimpleBoundDropdown();
            this.DataSet = new CustomControls.SimpleBoundDropdown();
            this.DataType = new CustomControls.SimpleBoundDropdown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
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
            // DataSubset
            // 
            this.DataSubset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataSubset.BackColor = System.Drawing.SystemColors.Window;
            this.DataSubset.DisplayColumn = null;
            this.DataSubset.ForeignKey = null;
            this.DataSubset.ForeignKeyValue = 0;
            this.DataSubset.Location = new System.Drawing.Point(12, 117);
            this.DataSubset.MySqlConnection = null;
            this.DataSubset.Name = "DataSubset";
            this.DataSubset.Size = new System.Drawing.Size(212, 21);
            this.DataSubset.TabIndex = 2;
            this.DataSubset.Table = null;
            this.DataSubset.ValueColumn = null;
            this.DataSubset.KeyChanged += new System.EventHandler<CustomControls.KeyChangedEventArgs>(this.DataSubset_KeyChanged);
            // 
            // DataSet
            // 
            this.DataSet.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataSet.BackColor = System.Drawing.SystemColors.Window;
            this.DataSet.DisplayColumn = null;
            this.DataSet.ForeignKey = null;
            this.DataSet.ForeignKeyValue = 0;
            this.DataSet.Location = new System.Drawing.Point(12, 73);
            this.DataSet.MySqlConnection = null;
            this.DataSet.Name = "DataSet";
            this.DataSet.Size = new System.Drawing.Size(212, 21);
            this.DataSet.TabIndex = 1;
            this.DataSet.Table = null;
            this.DataSet.ValueColumn = null;
            this.DataSet.KeyChanged += new System.EventHandler<CustomControls.KeyChangedEventArgs>(this.DataSet_KeyChanged);
            // 
            // DataType
            // 
            this.DataType.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DataType.BackColor = System.Drawing.SystemColors.Window;
            this.DataType.DisplayColumn = null;
            this.DataType.ForeignKey = null;
            this.DataType.ForeignKeyValue = 0;
            this.DataType.Location = new System.Drawing.Point(11, 29);
            this.DataType.MySqlConnection = null;
            this.DataType.Name = "DataType";
            this.DataType.Size = new System.Drawing.Size(213, 21);
            this.DataType.TabIndex = 0;
            this.DataType.Table = null;
            this.DataType.ValueColumn = null;
            this.DataType.KeyChanged += new System.EventHandler<CustomControls.KeyChangedEventArgs>(this.DataType_KeyChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Data Type";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Data Set";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 101);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Data Subset";
            // 
            // EnvironmentalDatabaseImporter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(318, 266);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
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
            this.PerformLayout();

        }

        #endregion

        private CustomControls.SimpleBoundDropdown DataType;
        private CustomControls.SimpleBoundDropdown DataSet;
        private CustomControls.SimpleBoundDropdown DataSubset;
        private System.Windows.Forms.Button btnEditTypes;
        private System.Windows.Forms.Button btnEditSets;
        private System.Windows.Forms.Button btnEditSubsets;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
    }
}