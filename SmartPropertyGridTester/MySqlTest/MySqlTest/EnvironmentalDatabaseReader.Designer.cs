namespace MySqlTest
{
    partial class EnvironmentalDatabaseReader
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.rdoBathymetry = new System.Windows.Forms.RadioButton();
            this.rdoWaterColumn = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.rdoBathymetry);
            this.groupBox1.Controls.Add(this.rdoWaterColumn);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(121, 68);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "File Data Type";
            // 
            // rdoBathymetry
            // 
            this.rdoBathymetry.AutoSize = true;
            this.rdoBathymetry.Location = new System.Drawing.Point(6, 42);
            this.rdoBathymetry.Name = "rdoBathymetry";
            this.rdoBathymetry.Size = new System.Drawing.Size(101, 17);
            this.rdoBathymetry.TabIndex = 3;
            this.rdoBathymetry.TabStop = true;
            this.rdoBathymetry.Text = "Bathymetry data";
            this.rdoBathymetry.UseVisualStyleBackColor = true;
            // 
            // rdoWaterColumn
            // 
            this.rdoWaterColumn.AutoSize = true;
            this.rdoWaterColumn.Location = new System.Drawing.Point(6, 19);
            this.rdoWaterColumn.Name = "rdoWaterColumn";
            this.rdoWaterColumn.Size = new System.Drawing.Size(116, 17);
            this.rdoWaterColumn.TabIndex = 2;
            this.rdoWaterColumn.TabStop = true;
            this.rdoWaterColumn.Text = "Water Column data";
            this.rdoWaterColumn.UseVisualStyleBackColor = true;
            // 
            // EnvironmentalDatabaseReader
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "EnvironmentalDatabaseReader";
            this.Size = new System.Drawing.Size(223, 150);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.EnvironmentalDatabaseReader_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.EnvironmentalDatabaseReader_DragEnter);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton rdoBathymetry;
        private System.Windows.Forms.RadioButton rdoWaterColumn;

    }
}
