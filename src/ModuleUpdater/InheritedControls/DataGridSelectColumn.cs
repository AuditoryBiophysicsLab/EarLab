using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.InheritedControls
{
	/// <summary>
	/// Summary description for DataGridLinkColumn.
	/// </summary>
	public class ColorDataGridSelectColumn : DataGridTextBoxColumn
	{
		public static ArrayList alternatingItemList;
		private static bool initializedList;

		public ColorDataGridSelectColumn()
		{
			if( !initializedList && alternatingItemList == null)
			{
				alternatingItemList = new ArrayList();
				initializedList = true;
			}
		}

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

		protected override void Edit(CurrencyManager source, int rowNum, Rectangle bounds, bool readOnly,
			string instantText, bool cellIsVisible)
		{
			this.DataGridTableStyle.DataGrid.Select(rowNum);
		}
	}
}
