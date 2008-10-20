namespace SmartPropertyGridTester
{
    partial class Form1
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
            VisualHint.SmartPropertyGrid.PropertyComparerDefaultSort propertyComparerDefaultSort1 = new VisualHint.SmartPropertyGrid.PropertyComparerDefaultSort();
            this.btnDone = new System.Windows.Forms.Button();
            this.propertyGrid = new ModulePropertyGrid();
            this.SuspendLayout();
            // 
            // btnDone
            // 
            this.btnDone.Location = new System.Drawing.Point(280, 370);
            this.btnDone.Name = "btnDone";
            this.btnDone.Size = new System.Drawing.Size(75, 23);
            this.btnDone.TabIndex = 1;
            this.btnDone.Text = "Done";
            this.btnDone.UseVisualStyleBackColor = true;
            this.btnDone.Click += new System.EventHandler(this.btnDone_Click);
            // 
            // propertyGrid1
            // 
            this.propertyGrid.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.propertyGrid.Font = new System.Drawing.Font("Tahoma", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
            this.propertyGrid.Location = new System.Drawing.Point(12, 12);
            this.propertyGrid.Name = "propertyGrid1";
            this.propertyGrid.PropertyComparer = propertyComparerDefaultSort1;
            this.propertyGrid.Size = new System.Drawing.Size(343, 352);
            this.propertyGrid.TabIndex = 2;
            this.propertyGrid.Text = "propertyGrid1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(367, 405);
            this.Controls.Add(this.propertyGrid);
            this.Controls.Add(this.btnDone);
            this.Name = "Form1";
            this.Text = "SmartPropertyGrid Test Application";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnDone;
        private ModulePropertyGrid propertyGrid;
    }
}

