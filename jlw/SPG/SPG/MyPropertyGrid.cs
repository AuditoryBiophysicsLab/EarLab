using System;
using System.Collections.Generic;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Windows.Forms;
using System.Reflection;
using System.Globalization;
using System.ComponentModel;
using System.Drawing;
using System.Resources;

namespace SPG
{
    public class MyPropertyGrid : VisualHint.SmartPropertyGrid.PropertyGrid
    {
        public MyPropertyGrid()
        {
        }

        public void Initialize()
        {
            PropertyEnumerator catEnum = AppendRootCategory(1, "Application");
            
            PropertyEnumerator propEnum = AppendManagedProperty(catEnum, 2, "AllowQuit",
                typeof(bool), Application.AllowQuit, "Help string", new ReadOnlyAttribute(true));
            
            AppendManagedProperty(catEnum, 3, "Use wait cursor", typeof(bool),
                Application.UseWaitCursor, "");
            
            catEnum = AppendRootCategory(4, "Form");
            
            AppendProperty(catEnum, 5, "Back color", ParentForm, "BackColor", "");
            
            propEnum = AppendProperty(catEnum, 6, "Size", ParentForm, "Size", "");
            ExpandProperty(propEnum, true);
        
            propEnum = InsertProperty(propEnum, 7, "Opacity", ParentForm, "Opacity", "");
        }
    }
}
