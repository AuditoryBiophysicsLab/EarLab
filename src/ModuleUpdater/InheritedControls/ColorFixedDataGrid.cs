using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.InheritedControls
{
	/// <summary>
	/// This class inherits from the DataGrid class, and creates a specialized datagrid.  This DataGrid has fixed column sizes which the user
	/// cannot resize, and when used in conjuction with DataGridLabelColumn columns, the user cannot select items as well.
	/// </summary>
	public class ColorFixedDataGrid : DataGrid
	{
		private int expandColumn = 0;

		/// <summary>
		/// This function initializes the FixedDataGrid.
		/// </summary>
		public ColorFixedDataGrid()
		{
			this.VisibleChanged += new EventHandler(ColorFixedDataGrid_VisibleChanged);
			this.VertScrollBar.VisibleChanged += new EventHandler(VertScrollBar_VisibleChanged);
			this.HorizScrollBar.VisibleChanged += new EventHandler(HorizScrollBar_VisibleChanged);
		}

		/// <summary>
		/// This function overrides the DataGrid OnMouseDown function, keeping the user from resizing the columns.
		/// </summary>
		/// <param name="e"></param>
		protected override void OnMouseDown(System.Windows.Forms.MouseEventArgs e)
		{
			if(this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.ColumnResize)
				return;
			base.OnMouseDown(e);
		}

		/// <summary>
		/// This function overrides the DataGrid OnMouseUp function, keeping the user from resizing the columns.
		/// </summary>
		/// <param name="e"></param>
		protected override void OnMouseUp(System.Windows.Forms.MouseEventArgs e)
		{
			if(this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.ColumnResize)
				return;
			base.OnMouseUp(e);
		}

		/// <summary>
		/// This function overrides the DataGrid OnMouseMove function, keeping the user from resizing the columns.
		/// </summary>
		/// <param name="e"></param>
		protected override void OnMouseMove(System.Windows.Forms.MouseEventArgs e)
		{
			if (this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.ColumnResize)
				return;
			base.OnMouseMove(e);
		}

		protected override void OnResize(EventArgs e)
		{
			base.OnResize (e);
			this.ResizeColumn();
			base.OnResize (e);
			this.Refresh();
		}

		private void ResizeColumn()
		{
			if (this.TableStyles.Count > 0 && this.DataSource != null)
			{
				int columns = this.TableStyles[0].GridColumnStyles.Count;
				int width = 0;
				for (int i=0; i<columns; i++)
				{
					if (i != this.expandColumn)
						width += this.TableStyles[0].GridColumnStyles[i].Width;
				}

				if (this.VertScrollBar.Visible)
					width += SystemInformation.VerticalScrollBarWidth;

				this.TableStyles[0].GridColumnStyles[this.expandColumn].Width = this.TableStyles[0].DataGrid.ClientSize.Width - width - 5;
			}
		}

		private void VertScrollBar_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}

		protected override void OnDataSourceChanged(EventArgs e)
		{
			this.OnResize(new EventArgs());
		}


		private void ColorFixedDataGrid_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}

		private void HorizScrollBar_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}
	}
}
