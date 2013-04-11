using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.Viewers.Panels
{
	/// <summary>
	/// Summary description for PanelAxisScatterBar.
	/// </summary>
	public class PanelAxisScatterBar : System.Windows.Forms.Panel
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private int controlLeft, controlTop, controlWidth, controlHeight;
		private Pen blackPen = new Pen(System.Drawing.Brushes.Black, 1);
		private int tickMajorWidth = 5;

		private double minValue, maxValue;
		private string labelString;
		private bool showAxis;
		private bool normalOrientation = true;
		private string numberFormat = "0";

		public PanelAxisScatterBar()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// We set up double buffering so that there isn't too much flicker (yeah, this is brilliant)
			this.SetStyle(ControlStyles.UserPaint|ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true); 
			this.UpdateStyles();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// PanelLimitedAxis
			// 
			this.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));

		}
		#endregion

		protected override void OnPaint(PaintEventArgs pe)
		{
			if (this.Controls.Count == 1 && this.showAxis)
			{
				this.controlLeft = this.Controls[0].Left;
				this.controlTop = this.Controls[0].Top;
				this.controlWidth = this.Controls[0].Width;
				this.controlHeight = this.Controls[0].Height;

				StringFormat stringFormat = new StringFormat();
				stringFormat.Alignment = StringAlignment.Near;
				stringFormat.LineAlignment = StringAlignment.Center;

				if (this.normalOrientation)
				{
					pe.Graphics.DrawLine(this.blackPen, this.controlLeft+this.controlWidth, this.controlTop, this.controlLeft+this.controlWidth+tickMajorWidth, this.controlTop);
					pe.Graphics.DrawLine(this.blackPen, this.controlLeft+this.controlWidth, this.controlTop+this.controlHeight-1, this.controlLeft+this.controlWidth+tickMajorWidth, this.controlTop+this.controlHeight-1);
					pe.Graphics.DrawString(this.maxValue.ToString(this.numberFormat), this.Font, Brushes.Black, this.controlLeft+this.controlWidth+tickMajorWidth+2, this.controlTop, stringFormat);
					pe.Graphics.DrawString(this.minValue.ToString(this.numberFormat), this.Font, Brushes.Black, this.controlLeft+this.controlWidth+tickMajorWidth+2, this.controlTop+this.controlHeight-1, stringFormat);
				
					if (labelString != null && labelString != "")
					{
						SizeF measuredSize = pe.Graphics.MeasureString(this.labelString.ToString(), this.Font);
						pe.Graphics.TranslateTransform(this.controlLeft-8, this.controlTop+(float)(this.controlHeight/2.0-1));
						pe.Graphics.RotateTransform(-90f);
						pe.Graphics.DrawString(this.labelString, this.Font, Brushes.Black, (float)(-measuredSize.Width/2.0), 0, stringFormat);
						pe.Graphics.TranslateTransform(-this.controlLeft+8, -this.controlTop-(float)(this.controlHeight/2.0-1));
						pe.Graphics.RotateTransform(90f);
					}
				}
				else
				{
					pe.Graphics.DrawLine(this.blackPen, this.controlLeft, this.controlTop, this.controlLeft, this.controlTop-this.tickMajorWidth);
					pe.Graphics.DrawLine(this.blackPen, this.controlLeft+this.controlWidth-1, this.controlTop, this.controlLeft+this.controlWidth-1, this.controlTop-this.tickMajorWidth);

					pe.Graphics.TranslateTransform(this.controlLeft, this.controlTop-this.tickMajorWidth-1);
					pe.Graphics.RotateTransform(-90f);

					pe.Graphics.DrawString(this.maxValue.ToString(this.numberFormat), this.Font, Brushes.Black, 0, 0, stringFormat);
					pe.Graphics.DrawString(this.minValue.ToString(this.numberFormat), this.Font, Brushes.Black, 0, this.controlWidth-1, stringFormat);
				
					pe.Graphics.RotateTransform(90f);
					pe.Graphics.TranslateTransform(-this.controlLeft, -this.controlTop+this.tickMajorWidth+2);

					if (labelString != null && labelString != "")
					{
						SizeF measuredSize = pe.Graphics.MeasureString(this.labelString.ToString(), this.Font);
						pe.Graphics.TranslateTransform(this.controlLeft+(float)(this.controlWidth/2.0-1), this.controlTop+this.controlHeight+6);
						pe.Graphics.RotateTransform(-180f);
						pe.Graphics.DrawString(this.labelString, this.Font, Brushes.Black, -(float)(measuredSize.Width/2.0), 0, stringFormat);
						pe.Graphics.RotateTransform(180f);
						pe.Graphics.TranslateTransform(-this.controlLeft-(float)(this.controlWidth/2.0-1), -this.controlTop-this.controlHeight-6);
						
					}
				}
			}

			// Calling the base class OnPaint
			base.OnPaint(pe);
		}

		#region Properties
		public bool ShowAxis
		{
			set
			{
				this.showAxis = value;
				this.Invalidate();
			}
		}

		public double Min
		{
			set
			{
				this.minValue = value;
				this.Invalidate();
			}
		}

		public double Max
		{
			set
			{
				this.maxValue = value;
				this.Invalidate();
			}
		}

		public string Label
		{
			set
			{
				this.labelString = value;
				this.Invalidate();
			}
		}

		public bool NormalOrientation
		{
			set
			{
				this.normalOrientation = value;
				this.Invalidate();
			}
		}

		public string NumberFormat
		{
			set
			{
				this.numberFormat = value;
				this.Invalidate();
			}
		}
		#endregion
	}
}
