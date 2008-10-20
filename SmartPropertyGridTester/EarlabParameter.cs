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
    }

    public class EarlabArray<T> 
    {
        private List<T> theList;

        public EarlabArray()
        {
            theList = new List<T>();
        }

        public EarlabArray(T[] data)
        {
            theList = new List<T>(data);
        }

        public T this[int index]
        {
            get { return theList[index]; }
            set { theList[index] = value; }
        }

        public int Length { get { return theList.Count; } }

        public T[] Array
        {
            get { return theList.ToArray(); }
            set
            {
                theList.Clear();
                foreach (T elem in value)
                    theList.Add(elem);
            }
        }

        public bool Equals(EarlabArray<T> that)
        {
            if (theList.Count != that.theList.Count)
                return false;
            for (int i = 0; i < theList.Count; i++)
                if (!theList[i].Equals(that.theList[i]))
                    return false;
            return true;
        }
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

        public bool IsDefault
        {
            get { return Value.Equals(Default); }
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

    public class EarlabIntArray : EarlabParameter<EarlabArray<int>>
    {
        public EarlabIntArray(string Name, string Description, EarlabArray<int> Default, EarlabArray<int> InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }

    public class EarlabFloatArray : EarlabParameter<EarlabArray<double>>
    {
        public EarlabFloatArray(string Name, string Description, EarlabArray<double> Default, EarlabArray<double> InitialValue)
            : base(Name, Description, Default, InitialValue)
        {
        }
    }
}
