using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SPG
{
    public partial class Form1 : Form
    {
        private MyPropertyGrid myPropertyGrid1;

        public Form1()
        {
            InitializeComponent();

            // Construct the object
            myPropertyGrid1 = new MyPropertyGrid();
            // Add the control to the collection of controls that are 'on' this form
            this.Controls.Add(myPropertyGrid1);
            // Initialize the new control
            myPropertyGrid1.Initialize();
            // Tell the form that the new control is at (10, 10)
            myPropertyGrid1.Location = new Point(10, 10);
            // Tell the form that the size of the new control is the same as the form's client size less 20 pixels horizontally and vertically
            myPropertyGrid1.Size = new Size(this.ClientSize.Width - 20, this.ClientSize.Height - 20);
            // Anchor the control to the Top, Left, Right and Bottom of the form so it automatically resizes with the form
            myPropertyGrid1.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Bottom;
        }
    }
}