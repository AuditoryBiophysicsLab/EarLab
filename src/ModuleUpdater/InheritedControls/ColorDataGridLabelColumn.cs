using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.InheritedControls
{
	/// <summary>
	/// This class creates a DataGridColumnStyle which inherits from DataGridTextBoxColumn and acts like the cell contains a label instead
	/// of a text box.  Edit and selection functionality is not available.
	/// </summary>
	public class ColorDataGridLabelColumn : DataGridTextBoxColumn
	{
		public static ArrayList alternatingItemList;
		private static bool initializedList;
				
		public ColorDataGridLabelColumn()
		{
			if( !initializedList && alternatingItemList == null)
			{
				alternatingItemList = new ArrayList();
				initializedList = true;
			}
		}

		// You Paint function

		/*
		 * protected override void Paint(Graphics g, Rectangle bounds, CurrencyManager source, int rowNum)
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
		
		protected override void Paint(Graphics g, Rectangle bounds, CurrencyManager source, int rowNum, Brush backColor, Brush foreColor, bool alignToRight)
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
				base.Paint(g, bounds, source, rowNum, backColor, foreColor, alignToRight);

		}

		/// <summary>
		/// This function overrides the DataGridTextBoxColumn column's Edit function, and doesn't call the base class Edit function, thereby
		/// eliminating the Edit functionality of this column style.
		/// </summary>
		/// <param name="source"></param>
		/// <param name="rowNum"></param>
		/// <param name="bounds"></param>
		/// <param name="readOnly"></param>
		/// <param name="instantText"></param>
		/// <param name="cellIsVisible"></param>
		/// 
		protected override void Edit(CurrencyManager source, int rowNum, Rectangle bounds, bool readOnly,
			string instantText, bool cellIsVisible)
		{
		}
	}
}
