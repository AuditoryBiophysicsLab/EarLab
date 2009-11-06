using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;


namespace RunfileEditor
{
    class AbstractTabPageClass : Form
    {
        //private TabControl tabControl1;
        private TabPage tabPageA;

        private RichTextBox richTextBox1;
        private Label label3;
        private Label label2;
        private PictureBox pictureBox1;
        private Label label1;

        public AbstractTabPageClass(EarlabRunfile RunfileObject, string FileNameTemp)
        {

            label1.Text = RunfileObject.RunfileInformation.RunfileInformationAuthor;
            label2.Text = RunfileObject.RunfileInformation.RunfileInformationEditDate;
            richTextBox1.Text = RunfileObject.RunfileInformation.RunfileInformationAbstract;

            string tempPath = Path.GetFullPath(FileNameTemp);
            tempPath = Regex.Replace(tempPath, Path.GetFileName(FileNameTemp), "");
            this.pictureBox1.ImageLocation = Path.Combine(tempPath, RunfileObject.RunfileInformation.RunfileInformationImageLocation); 



            //this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageA = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            //this.tabControl1.SuspendLayout();
            this.tabPageA.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            //this.tabControl1.Controls.Add(this.tabPageA);
            //this.tabControl1.Location = new System.Drawing.Point(13, 13);
            //this.tabControl1.Name = "tabControl1";
            //this.tabControl1.SelectedIndex = 0;
            //this.tabControl1.Size = new System.Drawing.Size(845, 622);
            //this.tabControl1.TabIndex = 0;
            // 
            // tabPageA
            // 
            this.tabPageA.Controls.Add(this.richTextBox1);
            this.tabPageA.Controls.Add(this.label3);
            this.tabPageA.Controls.Add(this.label2);
            this.tabPageA.Controls.Add(this.pictureBox1);
            this.tabPageA.Controls.Add(this.label1);
            this.tabPageA.Location = new System.Drawing.Point(4, 22);
            this.tabPageA.Name = "tabPageA";
            this.tabPageA.Text = "Model Abstract";
            this.tabPageA.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageA.Size = new System.Drawing.Size(837, 596);
            this.tabPageA.TabIndex = 1;
            this.tabPageA.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 0;
            //this.label1.Text = "label1";
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(7, 24);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(502, 566);
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(515, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 2;
            //this.label2.Text = "label2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(515, 41);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 3;
            //this.label3.Text = "label3";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(515, 58);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(316, 532);
            this.richTextBox1.TabIndex = 4;
            this.richTextBox1.Text = "";
            // 
            // AbstractTabPageClass
            // 
            this.ClientSize = new System.Drawing.Size(1053, 672);
            //this.Controls.Add(this.tabControl1);
            this.Name = "AbstractTabPageClass";
            //this.tabControl1.ResumeLayout(false);
            this.tabPageA.ResumeLayout(false);
            this.tabPageA.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        




    }
}
