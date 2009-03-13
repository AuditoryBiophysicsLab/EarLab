namespace MySqlTest
{
    partial class cboDropdown
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
            this.cboCombo = new System.Windows.Forms.ComboBox();
            this.lblText = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cboCombo
            // 
            this.cboCombo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.cboCombo.FormattingEnabled = true;
            this.cboCombo.Location = new System.Drawing.Point(3, 16);
            this.cboCombo.Name = "cboCombo";
            this.cboCombo.Size = new System.Drawing.Size(158, 21);
            this.cboCombo.TabIndex = 0;
            this.cboCombo.SelectedValueChanged += new System.EventHandler(this.cboCombo_SelectedValueChanged);
            // 
            // lblText
            // 
            this.lblText.AutoSize = true;
            this.lblText.Location = new System.Drawing.Point(3, 0);
            this.lblText.Name = "lblText";
            this.lblText.Size = new System.Drawing.Size(35, 13);
            this.lblText.TabIndex = 1;
            this.lblText.Text = "label1";
            // 
            // cboDropdown
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.Controls.Add(this.lblText);
            this.Controls.Add(this.cboCombo);
            this.Name = "cboDropdown";
            this.Size = new System.Drawing.Size(164, 40);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cboCombo;
        private System.Windows.Forms.Label lblText;
    }
}
