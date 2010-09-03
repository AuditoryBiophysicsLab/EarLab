namespace RunfileEditor
{
    partial class frmContainer
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label4 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.listViewErrors = new System.Windows.Forms.ListView();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.OpenXMLRunfile = new System.Windows.Forms.Button();
            this.OpenFile = new System.Windows.Forms.Button();
            this.SendFileXMLToEFI = new System.Windows.Forms.Button();
            this.SaveXMLRunfile = new System.Windows.Forms.Button();
            this.menuStrip2 = new System.Windows.Forms.MenuStrip();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem13 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem14 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem15 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem16 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem17 = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.menuStrip2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.AllowDrop = true;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(12, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1024, 512);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.listViewErrors);
            this.tabPage1.Controls.Add(this.pictureBox1);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.OpenXMLRunfile);
            this.tabPage1.Controls.Add(this.OpenFile);
            this.tabPage1.Controls.Add(this.SendFileXMLToEFI);
            this.tabPage1.Controls.Add(this.SaveXMLRunfile);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1016, 486);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "GUI Home";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(56, 249);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 19;
            this.label4.Text = "label4";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(393, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "label2";
            // 
            // listViewErrors
            // 
            this.listViewErrors.Location = new System.Drawing.Point(16, 281);
            this.listViewErrors.MultiSelect = false;
            this.listViewErrors.Name = "listViewErrors";
            this.listViewErrors.Size = new System.Drawing.Size(994, 200);
            this.listViewErrors.TabIndex = 0;
            this.listViewErrors.UseCompatibleStateImageBehavior = false;
            this.listViewErrors.View = System.Windows.Forms.View.List;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(698, 37);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(252, 187);
            this.pictureBox1.TabIndex = 17;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(754, 4);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(130, 13);
            this.label3.TabIndex = 15;
            this.label3.Text = "Small Picture of the Model";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(56, 37);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(173, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Open XML Runfile document from :";
            // 
            // OpenXMLRunfile
            // 
            this.OpenXMLRunfile.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.OpenXMLRunfile.Location = new System.Drawing.Point(16, 70);
            this.OpenXMLRunfile.Name = "OpenXMLRunfile";
            this.OpenXMLRunfile.Size = new System.Drawing.Size(254, 53);
            this.OpenXMLRunfile.TabIndex = 7;
            this.OpenXMLRunfile.Text = "Open Runfile  Document";
            this.OpenXMLRunfile.UseVisualStyleBackColor = true;
            this.OpenXMLRunfile.Click += new System.EventHandler(this.OpenFile_Click);
            // 
            // OpenFile
            // 
            this.OpenFile.Location = new System.Drawing.Point(17, 185);
            this.OpenFile.Name = "OpenFile";
            this.OpenFile.Size = new System.Drawing.Size(254, 50);
            this.OpenFile.TabIndex = 3;
            this.OpenFile.Text = "Exit Runfile Editor";
            this.OpenFile.UseVisualStyleBackColor = true;
            this.OpenFile.Click += new System.EventHandler(this.Exit_Click);
            // 
            // SendFileXMLToEFI
            // 
            this.SendFileXMLToEFI.Location = new System.Drawing.Point(288, 70);
            this.SendFileXMLToEFI.Name = "SendFileXMLToEFI";
            this.SendFileXMLToEFI.Size = new System.Drawing.Size(254, 53);
            this.SendFileXMLToEFI.TabIndex = 2;
            this.SendFileXMLToEFI.Text = "Send Runfile to Earlab";
            this.SendFileXMLToEFI.UseVisualStyleBackColor = true;
            this.SendFileXMLToEFI.Click += new System.EventHandler(this.SendFileXMLToEFI_Click);
            // 
            // SaveXMLRunfile
            // 
            this.SaveXMLRunfile.Location = new System.Drawing.Point(16, 129);
            this.SaveXMLRunfile.Name = "SaveXMLRunfile";
            this.SaveXMLRunfile.Size = new System.Drawing.Size(254, 50);
            this.SaveXMLRunfile.TabIndex = 1;
            this.SaveXMLRunfile.Text = "Save XML Runfile";
            this.SaveXMLRunfile.UseVisualStyleBackColor = true;
            this.SaveXMLRunfile.Click += new System.EventHandler(this.SaveXMLFile_Click);
            // 
            // menuStrip2
            // 
            this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.toolStripMenuItem6,
            this.toolStripMenuItem16});
            this.menuStrip2.Location = new System.Drawing.Point(0, 0);
            this.menuStrip2.Name = "menuStrip2";
            this.menuStrip2.Size = new System.Drawing.Size(1048, 24);
            this.menuStrip2.TabIndex = 2;
            this.menuStrip2.Text = "menuStrip2";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem3,
            this.toolStripMenuItem4,
            this.toolStripMenuItem5});
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(70, 20);
            this.toolStripMenuItem2.Text = "Earlab GUI";
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(139, 22);
            this.toolStripMenuItem3.Text = "About";
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(139, 22);
            this.toolStripMenuItem4.Text = "Online Help";
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(139, 22);
            this.toolStripMenuItem5.Text = "Exit";
            this.toolStripMenuItem5.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem13,
            this.toolStripMenuItem14,
            this.toolStripMenuItem15});
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(35, 20);
            this.toolStripMenuItem6.Text = "File";
            // 
            // toolStripMenuItem13
            // 
            this.toolStripMenuItem13.Name = "toolStripMenuItem13";
            this.toolStripMenuItem13.Size = new System.Drawing.Size(197, 22);
            this.toolStripMenuItem13.Text = "Open Saved Run File...";
            this.toolStripMenuItem13.Click += new System.EventHandler(this.openSavedRunfileToolStripMenuItem_Click);
            // 
            // toolStripMenuItem14
            // 
            this.toolStripMenuItem14.Name = "toolStripMenuItem14";
            this.toolStripMenuItem14.Size = new System.Drawing.Size(197, 22);
            this.toolStripMenuItem14.Text = "Save";
            // 
            // toolStripMenuItem15
            // 
            this.toolStripMenuItem15.Name = "toolStripMenuItem15";
            this.toolStripMenuItem15.Size = new System.Drawing.Size(197, 22);
            this.toolStripMenuItem15.Text = "Save As...";
            // 
            // toolStripMenuItem16
            // 
            this.toolStripMenuItem16.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem17});
            this.toolStripMenuItem16.Name = "toolStripMenuItem16";
            this.toolStripMenuItem16.Size = new System.Drawing.Size(40, 20);
            this.toolStripMenuItem16.Text = "Help";
            // 
            // toolStripMenuItem17
            // 
            this.toolStripMenuItem17.Name = "toolStripMenuItem17";
            this.toolStripMenuItem17.Size = new System.Drawing.Size(139, 22);
            this.toolStripMenuItem17.Text = "Earlab Help";
            // 
            // frmContainer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1048, 564);
            this.Controls.Add(this.menuStrip2);
            this.Controls.Add(this.tabControl1);
            this.IsMdiContainer = true;
            this.Name = "frmContainer";
            this.Text = "Earlab GUI version .7";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.menuStrip2.ResumeLayout(false);
            this.menuStrip2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion


        public void button_create_if_no_errors()
        {
            this.button1 = new System.Windows.Forms.Button();
            // 
            // button1
            // 
            this.tabPage1.Controls.Add(this.button1);
            this.button1.Location = new System.Drawing.Point(288, 185);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(254, 50);
            this.button1.TabIndex = 18;
            this.button1.Text = "Only Appear if No Errors";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.DesktopEarlabLaunch_Click);

        }

        public void remove_button()
        {

            this.tabPage1.Controls.Remove(this.button1);
        }


        //private void create_abstract_tab()
        //{
        //    this.tabPage2 = new System.Windows.Forms.TabPage();
        //    this.tabPage2.SuspendLayout();
        //    ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
        //    // 
        //    // tabPage2
        //    // 
        //    this.tabPage2.Controls.Add(this.label5);
        //    this.tabPage2.Controls.Add(this.label4);
        //    this.tabPage2.Controls.Add(this.textBox1);
        //    this.tabPage2.Controls.Add(this.pictureBox2);
        //    this.tabPage2.Location = new System.Drawing.Point(4, 22);
        //    this.tabPage2.Name = "tabPage2";
        //    this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
        //    this.tabPage2.Size = new System.Drawing.Size(1016, 486);
        //    this.tabPage2.TabIndex = 1;
        //    this.tabPage2.Text = "tabPage2";
        //    this.tabPage2.UseVisualStyleBackColor = true;
        //    this.label5 = new System.Windows.Forms.Label();
        //    this.label4 = new System.Windows.Forms.Label();
        //System.Windows.Forms.TabPage tabPage2;
        //System.Windows.Forms.TextBox textBox1;
        //System.Windows.Forms.PictureBox pictureBox2;
        //System.Windows.Forms.Label label5;
        //System.Windows.Forms.Label label4;
        //// 
        //// label5
        //// 
        //this.label5.AutoSize = true;
        //this.label5.Location = new System.Drawing.Point(579, 42);
        //this.label5.Name = "label5";
        //this.label5.Size = new System.Drawing.Size(35, 13);
        //this.label5.TabIndex = 8;
        //this.label5.Text = "label5";
        //// 
        //// label4
        //// 
        //this.label4.AutoSize = true;
        //this.label4.Location = new System.Drawing.Point(579, 16);
        //this.label4.Name = "label4";
        //this.label4.Size = new System.Drawing.Size(35, 13);
        //this.label4.TabIndex = 7;
        //this.label4.Text = "label4";
        //// 
        //// textBox1
        //// 
        //this.textBox1.Location = new System.Drawing.Point(579, 102);
        //this.textBox1.Multiline = true;
        //this.textBox1.Name = "textBox1";
        //this.textBox1.Size = new System.Drawing.Size(359, 345);
        //this.textBox1.TabIndex = 6;
        //// 
        //// pictureBox2
        //// 
        //this.pictureBox2.Location = new System.Drawing.Point(23, 16);
        //this.pictureBox2.Name = "pictureBox2";
        //this.pictureBox2.Size = new System.Drawing.Size(524, 437);
        //this.pictureBox2.TabIndex = 4;
        //this.pictureBox2.TabStop = false;
        //this.tabPage2.ResumeLayout(false);
        //this.tabPage2.PerformLayout();
        //((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();

        //}


        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button OpenFile;
        private System.Windows.Forms.Button SendFileXMLToEFI;
        private System.Windows.Forms.Button SaveXMLRunfile;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button OpenXMLRunfile;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem6;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem13;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem14;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem15;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem16;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem17;
        public System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label4;





    }
}

