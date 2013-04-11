using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.InheritedControls
{
	/// <summary>
	/// This class creates a DataGridColumnStyle which inherits from DataGridTextBoxColumn and acts like the cell contains a label instead
	/// of a text box.  Edit and selection functionality is not available.
	/// </summary>
	public class DataGridLabelColumn : DataGridTextBoxColumn
	{		
		public DataGridLabelColumn(){}

/*		protected override void Paint(Graphics g, Rectangle bounds, CurrencyManager source, int rowNum, Brush backColor, Brush foreColor, bool alignToRight)
		{
			if ((rowNum & 1) == 1)
				base.Paint(g, bounds, source, rowNum, new SolidBrush(this.DataGridTableStyle.BackColor), new SolidBrush(this.DataGridTableStyle.ForeColor), alignToRight);
			else
				base.Paint(g, bounds, source, rowNum, new SolidBrush(this.DataGridTableStyle.AlternatingBackColor), new SolidBrush(this.DataGridTableStyle.ForeColor), alignToRight);
		}*/

		protected override void Edit(CurrencyManager source, int rowNum, Rectangle bounds, bool readOnly,
			string instantText, bool cellIsVisible)
		{
		}
	}
}
