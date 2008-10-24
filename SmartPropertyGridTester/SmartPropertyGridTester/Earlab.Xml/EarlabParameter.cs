using System;
using System.Collections.Generic;
using System.Text;

namespace SmartPropertyGridTester
{
    public abstract class EarlabParameter
    {
        public readonly string Name;
        public readonly string Description;
        public bool IsError = false;
        public string ErrorText;

        public EarlabParameter(string Name, string Description)
        {
            this.Name = Name;
            this.Description = Description;
        }

        public bool Equals(EarlabParameter that)
        {
            if ((this.Name == that.Name) && (this.Description == that.Description))
                return true;
            return false;
        }

        public abstract bool IsDefault { get;}
        public abstract override string ToString();
    }

    public class EarlabParameter<T> : EarlabParameter
    {
        public readonly T Default;
        private T mValue;

        public EarlabParameter(string Name, string Description, T Default, T InitialValue)
            : base(Name, Description)
        {
            this.Default = Default;
            this.Value = InitialValue;
        }

        public override bool IsDefault
        {
            get { return Value.Equals(Default); }
        }

        public override string ToString()
        {
            Type valueType;
            int i;

            string result = string.Format("{0,-11} {1,-20}", this.GetType().Name.Remove(0, 6), Name);
            valueType = Value.GetType();
            if (!valueType.IsArray)
                result += Value.ToString();
            else
            {
                int[] itmp = Value as int[];
                double[] dtmp = Value as double[];

                result += "[ ";
                if (itmp != null)
                {
                    for (i = 0; i < itmp.Length; i++)
                        result += itmp[i].ToString() + " ";
                }
                if (dtmp != null)
                {
                    for (i = 0; i < dtmp.Length; i++)
                        result += dtmp[i].ToString() + " ";
                }
                result += "]";
            }

            return result;
        }

        public T Value
        {
            set { mValue = value; }
            get { return mValue; }
        }
    }

    public class EarlabString : EarlabParameter<string>
    {
        public EarlabString(string Name, string Description, string Default, string InitialValue)
            :base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabInteger : EarlabParameter<int>
    {
        public EarlabInteger(string Name, string Description, int Default, int InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }
    
    public class EarlabFloat : EarlabParameter<double>
    {
        public EarlabFloat(string Name, string Description, double Default, double InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabBoolean : EarlabParameter<bool>
    {
        public EarlabBoolean(string Name, string Description, bool Default, bool InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabInputFile : EarlabParameter<string>
    {
        public EarlabInputFile(string Name, string Description, string Default, string InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabOutputFile : EarlabParameter<string>
    {
        public EarlabOutputFile(string Name, string Description, string Default, string InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabIntArray : EarlabParameter<int[]>
    {
        public EarlabIntArray(string Name, string Description, int[] Default, int[] InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabFloatArray : EarlabParameter<double[]>
    {
        public EarlabFloatArray(string Name, string Description, double[] Default, double[] InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    static class EarlabParameterFactory
    {
        public static EarlabParameter CreateParameter(string ParamType, string ParamName, string Description, string DefaultValue, string InitialValue)
        {
            double dblDefault, dblValue;
            int intDefault, intValue;
            bool boolDefault, boolValue;

            switch (ParamType.ToLower())
            {
                case "float":
                case "double":
                    if ((!double.TryParse(DefaultValue, out dblDefault)) || (!double.TryParse(InitialValue, out dblValue)))
                        return null;
                    return new EarlabFloat(ParamName, DefaultValue, dblDefault, dblValue);
                case "int":
                case "integer":
                    if ((!int.TryParse(DefaultValue, out intDefault)) || (!int.TryParse(InitialValue, out intValue)))
                        return null;
                    return new EarlabFloat(ParamName, DefaultValue, intDefault, intValue);
                case "bool":
                case "boolean":
                    if ((!bool.TryParse(DefaultValue, out boolDefault)) || (!bool.TryParse(InitialValue, out boolValue)))
                        return null;
                    return new EarlabBoolean(ParamName, DefaultValue, boolDefault, boolValue);
                case "string":
                    return new EarlabString(ParamName, DefaultValue, DefaultValue, InitialValue);
                case "intarray":
                case "integerarray":
                case "int_array":
                    return null;
               case "floatarray":
                case "doublearray":
                case "float_array":
                    return null;
                default:
                    return null;
            }
        }
    }
}
