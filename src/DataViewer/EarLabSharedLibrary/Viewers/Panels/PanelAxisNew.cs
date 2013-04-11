using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.Viewers.Panels
{
	/// <summary>
	/// Summary description for PanelAxisNew.
	/// </summary>
	public class PanelAxisNew : System.Windows.Forms.Panel
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private Pen blackPen;

		private Control topAxisControl = null;
		private bool topAxisShow = false;
		private Point topAxisStartPoint = Point.Empty;
		private Point topAxisEndPoint = Point.Empty;
		private float topAxisStartValue = 0;
		private float topAxisEndValue = 0;
		private int topAxisMajorTickHeight = 3;
		private int topAxisMajorTickOffset = 2;
		private int topAxisMajorTickNumbersSpacing = 10;
		private string topAxisMajorTickNumbersFormat = "0";
		private bool topAxisMajorTickNumbersShow = true;
		private string topAxisLabel = "Axis Label Not Set";
		private bool topAxisLabelShow = true;

		private Control bottomAxisControl = null;
		private bool bottomAxisShow = false;
		private Point bottomAxisStartPoint = Point.Empty;
		private Point bottomAxisEndPoint = Point.Empty;
		private float bottomAxisStartValue = 0;
		private float bottomAxisEndValue = 0;
		private double[] bottomAxisValueArray = null;
		private int bottomAxisMajorTickHeight = 3;
		private int bottomAxisMajorTickOffset = 2;
		private int bottomAxisMajorTickNumbersSpacing = 10;
		private string bottomAxisMajorTickNumbersFormat = "0";
		private bool bottomAxisMajorTickNumbersShow = true;
		private string bottomAxisLabel = "Axis Label Not Set";
		private bool bottomAxisLabelShow = true;

		private Control leftAxisControl = null;
		private bool leftAxisShow = false;
		private Point leftAxisStartPoint = Point.Empty;
		private Point leftAxisEndPoint = Point.Empty;
		private float leftAxisStartValue = 0;
		private float leftAxisEndValue = 0;
		private double[] leftAxisValueArray = null;
		private int leftAxisMajorTickHeight = 3;
		private int leftAxisMajorTickOffset = 2;
		private int leftAxisMajorTickNumbersSpacing = 10;
		private string leftAxisMajorTickNumbersFormat = "0";
		private bool leftAxisMajorTickNumbersShow = true;
		private string leftAxisLabel = "Axis Label Not Set";
		private bool leftAxisLabelShow = true;

		private Control rightAxisControl = null;
		private bool rightAxisShow = false;
		private Point rightAxisStartPoint = Point.Empty;
		private Point rightAxisEndPoint = Point.Empty;
		private float rightAxisStartValue = 0;
		private float rightAxisEndValue = 0;
		private int rightAxisMajorTickHeight = 3;
		private int rightAxisMajorTickOffset = 2;
		private int rightAxisMajorTickNumbersSpacing = 10;
		private string rightAxisMajorTickNumbersFormat = "0";
		private bool rightAxisMajorTickNumbersShow = true;
		private string rightAxisLabel = "Axis Label Not Set";
		private bool rightAxisLabelShow = true;

		private StringFormat stringFormat = new StringFormat();
		SizeF measuredSize;

		public PanelAxisNew()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// We set up double buffering so that there isn't too much flicker (yeah, this is brilliant)
			this.SetStyle(ControlStyles.UserPaint|ControlStyles.AllPaintingInWmPaint|ControlStyles.DoubleBuffer, true); 
			this.UpdateStyles();

			blackPen = new Pen(Color.Black, 0);
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
			// PanelAxis
			// 
			this.Font = new System.Drawing.Font("Arial", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Pixel, ((System.Byte)(0)));

		}
		#endregion

		protected override void OnPaint(PaintEventArgs pe)
		{
			#region Top Axis Code
			if (this.topAxisShow && this.topAxisControl != null)
			{
				this.topAxisStartPoint = new Point(this.topAxisControl.Left-this.Left, this.topAxisControl.Top-this.Top);
				this.topAxisEndPoint = new Point(this.topAxisControl.Right-1-this.Left, this.topAxisControl.Top-this.Top);

				// transform origin so we don't have to add these numbers all the time
				pe.Graphics.TranslateTransform(this.topAxisStartPoint.X, this.topAxisStartPoint.Y);

				int width = Math.Abs(this.topAxisEndPoint.X - this.topAxisStartPoint.X);

				this.stringFormat.Alignment = StringAlignment.Center;
				this.stringFormat.LineAlignment = StringAlignment.Far;

				// draw the axis label
				if (this.topAxisLabelShow)
					pe.Graphics.DrawString(this.topAxisLabel, this.Font, Brushes.Black, width/2f, -this.topAxisMajorTickOffset-1, this.stringFormat);

				// put the origin back to normal spot for other axis elements
				pe.Graphics.TranslateTransform(-this.topAxisStartPoint.X, -this.topAxisStartPoint.Y);
			}
			#endregion

			#region Bottom Axis Code
			if (this.bottomAxisShow && this.bottomAxisControl != null && this.BottomAxisStartValue != this.BottomAxisEndValue)
			{
				this.bottomAxisStartPoint = new Point(this.bottomAxisControl.Left-this.Left, this.bottomAxisControl.Bottom-this.Top);
				this.bottomAxisEndPoint = new Point(this.bottomAxisControl.Right-1-this.Left, this.bottomAxisControl.Bottom-this.Top);

				// transform origin so we don't have to add these numbers all the time
				pe.Graphics.TranslateTransform(this.bottomAxisStartPoint.X, this.bottomAxisStartPoint.Y);

				// calculate the number of ticks to have so that the numbers don't overlap
				if (this.bottomAxisValueArray == null)
					this.measuredSize = pe.Graphics.MeasureString(this.bottomAxisEndValue.ToString(this.bottomAxisMajorTickNumbersFormat), this.Font);
				else
					this.measuredSize = pe.Graphics.MeasureString(this.bottomAxisValueArray[(int)this.bottomAxisEndValue].ToString(this.bottomAxisMajorTickNumbersFormat), this.Font);
				int width = Math.Abs(this.bottomAxisEndPoint.X - this.bottomAxisStartPoint.X);
				double ratio = Math.Abs(this.bottomAxisStartValue - this.bottomAxisEndValue) / (double)width;
				double stepSize = width / Math.Ceiling(width / (this.measuredSize.Width + this.bottomAxisMajorTickNumbersSpacing));

				// draw the major tick marks
				for (double i=0;i<width+1;i+=stepSize)
					pe.Graphics.DrawLine(blackPen, (float)i, 0f, (float)i, (float)this.bottomAxisMajorTickHeight);

				// draw the major tick mark values (we add one since we need to round up if we are 'close enough')
				if (this.bottomAxisMajorTickNumbersShow)
				{
					this.stringFormat.Alignment = StringAlignment.Center;
					this.stringFormat.LineAlignment = StringAlignment.Near;
					for (double i=0;i<width+1;i+=stepSize)
					{
						if (this.bottomAxisValueArray == null)
							pe.Graphics.DrawString((this.bottomAxisStartValue+i*ratio).ToString(this.bottomAxisMajorTickNumbersFormat), this.Font, Brushes.Black, (float)i, (float)(this.bottomAxisMajorTickHeight+this.bottomAxisMajorTickOffset), this.stringFormat);
						else
							pe.Graphics.DrawString(this.bottomAxisValueArray[(int)Math.Round(this.bottomAxisStartValue+i*ratio)].ToString(this.bottomAxisMajorTickNumbersFormat), this.Font, Brushes.Black, (float)i, (float)(this.bottomAxisMajorTickHeight+this.bottomAxisMajorTickOffset), this.stringFormat);
					}
				}

				// draw the axis label
				if (this.bottomAxisLabelShow)
					pe.Graphics.DrawString(this.bottomAxisLabel, this.Font, Brushes.Black, width/2f, this.bottomAxisMajorTickHeight+this.Font.Height+1, this.stringFormat);

				// put the origin back to normal spot for other axis elements
				pe.Graphics.TranslateTransform(-this.bottomAxisStartPoint.X, -this.bottomAxisStartPoint.Y);
			}
			#endregion

			#region Left Axis Code
			if (this.leftAxisShow && this.leftAxisControl != null && this.leftAxisStartValue != this.leftAxisEndValue)
			{
				this.leftAxisStartPoint = new Point(this.leftAxisControl.Left-1-this.Left, this.leftAxisControl.Bottom-1-this.Top);
				this.leftAxisEndPoint = new Point(this.leftAxisControl.Left-1-this.Left, this.leftAxisControl.Top-this.Top);

				// transform origin so we don't have to add these numbers all the time
				pe.Graphics.TranslateTransform(this.leftAxisStartPoint.X, this.leftAxisStartPoint.Y);

				// calculate the number of ticks to have so that the numbers don't overlap
				int height = Math.Abs(this.leftAxisEndPoint.Y - this.leftAxisStartPoint.Y);
				double stepSize = height / Math.Ceiling((double)height / (this.Font.Height + this.leftAxisMajorTickNumbersSpacing));

				// draw the major tick marks
				for (double i=0;i>-height-1;i-=stepSize)
					pe.Graphics.DrawLine(blackPen, 0f, (float)i, (float)-this.leftAxisMajorTickHeight, (float)i);

				// draw the major tick mark values (we add one since we need to round up if we are 'close enough')
				if (this.leftAxisMajorTickNumbersShow)
				{
					this.stringFormat.Alignment = StringAlignment.Far;
					this.stringFormat.LineAlignment = StringAlignment.Center;
					for (double i=0;i>-height-1;i-=stepSize)
					{
						if (this.leftAxisValueArray == null)
							pe.Graphics.DrawString((this.leftAxisStartValue+this.leftAxisEndValue*(-i/height)).ToString(this.leftAxisMajorTickNumbersFormat), this.Font, Brushes.Black, (float)(-this.leftAxisMajorTickHeight-this.leftAxisMajorTickOffset), (float)i, this.stringFormat);
						else
							pe.Graphics.DrawString(this.leftAxisValueArray[(int)Math.Round(this.leftAxisStartValue+this.leftAxisEndValue*(-i/height))].ToString(this.leftAxisMajorTickNumbersFormat), this.Font, Brushes.Black, (float)(-this.leftAxisMajorTickHeight-this.leftAxisMajorTickOffset), (float)i, this.stringFormat);
					}
				}

				// draw the axis label
				if (this.leftAxisLabelShow)
				{
					this.stringFormat.LineAlignment = StringAlignment.Far;
					this.stringFormat.Alignment = StringAlignment.Center;
					pe.Graphics.RotateTransform(-90f);

					if (this.leftAxisValueArray == null)
						this.measuredSize = pe.Graphics.MeasureString(this.leftAxisEndValue.ToString(this.leftAxisMajorTickNumbersFormat), this.Font);
					else
						this.measuredSize = pe.Graphics.MeasureString(this.leftAxisValueArray[(int)this.leftAxisEndValue].ToString(this.leftAxisMajorTickNumbersFormat), this.Font);
					pe.Graphics.DrawString(this.leftAxisLabel, this.Font, Brushes.Black, height/2f, -this.leftAxisMajorTickHeight-this.leftAxisMajorTickOffset-this.measuredSize.Width-1, this.stringFormat);
					pe.Graphics.RotateTransform(90f);
				}

				// put the origin back to normal spot for other axis elements
				pe.Graphics.TranslateTransform(-this.leftAxisStartPoint.X, -this.leftAxisStartPoint.Y);
			}
			#endregion

			#region Right Axis Code
			if (this.rightAxisShow && this.rightAxisControl != null && this.rightAxisStartValue != this.rightAxisEndValue)
			{
				this.rightAxisStartPoint = new Point(this.rightAxisControl.Right-this.Left, this.rightAxisControl.Bottom-1-this.Top);
				this.rightAxisEndPoint = new Point(this.rightAxisControl.Right-this.Left, this.rightAxisControl.Top-this.Top);

				// transform origin so we don't have to add these numbers all the time
				pe.Graphics.TranslateTransform(this.rightAxisStartPoint.X, this.rightAxisStartPoint.Y);

				// calculate the number of ticks to have so that the numbers don't overlap
				int height = Math.Abs(this.rightAxisEndPoint.Y - this.rightAxisStartPoint.Y);
				double ratio = Math.Abs(this.rightAxisStartValue - this.rightAxisEndValue) / (double)height;
				double stepSize = height / Math.Ceiling((double)height / (this.Font.Height + this.rightAxisMajorTickNumbersSpacing));

				// draw the major tick marks
				for (double i=0;i>-height-1;i-=stepSize)
					pe.Graphics.DrawLine(blackPen, 0f, (float)i, (float)this.rightAxisMajorTickHeight, (float)i);

				// draw the major tick mark values (we add one since we need to round up if we are 'close enough')
				if (this.rightAxisMajorTickNumbersShow)
				{
					this.stringFormat.Alignment = StringAlignment.Near;
					this.stringFormat.LineAlignment = StringAlignment.Center;
					for (double i=0;i>-height-1;i-=stepSize)
						pe.Graphics.DrawString((this.rightAxisStartValue+(-i*ratio)).ToString(this.rightAxisMajorTickNumbersFormat), this.Font, Brushes.Black, (float)(this.rightAxisMajorTickHeight+this.rightAxisMajorTickOffset), (float)i, this.stringFormat);
				}

				// draw the axis label
				if (this.rightAxisLabelShow)
				{
					stringFormat.LineAlignment = StringAlignment.Near;
					stringFormat.Alignment = StringAlignment.Center;
					pe.Graphics.RotateTransform(-90f);

					this.measuredSize = pe.Graphics.MeasureString(this.rightAxisEndValue.ToString(this.rightAxisMajorTickNumbersFormat), this.Font);
					SizeF tempSize = pe.Graphics.MeasureString(this.rightAxisStartValue.ToString(this.rightAxisMajorTickNumbersFormat), this.Font);
					if (tempSize.Width > this.measuredSize.Width)
						this.measuredSize = tempSize;

					pe.Graphics.DrawString(this.rightAxisLabel, this.Font, Brushes.Black, height/2f, this.rightAxisMajorTickHeight+this.rightAxisMajorTickOffset+this.measuredSize.Width+1, stringFormat);

					// rotate origin back to normal spot for other axis elements
					pe.Graphics.RotateTransform(90f);
				}

				pe.Graphics.TranslateTransform(-this.rightAxisStartPoint.X, -this.rightAxisStartPoint.Y);
			}
			#endregion

			// Calling the base class OnPaint
			base.OnPaint(pe);
		}

		protected override void OnResize(EventArgs e)
		{
			// we need to invalidate so all the axes elements are redrawn/centered
			this.Invalidate();

			// Calling the base class OnResize
			base.OnResize (e);
		}

		
		#region Top Axis
		public Control TopAxisControl
		{
			set
			{
				this.topAxisControl = value;
			}
		}

		public bool TopAxisShow
		{
			set
			{
				this.topAxisShow = value;
			}
			get
			{
				return this.topAxisShow;
			}
		}

		public float TopAxisStartValue
		{
			set
			{
				this.topAxisStartValue = value;
			}
			get
			{
				return this.topAxisStartValue;
			}
		}

		public float TopAxisEndValue
		{
			set
			{
				this.topAxisEndValue = value;
			}
			get
			{
				return this.topAxisEndValue;
			}
		}

		public int TopAxisMajorTickHeight
		{
			set
			{
				this.topAxisMajorTickHeight = value;
			}
			get
			{
				return this.topAxisMajorTickHeight;
			}
		}

		public int TopAxisMajorTickOffset
		{
			set
			{
				this.topAxisMajorTickOffset = value;
			}
			get
			{
				return this.topAxisMajorTickOffset;
			}
		}

		public int TopAxisMajorTickNumbersSpacing
		{
			set
			{
				this.topAxisMajorTickNumbersSpacing = value;
			}
			get
			{
				return this.topAxisMajorTickNumbersSpacing;
			}
		}

		public string TopAxisMajorTickNumbersFormat
		{
			set
			{
				this.topAxisMajorTickNumbersFormat = value;
			}
			get
			{
				return this.topAxisMajorTickNumbersFormat;
			}
		}

		public bool TopAxisMajorTickNumbersShow
		{
			set
			{
				this.topAxisMajorTickNumbersShow = value;
			}
			get
			{
				return this.topAxisMajorTickNumbersShow;
			}
		}

		public string TopAxisLabel
		{
			set
			{
				this.topAxisLabel = value;
			}
			get
			{
				return this.topAxisLabel;
			}
		}

		public bool TopAxisLabelShow
		{
			set
			{
				this.topAxisLabelShow = value;
			}
			get
			{
				return this.topAxisLabelShow;
			}
		}
		#endregion

		#region Bottom Axis
		public Control BottomAxisControl
		{
			set
			{
				this.bottomAxisControl = value;
			}
		}

		public bool BottomAxisShow
		{
			set
			{
				this.bottomAxisShow = value;
			}
			get
			{
				return this.bottomAxisShow;
			}
		}

		public float BottomAxisStartValue
		{
			set
			{
				this.bottomAxisStartValue = value;
			}
			get
			{
				return this.bottomAxisStartValue;
			}
		}

		public float BottomAxisEndValue
		{
			set
			{
				this.bottomAxisEndValue = value;
			}
			get
			{
				return this.bottomAxisEndValue;
			}
		}

		public double[] BottomAxisValueArray
		{
			set { this.bottomAxisValueArray = value; }
		}

		public int BottomAxisMajorTickHeight
		{
			set
			{
				this.bottomAxisMajorTickHeight = value;
			}
			get
			{
				return this.bottomAxisMajorTickHeight;
			}
		}

		public int BottomAxisMajorTickOffset
		{
			set
			{
				this.bottomAxisMajorTickOffset = value;
			}
			get
			{
				return this.bottomAxisMajorTickOffset;
			}
		}

		public int BottomAxisMajorTickNumbersSpacing
		{
			set
			{
				this.bottomAxisMajorTickNumbersSpacing = value;
			}
			get
			{
				return this.bottomAxisMajorTickNumbersSpacing;
			}
		}

		public string BottomAxisMajorTickNumbersFormat
		{
			set
			{
				this.bottomAxisMajorTickNumbersFormat = value;
			}
			get
			{
				return this.bottomAxisMajorTickNumbersFormat;
			}
		}

		public bool BottomAxisMajorTickNumbersShow
		{
			set
			{
				this.bottomAxisMajorTickNumbersShow = value;
			}
			get
			{
				return this.bottomAxisMajorTickNumbersShow;
			}
		}

		public string BottomAxisLabel
		{
			set
			{
				this.bottomAxisLabel = value;
			}
			get
			{
				return this.bottomAxisLabel;
			}
		}

		public bool BottomAxisLabelShow
		{
			set
			{
				this.bottomAxisLabelShow = value;
			}
			get
			{
				return this.bottomAxisLabelShow;
			}
		}
		#endregion

		#region Left Axis
		public Control LeftAxisControl
		{
			set
			{
				this.leftAxisControl = value;
			}
		}

		public bool LeftAxisShow
		{
			set
			{
				this.leftAxisShow = value;
			}
			get
			{
				return this.leftAxisShow;
			}
		}

		public float LeftAxisStartValue
		{
			set
			{
				this.leftAxisStartValue = value;
			}
			get
			{
				return this.leftAxisStartValue;
			}
		}

		public float LeftAxisEndValue
		{
			set
			{
				this.leftAxisEndValue = value;
			}
			get
			{
				return this.leftAxisEndValue;
			}
		}

		public double[] LeftAxisValueArray
		{
			set { this.leftAxisValueArray = value; }
		}

		public int LeftAxisMajorTickHeight
		{
			set
			{
				this.leftAxisMajorTickHeight = value;
			}
			get
			{
				return this.leftAxisMajorTickHeight;
			}
		}

		public int LeftAxisMajorTickOffset
		{
			set
			{
				this.leftAxisMajorTickOffset = value;
			}
			get
			{
				return this.leftAxisMajorTickOffset;
			}
		}

		public int LeftAxisMajorTickNumbersSpacing
		{
			set
			{
				this.leftAxisMajorTickNumbersSpacing = value;
			}
			get
			{
				return this.leftAxisMajorTickNumbersSpacing;
			}
		}

		public string LeftAxisMajorTickNumbersFormat
		{
			set
			{
				this.leftAxisMajorTickNumbersFormat = value;
			}
			get
			{
				return this.leftAxisMajorTickNumbersFormat;
			}
		}

		public bool LeftAxisMajorTickNumbersShow
		{
			set
			{
				this.leftAxisMajorTickNumbersShow = value;
			}
			get
			{
				return this.leftAxisMajorTickNumbersShow;
			}
		}

		public string LeftAxisLabel
		{
			set
			{
				this.leftAxisLabel = value;
			}
			get
			{
				return this.leftAxisLabel;
			}
		}

		public bool LeftAxisLabelShow
		{
			set
			{
				this.leftAxisLabelShow = value;
			}
			get
			{
				return this.leftAxisLabelShow;
			}
		}
		#endregion

		#region Right Axis
		public Control RightAxisControl
		{
			set
			{
				this.rightAxisControl = value;
			}
		}

		public bool RightAxisShow
		{
			set
			{
				this.rightAxisShow = value;
			}
			get
			{
				return this.rightAxisShow;
			}
		}

		public float RightAxisStartValue
		{
			set
			{
				this.rightAxisStartValue = value;
			}
			get
			{
				return this.rightAxisStartValue;
			}
		}

		public float RightAxisEndValue
		{
			set
			{
				this.rightAxisEndValue = value;
			}
			get
			{
				return this.rightAxisEndValue;
			}
		}

		public int RightAxisMajorTickHeight
		{
			set
			{
				this.rightAxisMajorTickHeight = value;
			}
			get
			{
				return this.rightAxisMajorTickHeight;
			}
		}

		public int RightAxisMajorTickOffset
		{
			set
			{
				this.rightAxisMajorTickOffset = value;
			}
			get
			{
				return this.rightAxisMajorTickOffset;
			}
		}

		public int RightAxisMajorTickNumbersSpacing
		{
			set
			{
				this.rightAxisMajorTickNumbersSpacing = value;
			}
			get
			{
				return this.rightAxisMajorTickNumbersSpacing;
			}
		}

		public string RightAxisMajorTickNumbersFormat
		{
			set
			{
				this.rightAxisMajorTickNumbersFormat = value;
			}
			get
			{
				return this.rightAxisMajorTickNumbersFormat;
			}
		}

		public bool RightAxisMajorTickNumbersShow
		{
			set
			{
				this.rightAxisMajorTickNumbersShow = value;
			}
			get
			{
				return this.rightAxisMajorTickNumbersShow;
			}
		}

		public string RightAxisLabel
		{
			set
			{
				this.rightAxisLabel = value;
			}
			get
			{
				return this.rightAxisLabel;
			}
		}

		public bool RightAxisLabelShow
		{
			set
			{
				this.rightAxisLabelShow = value;
			}
			get
			{
				return this.rightAxisLabelShow;
			}
		}
		#endregion
	}
}
