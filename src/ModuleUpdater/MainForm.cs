using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ModuleUpdater
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.Button loginButton;
		private System.Windows.Forms.GroupBox loginGroupBox;
		private System.Windows.Forms.TextBox usernameTextBox;
		private System.Windows.Forms.TextBox passwordTextBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			// Required for Windows Form Designer support
			InitializeComponent();
			EnableVisualStyles.Enable(this);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainForm));
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.cancelButton = new System.Windows.Forms.Button();
			this.loginGroupBox = new System.Windows.Forms.GroupBox();
			this.passwordTextBox = new System.Windows.Forms.TextBox();
			this.usernameTextBox = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.loginButton = new System.Windows.Forms.Button();
			this.loginGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabPage2
			// 
			this.tabPage2.Location = new System.Drawing.Point(0, 0);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.TabIndex = 0;
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(200, 152);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(80, 23);
			this.cancelButton.TabIndex = 0;
			this.cancelButton.Text = "Cancel";
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// loginGroupBox
			// 
			this.loginGroupBox.Controls.Add(this.passwordTextBox);
			this.loginGroupBox.Controls.Add(this.usernameTextBox);
			this.loginGroupBox.Controls.Add(this.label2);
			this.loginGroupBox.Controls.Add(this.label1);
			this.loginGroupBox.Location = new System.Drawing.Point(8, 72);
			this.loginGroupBox.Name = "loginGroupBox";
			this.loginGroupBox.Size = new System.Drawing.Size(272, 72);
			this.loginGroupBox.TabIndex = 1;
			this.loginGroupBox.TabStop = false;
			this.loginGroupBox.Text = "Login Info";
			// 
			// passwordTextBox
			// 
			this.passwordTextBox.Location = new System.Drawing.Point(64, 40);
			this.passwordTextBox.Name = "passwordTextBox";
			this.passwordTextBox.PasswordChar = '*';
			this.passwordTextBox.Size = new System.Drawing.Size(200, 20);
			this.passwordTextBox.TabIndex = 3;
			this.passwordTextBox.Text = "";
			// 
			// usernameTextBox
			// 
			this.usernameTextBox.Location = new System.Drawing.Point(64, 16);
			this.usernameTextBox.Name = "usernameTextBox";
			this.usernameTextBox.Size = new System.Drawing.Size(200, 20);
			this.usernameTextBox.TabIndex = 2;
			this.usernameTextBox.Text = "";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(64, 16);
			this.label2.TabIndex = 1;
			this.label2.Text = "Password:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Username:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// pictureBox1
			// 
			this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
			this.pictureBox1.Location = new System.Drawing.Point(8, 8);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(272, 56);
			this.pictureBox1.TabIndex = 2;
			this.pictureBox1.TabStop = false;
			// 
			// loginButton
			// 
			this.loginButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.loginButton.Location = new System.Drawing.Point(112, 152);
			this.loginButton.Name = "loginButton";
			this.loginButton.Size = new System.Drawing.Size(80, 23);
			this.loginButton.TabIndex = 3;
			this.loginButton.Text = "Login";
			this.loginButton.Click += new System.EventHandler(this.loginButton_Click);
			// 
			// MainForm
			// 
			this.AcceptButton = this.loginButton;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(288, 182);
			this.Controls.Add(this.loginButton);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.loginGroupBox);
			this.Controls.Add(this.cancelButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Module Updater Login";
			this.loginGroupBox.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			// we check to make sure that the Operating System is Windows XP or greater enable XP themes
			if (Environment.OSVersion.Version.Major > 4 & Environment.OSVersion.Version.Minor > 0)
				Application.EnableVisualStyles();

			Application.Run(new MainForm());
		}

		private void loginButton_Click(object sender, System.EventArgs e)
		{
			if (usernameTextBox.Text == "EarLab" && passwordTextBox.Text == "EarLab")
			{
				Cursor.Current = Cursors.WaitCursor;
				Dialogs.WebServiceDialog webserviceDialog = new Dialogs.WebServiceDialog();
				this.Hide();
				Cursor.Current = Cursors.Default;

				webserviceDialog.ShowDialog(this);
				this.Close();
			}
			else
				MessageBox.Show(this, "Incorrect Username/Password entered.  Please try again.", "Incorrect Entry", MessageBoxButtons.OK, MessageBoxIcon.Stop);
		}

		private void cancelButton_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}
	}
}
