using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace RunfileEditor
{
    public partial class frmChild : Form
    {
        public frmChild(RunfileEditor.frmContainer parent)
        {
            InitializeComponent();
            //Set the parent of the form to the container
            //this.MdiParent = parent;

        }

        public TextBox txtErrorMsgs;




    }
}