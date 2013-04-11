using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.InheritedControls
{
	/// <summary>
	/// This class creates a DataGridColumnStyle column which inherits from the DataGridTextBoxColumn, and contains special functionality to
	/// create the appearance of link labels instead of text boxes in the column.
	/// </summary>
	public class DataGridLinkColumn : DataGridTextBoxColumn
	{
		private int activeRow;
		private System.Drawing.Brush textBrush;

		/// <summary>
		/// This function initializes the DataGridLinkColumn.
		/// </summary>
		public DataGridLinkColumn()
		{
			this.activeRow = -1;
		}

		/// <summary>
		/// This function is overriden so that the column is a label, not an editable text box.
		/// </summary>
		protected override void Edit(CurrencyManager source, int rowNum, Rectangle bounds, bool readOnly, string instantText, bool cellIsVisible){}

		/// <summary>
		/// This function is overriden so we can draw the link colors depending on whether the link is active or not.
		/// </summary>
		protected override void Paint(System.Drawing.Graphics g, System.Drawing.Rectangle bounds, System.Windows.Forms.CurrencyManager source,
			int rowNum, System.Drawing.Brush backBrush, System.Drawing.Brush foreBrush, bool alignToRight)
		{
			if (rowNum == this.activeRow)
				this.textBrush = Brushes.Red;
			else
				this.textBrush = new SolidBrush(this.DataGridTableStyle.LinkColor);

			base.Paint(g, bounds, source, rowNum, backBrush, this.textBrush, alignToRight);
		}

		/// <summary>
		/// Gets or sets the active link row in the column.
		/// </summary>
		public int ActiveRow
		{
			set { this.activeRow = value; }
			get { return this.activeRow; }
		}
	}
}
