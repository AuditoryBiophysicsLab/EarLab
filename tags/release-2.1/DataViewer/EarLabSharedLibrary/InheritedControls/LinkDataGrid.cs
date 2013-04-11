using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace EarLab.InheritedControls
{
	public class LinkDataGrid : DataGrid
	{
		private int activeRow, activeColumn;
		private int activeStyle = 0;

		public LinkDataGrid()
		{
			this.VisibleChanged += new EventHandler(LinkDataGrid_VisibleChanged);
			this.VertScrollBar.VisibleChanged += new EventHandler(VertScrollBar_VisibleChanged);
			this.HorizScrollBar.VisibleChanged += new EventHandler(HorizScrollBar_VisibleChanged);
			this.activeRow = this.activeColumn = -1;
		}

		protected override void OnMouseDown(System.Windows.Forms.MouseEventArgs e)
		{
			int currentColumn = this.HitTest(e.X, e.Y).Column;
			int currentRow = this.HitTest(e.X, e.Y).Row;

			if (this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.ColumnHeader)
			{
				if (this.TableStyles[this.activeStyle].GridColumnStyles[currentColumn].GetType().Name == "DataGridLinkColumn")
					return;
			}		
			else if (this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.Cell)
			{
				if (this.TableStyles[this.activeStyle].GridColumnStyles[currentColumn].GetType().Name == "DataGridLinkColumn")
				{
					activeRow = this.HitTest(e.X, e.Y).Row;
					activeColumn = this.HitTest(e.X, e.Y).Column;

					((DataGridLinkColumn)this.TableStyles[this.activeStyle].GridColumnStyles[this.activeColumn]).ActiveRow = this.activeRow;
					this.Invalidate(this.GetCellBounds(this.activeRow, this.activeColumn), false);
					return;
				}
			}

			base.OnMouseDown(e);
		}

		public delegate void LinkClickedHandler(string columnName, DataRow row);
		public event LinkClickedHandler LinkClicked;

		protected override void OnMouseUp(System.Windows.Forms.MouseEventArgs e)
		{
			// if activeRow and activeColumn are both -1, than there is no active link so we don't have to do any painting
			if (this.activeRow > -1 && this.activeColumn > -1)
			{
				((DataGridLinkColumn)this.TableStyles[this.activeStyle].GridColumnStyles[this.activeColumn]).ActiveRow = -1;
				this.Invalidate(this.GetCellBounds(this.activeRow, this.activeColumn), false);		// invalidate the active link cell

				// check to see if the mouseup was in the same active row and column, then user wanted to click that link
				if (this.activeRow == this.HitTest(e.X, e.Y).Row && this.activeColumn == this.HitTest(e.X, e.Y).Column)
				{
					this.CurrentCell = new DataGridCell(this.activeRow, this.activeColumn);			// set current active cell

					// raise LinkLabelClicked event if anyone is listening
					if (this.LinkClicked != null)
						this.LinkClicked(this.TableStyles[this.activeStyle].GridColumnStyles[this.activeColumn].MappingName, ((DataRowView)this.BindingContext[this.DataSource, this.DataMember].Current).Row);
				}

				this.activeRow = this.activeColumn = -1;											// reset the values to show no active link
			}
			
			base.OnMouseUp(e);
			this.ResizeColumn();
		}

		/// <summary>
		/// This function overrides the DataGrid OnMouseMove function, allowing a 'Hand' cursor to be displayed when the cursor moves over the
		/// 'View' or 'Delete' DataGridLinkColumn columns.
		/// </summary>
		/// <param name="e"></param>
		protected override void OnMouseMove(System.Windows.Forms.MouseEventArgs e)
		{
			// if the click was on a cell, we check to see if it was either of the two link columns, and if so we change the current cursor to
			// a hand, otherwise we change it to the default cursor (if it was not so already)
			if(this.HitTest(e.X, e.Y).Type == DataGrid.HitTestType.Cell)
			{
				if (this.TableStyles[this.activeStyle].GridColumnStyles[this.HitTest(e.X, e.Y).Column].GetType().Name == "DataGridLinkColumn")
				{
					if (this.Cursor != Cursors.Hand)
						this.Cursor = Cursors.Hand;
				}
				else
				{
					if (this.Cursor != Cursors.Default)
						this.Cursor = Cursors.Default;
				}
				return;
			}
			else
			{
				if (this.Cursor != Cursors.Default)
					this.Cursor = Cursors.Default;
			}

			base.OnMouseMove(e);
		}

		/// <summary>
		/// This function overrides the DataGrid OnMouseLeave function, making sure that the cursor is returned to default.
		/// </summary>
		/// <param name="e"></param>
		protected override void OnMouseLeave(System.EventArgs e)
		{
			if (this.Cursor != Cursors.Default)
				this.Cursor = Cursors.Default;

			base.OnMouseLeave(e);
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
				int columns = this.TableStyles[this.activeStyle].GridColumnStyles.Count;
				int width = 0;
				for (int i=0; i<columns; i++)
					width += this.TableStyles[this.activeStyle].GridColumnStyles[i].Width;
				if (this.VertScrollBar.Visible)
					width += SystemInformation.VerticalScrollBarWidth;
				width += 5;		// little extra for borders and such.  I'm a freakin' cheater.

				if (this.TableStyles[this.activeStyle].DataGrid.ClientSize.Width - width != 0)
				{
					for (int i=columns-1; i>=0 ; i--)
					{
						if (this.TableStyles[this.activeStyle].GridColumnStyles[i].GetType().Name != "DataGridLinkColumn")
						{
							this.TableStyles[this.activeStyle].GridColumnStyles[i].Width += this.TableStyles[this.activeStyle].DataGrid.ClientSize.Width - width;
							break;
						}
					}
				}
			}
		}

		private void VertScrollBar_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}

		private void LinkDataGrid_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}

		private void HorizScrollBar_VisibleChanged(object sender, EventArgs e)
		{
			this.OnResize(new EventArgs());
		}

		protected override void OnDataSourceChanged(EventArgs e)
		{
			this.activeStyle = 0;
			for (int i=0;i<this.TableStyles.Count;i++)
			{
				if (this.DataSource.GetType().Name == "DataView" && this.TableStyles[i].MappingName == ((DataView)this.DataSource).Table.TableName)
					this.activeStyle = i;
				else if (this.DataSource.GetType().Name == "DataTable" && this.TableStyles[i].MappingName == ((DataTable)this.DataSource).TableName)
					this.activeStyle = i;
			}

			base.OnDataSourceChanged(e);
			this.OnResize(new EventArgs());
		}

	}
}