using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using VisualHint.SmartPropertyGrid;

namespace SmartPropertyGridTester
{
    public partial class ModulePropertyGrid : VisualHint.SmartPropertyGrid.PropertyGrid
    {
        private PropertyEnumerator mParamsEnum;
        private PropertyEnumerator mInputsEnum;
        private PropertyEnumerator mOutputsEnum;
        private List<EarlabParameter> mParamList;

        public ModulePropertyGrid()
        {
            InitializeComponent();
        }

        public List<EarlabParameter> ParameterList
        {
            set
            {
                mParamList = value;
                Initialize();
            }
        }

        private void Initialize()
        {
            PropertyEnumerator curParamEnum, curArrayEnum;
            int propID = 1;

            if (mParamList == null)
                return;
            if (mParamList.Count == 0)
                return;

            Clear();
            mParamsEnum = AppendRootCategory(propID++, "Parameters");
            foreach (EarlabParameter param in mParamList)
            {
                Type t = param.GetType();
                string TypeString;

                switch (t.Name)
                {
                    case "EarlabOutputFile":
                    case "EarlabInputFile":
                    case "EarlabFloat":
                    case "EarlabInteger":
                    case "EarlabString":
                        curParamEnum = AppendProperty(mParamsEnum, propID++, param.Name, param, "Value", "");
                        curParamEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                        ExpandProperty(curParamEnum, true);
                        break;
                    case "EarlabBoolean":
                        curParamEnum = AppendProperty(mParamsEnum, propID++, param.Name, param, "Value", "");
                        curParamEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelCheckbox);
                        ExpandProperty(curParamEnum, true);
                        break;
                    case "EarlabIntArray":
                        int[] theIntArray = ((EarlabIntArray)param).Value.Array;
                        curParamEnum = AppendManagedProperty(mParamsEnum, propID++, param.Name, typeof(string), "", "");
                        curParamEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                        for (int i = 0; i < theIntArray.Length; i++)
                            curArrayEnum = AppendManagedProperty(curParamEnum, propID++, i.ToString(), typeof(int), theIntArray[i], "");
                        ExpandProperty(curParamEnum, false);
                        break;
                    case "EarlabFloatArray":
                        double[] theDoubleArray = ((EarlabFloatArray)param).Value.Array;
                        curParamEnum = AppendManagedProperty(mParamsEnum, propID++, param.Name, typeof(string), "", "");
                        curParamEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                        for (int i = 0; i < theDoubleArray.Length; i++)
                            curArrayEnum = AppendManagedProperty(curParamEnum, propID++, i.ToString(), typeof(double), theDoubleArray[i], "");
                        ExpandProperty(curParamEnum, false);
                        break;
                    default:
                        curParamEnum = AppendManagedProperty(mParamsEnum, propID++, param.Name, typeof(string), "<unknown>", "Unknown parameter type!");
                        break;
                }
                TypeString = t.Name.Remove(0, 6);
                if (param.IsError)
                {
                    curParamEnum.Property.Comment = "Type: " + TypeString + "\r\nError: " + param.ErrorText + "\r\nDescription: " + param.Description;
                    curParamEnum.Property.ForeColor = Color.Red;
                }
                else
                    curParamEnum.Property.Comment = "Type: " + TypeString + "\r\nDescription: " + param.Description;
                
            }
            mInputsEnum = AppendRootCategory(propID++, "Inputs");
            mOutputsEnum = AppendRootCategory(propID++, "Outputs");
        }
    }
}
