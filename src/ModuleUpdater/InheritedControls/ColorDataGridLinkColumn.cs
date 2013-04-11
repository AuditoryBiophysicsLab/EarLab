using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.InheritedControls
{
	/// <summary>
	/// This class creates a DataGridColumnStyle column which inherits from the DataGridTextBoxColumn, and contains special functionality to
	/// create the appearance of link labels instead of text boxes in the column.
	/// </summary>
	public class ColorDataGridLinkColumn : DataGridTextBoxColumn
	{
		int redRow;
		public static ArrayList alternatingItemList;
		private static bool initializedList;

		/// <summary>
		/// This function initializes the DataGridLinkColumn.
		/// </summary>
		public ColorDataGridLinkColumn()
		{
			if( !initializedList && alternatingItemList == null)
			{
				alternatingItemList = new ArrayList();
				initializedList = true;
			}
			redRow = -1;
		}

		/// <summary>
		/// This function overrides the DataGridTextBoxColumn Edit function, and doesn't call the base class.  Therefore, the Edit 
		/// functionality is not available for this DataGridColumnStyle.
		/// </summary>
		/// <param name="source"></param>
		/// <param name="rowNum"></param>
		/// <param name="bounds"></param>
		/// <param name="readOnly"></param>
		/// <param name="instantText"></param>
		/// <param name="cellIsVisible"></param>
		protected override void Edit(CurrencyManager source,
			int rowNum,
			Rectangle bounds,
			bool readOnly,
			string instantText,
			bool cellIsVisible
			)
		{
		}

		/// <summary>
		/// This function overrides the DataGridTextBoxColumn Paint function, and paints text so that it appears like a link label.
		/// </summary>
		/// <param name="g"></param>
		/// <param name="bounds"></param>
		/// <param name="source"></param>
		/// <param name="rowNum"></param>
		/// <param name="backBrush"></param>
		/// <param name="foreBrush"></param>
		/// <param name="alignToRight"></param>
		/// 
/*
		protected override void Paint(Graphics g, Rectangle bounds, CurrencyManager source, int rowNum)
		{
			this.Paint(g, bounds, source, rowNum, false);
		}

		protected override void Paint(Graphics g, Rectangle bounds, CurrencyManager source, int rowNum, bool alignToRight)
		{
			if( rowNum < alternatingItemList.Count )
			{
				if( (bool)alternatingItemList[rowNum] )
				{
					base.Paint(g, bounds, source, rowNum, new SolidBrush(this.DataGridTableStyle.AlternatingBackColor), new SolidBrush(this.DataGridTableStyle.ForeColor), alignToRight);
				}
				else
				{
					base.Paint(g, bounds, source, rowNum, new SolidBrush(this.DataGridTableStyle.BackColor), new SolidBrush(this.DataGridTableStyle.ForeColor), alignToRight);
				}
			}
			else
				base.Paint(g, bounds, source, rowNum, new SolidBrush(this.DataGridTableStyle.BackColor), new SolidBrush(this.DataGridTableStyle.ForeColor), alignToRight);

		}

*/

		protected override void Paint(System.Drawing.Graphics g,
			System.Drawing.Rectangle bounds, System.Windows.Forms.CurrencyManager
			source, int rowNum, System.Drawing.Brush backBrush, System.Drawing.Brush
			foreBrush, bool alignToRight)
		{
			// in order to center the text we create a string format object and use it
			if( rowNum < alternatingItemList.Count )
			{
				if( (bool)alternatingItemList[rowNum] )
				{
					backBrush = new SolidBrush(this.DataGridTableStyle.AlternatingBackColor);
				}
				else
				{
					backBrush = new SolidBrush(this.DataGridTableStyle.BackColor);
				}
			}
			
			StringFormat myFormat = new StringFormat();
			myFormat.Alignment = StringAlignment.Center;
			myFormat.LineAlignment = StringAlignment.Center;

			// we use the link color from the enclosing datagrid to determine our link color, though the active link color will be
			// hardcoded to be red
			SolidBrush linkBrush = new SolidBrush(this.DataGridTableStyle.DataGrid.LinkColor);

			// we fill in the background with background color just in case, erasing everything
			g.FillRectangle(backBrush, bounds.X, bounds.Y, bounds.Width, bounds.Height);

			string displayText;
			if (this.GetColumnValueAtRow(source, rowNum).ToString() == "")
				displayText = this.NullText;
			else
				displayText = this.GetColumnValueAtRow(source, rowNum).ToString();

			if (rowNum == redRow)
			{
				g.DrawString(displayText, this.DataGridTableStyle.DataGrid.Font, Brushes.Red,
					bounds.X + bounds.Width/2, bounds.Y + bounds.Height/2, myFormat);
				redRow = -1;
			}
			else
				g.DrawString(displayText, this.DataGridTableStyle.DataGrid.Font, linkBrush,
					bounds.X + bounds.Width/2, bounds.Y + bounds.Height/2, myFormat);
		}

		/// <summary>
		/// This function recieves a row index from a ViewDeleteDataGrid, which this class uses to activate either the 'View' or 'Delete'
		/// cell in that row of the ViewDeleteDataGrid.
		/// </summary>
		/// <param name="theRow"></param>
		public void setBrush(int theRow)
		{
			redRow = theRow;
		}
	}
}
