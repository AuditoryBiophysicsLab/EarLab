using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using EarLab.Utilities;

namespace EarLab.Dialogs
{
	/// <summary>
	/// Summary description for AnalysisDialogNew.
	/// </summary>
	public class AnalysisDialogNew : System.Windows.Forms.Form
	{
		private EarLab.Viewers.Layouts.Layout2DColorFull layout2DColor;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

        private double[,] dataArray;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem saveToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem closeToolStripMenuItem;
        private ToolStripMenuItem editToolStripMenuItem;
        private ToolStripMenuItem copyToolStripMenuItem;
        private ToolStripMenuItem toolsToolStripMenuItem;
        private ToolStripMenuItem toolTipsToolStripMenuItem;
        private SaveFileDialog saveFileDialog;
        private bool toolTipShow = false;

		public AnalysisDialogNew(double[,] dataArray, double[] axisArray, string[] axisNames)
		{
			// Required for Windows Form Designer support
			InitializeComponent();

            this.dataArray = dataArray;
			this.layout2DColor.View(dataArray);
			this.layout2DColor.Layout2DColor.ColorbarPanel.SetMinMax(axisArray[4], axisArray[5], true);

			this.layout2DColor.Layout2DColor.ViewerAxisPanel.TopAxisLabel = axisNames[0];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisLabel = axisNames[1];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisStartValue = (float)axisArray[0];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisEndValue = (float)axisArray[1];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat = "0";
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisLabel = axisNames[2];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisStartValue = (float)axisArray[2];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisEndValue = (float)axisArray[3];
			this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisMajorTickNumbersFormat = "0.00e00";
			this.layout2DColor.Layout2DColor.ColorbarAxisPanel.RightAxisLabel = axisNames[3];
			this.layout2DColor.Layout2DColor.ColorbarAxisPanel.RightAxisStartValue = (float)axisArray[4];
			this.layout2DColor.Layout2DColor.ColorbarAxisPanel.RightAxisEndValue = (float)axisArray[5];

            this.layout2DColor.Layout2DColor.ViewerPanel.MouseMove += new MouseEventHandler(ViewerPanel_MouseMove);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

        #region Tool Tip Code
        private void ViewerPanel_MouseMove(object sender, MouseEventArgs e)
        {
            ToolTip toolTip = this.layout2DColor.Layout2DColor.ViewerPanel.ToolTip;
            Point mousePoint = new Point(e.X, e.Y);
            String tipString;

            if (e.Button == MouseButtons.None && this.toolTipShow)
            {
                int xIndex, yIndex;
                double xValue, yValue, dValue;

                dValue = this.layout2DColor.Layout2DColor.PointData(mousePoint, out xIndex, out yIndex);
                yValue = this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisStartValue + (this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisEndValue - this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisStartValue) * ((float)yIndex / (dataArray.GetLength(1) - 1));
                xValue = this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisStartValue + (this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisEndValue - this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisStartValue) * ((float)xIndex / (dataArray.GetLength(0) - 1));

                tipString = "   Location: " + xValue.ToString(this.layout2DColor.Layout2DColor.ViewerAxisPanel.BottomAxisMajorTickNumbersFormat) + ", ";
                tipString += yValue.ToString(this.layout2DColor.Layout2DColor.ViewerAxisPanel.LeftAxisMajorTickNumbersFormat);
                tipString += "\nData Value: " + dValue.ToString("0.00e00");

                toolTip.SetToolTip((Control)this.layout2DColor.Layout2DColor.ViewerPanel, tipString);
                toolTip.Active = true;
            }
            else
            {
                // hide the tool tip (it might have been showing
                toolTip.Active = false;
            }
        }

        private void tipMenuItem_Click(object sender, System.EventArgs e)
        {
            //if (this.tipMenuItem.Checked)
            //    this.tipMenuItem.Checked = this.toolTipShow = false;
            //else
            //    this.tipMenuItem.Checked = this.toolTipShow = true;
        }

        private Bitmap GetBitmap()
        {
            Application.DoEvents();

            Graphics layoutGraphics = this.layout2DColor.CreateGraphics();
            Bitmap layoutBitmap = new Bitmap(this.layout2DColor.ClientSize.Width, this.layout2DColor.ClientSize.Height, layoutGraphics);
            Graphics bitmapGraphics = Graphics.FromImage(layoutBitmap);
            IntPtr layoutDeviceContext = layoutGraphics.GetHdc();
            IntPtr bitmapDeviceContext = bitmapGraphics.GetHdc();
            NativeMethods.BitBlt(bitmapDeviceContext, 0, 0, this.layout2DColor.ClientSize.Width, this.layout2DColor.ClientSize.Height, layoutDeviceContext, 0, 0, 13369376);
            layoutGraphics.ReleaseHdc(layoutDeviceContext);
            bitmapGraphics.ReleaseHdc(bitmapDeviceContext);

            layoutGraphics.Dispose();
            bitmapGraphics.Dispose();

            return layoutBitmap;
        }
        #endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AnalysisDialogNew));
            this.layout2DColor = new EarLab.Viewers.Layouts.Layout2DColorFull();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolTipsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // layout2DColor
            // 
            this.layout2DColor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.layout2DColor.Location = new System.Drawing.Point(0, 27);
            this.layout2DColor.Name = "layout2DColor";
            this.layout2DColor.Size = new System.Drawing.Size(648, 315);
            this.layout2DColor.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(648, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem,
            this.toolStripSeparator1,
            this.closeToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
            this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(144, 6);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
            this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.copyToolStripMenuItem.Text = "&Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolTipsToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // toolTipsToolStripMenuItem
            // 
            this.toolTipsToolStripMenuItem.CheckOnClick = true;
            this.toolTipsToolStripMenuItem.Name = "toolTipsToolStripMenuItem";
            this.toolTipsToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.toolTipsToolStripMenuItem.Text = "ToolTips";
            this.toolTipsToolStripMenuItem.Click += new System.EventHandler(this.toolTipsToolStripMenuItem_Click);
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.Filter = resources.GetString("saveFileDialog.Filter");
            this.saveFileDialog.Title = "Save File...";
            // 
            // AnalysisDialogNew
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(648, 342);
            this.Controls.Add(this.layout2DColor);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(650, 360);
            this.Name = "AnalysisDialogNew";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Data Analysis";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

        private void toolTipsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.toolTipShow = this.toolTipsToolStripMenuItem.Checked;
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                System.Drawing.Imaging.ImageFormat imageFormat;
                switch (saveFileDialog.FilterIndex)
                {
                    case 2:
                        imageFormat = System.Drawing.Imaging.ImageFormat.Jpeg;
                        break;
                    case 3:
                        imageFormat = System.Drawing.Imaging.ImageFormat.Gif;
                        break;
                    case 4:
                        imageFormat = System.Drawing.Imaging.ImageFormat.Png;
                        break;
                    case 5:
                        imageFormat = System.Drawing.Imaging.ImageFormat.Tiff;
                        break;
                    default:
                        imageFormat = System.Drawing.Imaging.ImageFormat.Bmp;
                        break;
                }

                Bitmap saveBitmap = this.GetBitmap();
                saveBitmap.Save(saveFileDialog.FileName, imageFormat);
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Bitmap copyBitmap = this.GetBitmap();
            Clipboard.SetImage(copyBitmap);
        }
	}
}
