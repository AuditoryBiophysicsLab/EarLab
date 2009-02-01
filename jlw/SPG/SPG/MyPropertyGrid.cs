using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Windows.Forms;
using System.Globalization;
using System.ComponentModel;
using System.Drawing;
using System.Resources;
using System.Reflection;

namespace SPG
{
    public class MyPropertyGrid : VisualHint.SmartPropertyGrid.PropertyGrid
    {
     
        
        //public int Opacity
        

        public MyPropertyGrid()
        {
        }

        private int _opacity;
        
        //[PropertyValidator(typeof( PropertyValidatorMinMax ), 0, 100)]

        public int Opacity
        {
            set { _opacity = value; }
            get { return _opacity; }
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
        
            //propEnum = InsertProperty(propEnum, 7, "Opacity", ParentForm, "Opacity", "");

            //  handles the item
            Opacity = (int)(ParentForm.Opacity * 100.0);
            propEnum = AppendProperty(propEnum, 7, "Opacity", this, "Opacity", "");
            propEnum.Property.Feel = GetRegisteredFeel( 
                VisualHint.SmartPropertyGrid.PropertyGrid.FeelTrackbarEdit);

            propEnum = AppendManagedProperty(catEnum, 8, "Chess piece", typeof(string), "Pawn", "");
            propEnum.Property.Feel = GetRegisteredFeel(
            VisualHint.SmartPropertyGrid.PropertyGrid.FeelList);
            propEnum = AppendManagedProperty(catEnum, 9, "Incremented value", typeof(double), 1.2, "");
            propEnum.Property.Feel = GetRegisteredFeel(
            VisualHint.SmartPropertyGrid.PropertyGrid.FeelEditUpDown);

           propEnum = AppendManagedProperty(catEnum, 10, "Size readonly", typeof(bool), false, "");
            propEnum.Property.Feel = GetRegisteredFeel(
            VisualHint.SmartPropertyGrid.PropertyGrid.FeelCheckbox);
            propEnum.Property.Look = new PropertyCheckboxLook();


            ToolbarVisibility = true;
            CommentsVisibility = true;

           
        }
         protected override void OnPropertyChanged(VisualHint.SmartPropertyGrid.PropertyChangedEventArgs e)
        {
            if (e.PropertyEnum.Property.Id == 7)
            {
                ParentForm.Opacity = (double)Opacity / 100.0;
                //FormBackColor = Color.FromArgb((int)((double)Opacity / 100.0 * 255.0), FormBackColor);
            }
            else if (e.PropertyEnum.Property.Id == 10)
                        {
                            PropertyEnumerator propEnum = FindProperty(6);
                            propEnum.Property.Value.ReadOnly = (bool)e.PropertyEnum.Property.
                            Value.GetValue();
                        }
         
         }

        protected override void OnInPlaceCtrlVisible(InPlaceCtrlVisibleEventArgs e)
        {
                 if (e.PropertyEnum.Property.Id == 7)
                    (e.InPlaceCtrl as PropInPlaceTrackbar).RealtimeChange = true;
                else if (e.PropertyEnum.Property.Id == 10)
                   (e.InPlaceCtrl as PropInPlaceCheckbox).RealtimeChange = true;

               else if (e.PropertyEnum.Parent.Property.Id == 6)
                   (e.InPlaceCtrl as PropInPlaceUpDown).RealtimeChange = true;
               else if (e.PropertyEnum.Property.Id == 9)
                   (e.InPlaceCtrl as PropInPlaceUpDown).RealtimeChange = false;

               base.OnInPlaceCtrlVisible(e);
        }
        protected override void OnDisplayedValuesNeeded(DisplayedValuesNeededEventArgs e)
        {
            if (e.PropertyEnum.Property.Id == 8)
                e.DisplayedValues = new string[] {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn" };
            base.OnDisplayedValuesNeeded(e);
        }
        protected override void OnPropertyUpDown(PropertyUpDownEventArgs e)
        {
            if (e.PropertyEnum.Property.Id == 9)
                e.Value = (Double.Parse(e.Value) + (e.ButtonPressed ==
                PropertyUpDownEventArgs.UpDownButtons.Up ? 0.1 : -0.1)).ToString();
            base.OnPropertyUpDown(e);
        }
    }
}
