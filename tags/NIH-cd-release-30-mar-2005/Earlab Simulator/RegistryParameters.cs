using System;
using System.Windows;
using System.Windows.Forms;
using System.Drawing;
using Microsoft.Win32;

namespace Earlab
{
	/// <summary>
	/// Summary description for RegistryParameters.
	/// </summary>
	public class RegistryParameter
	{
		private RegistryKey mRegistryKey = null;
		private string mParameterName = null;
		private Object mParameter;
		private Object mDefault;

		public RegistryParameter(RegistryKey theKey, string ParameterName)
		{
			Initialize(theKey, ParameterName, null);
		}

		public RegistryParameter(RegistryKey theKey, string ParameterName, Object DefaultValue)
		{
			Initialize(theKey, ParameterName, DefaultValue);
		}

		private void Initialize(RegistryKey theKey, string ParameterName, Object DefaultValue)
		{
			// Remember parameters
			mRegistryKey = theKey;
			mParameterName = ParameterName;
			mDefault = DefaultValue;

			// Check parameters
			if (mRegistryKey == null)
				throw new ApplicationException("RegistryParameter: RegistryKey cannot be null");
			if (mParameterName == null)
				throw new ApplicationException("RegistryParameter: ParameterName cannot be null");

			// Load the value from the registry
			try
			{
				mParameter = mRegistryKey.GetValue(mParameterName, mDefault);
			}
			catch (ArgumentException)
			{
				mParameter = mDefault;
			}
		}

		public Object Value
		{
			set
			{
				mParameter = value;
				mRegistryKey.SetValue(mParameterName, mParameter);
			}
			get
			{
				return mParameter;
			}
		}
	}

	public class RegistryString: RegistryParameter
	{
		public RegistryString(RegistryKey theKey, string ParameterName, string DefaultValue) : base(theKey, ParameterName, DefaultValue){}
		public RegistryString(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new string Value
		{
			set {base.Value = value;}
			get {return (string)base.Value;}
		}
	}

	public class RegistryInt: RegistryParameter
	{
		public RegistryInt(RegistryKey theKey, string ParameterName, int DefaultValue) : base(theKey, ParameterName, (object)DefaultValue){}
		public RegistryInt(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new int Value
		{
			set {base.Value = (Object)value;}
			get {return (int)base.Value;}
		}
	}

	public class RegistryBool: RegistryParameter
	{
		public RegistryBool(RegistryKey theKey, string ParameterName, bool DefaultValue) : base(theKey, ParameterName, (object)DefaultValue){}
		public RegistryBool(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new bool Value
		{
			set {base.Value = (Object)value;}
			get {return bool.Parse(base.Value.ToString());}
		}
	}

	public class RegistryFloat: RegistryParameter
	{
		public RegistryFloat(RegistryKey theKey, string ParameterName, float DefaultValue) : base(theKey, ParameterName, (object)DefaultValue){}
		public RegistryFloat(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new float Value
		{
			set {base.Value = (Object)value;}
			get {return (float)base.Value;}
		}
	}

	public class RegistryDouble: RegistryParameter
	{
		public RegistryDouble(RegistryKey theKey, string ParameterName, double DefaultValue) : base(theKey, ParameterName, (object)DefaultValue){}
		public RegistryDouble(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new double Value
		{
			set {base.Value = (Object)value;}
			get {return (double)base.Value;}
		}
	}

	public class RegistryPoint
	{
		private RegistryInt X, Y;
		public RegistryPoint(RegistryKey theKey, string ParameterName, Point DefaultValue)
		{
			Init(theKey, ParameterName, DefaultValue);
		}

		public RegistryPoint(RegistryKey theKey, string ParameterName)
		{
			Init(theKey, ParameterName, new Point(0, 0));
		}

		private void Init(RegistryKey theKey, string ParameterName, Point DefaultValue)
		{
			X = new RegistryInt(theKey, ParameterName + "_X", DefaultValue.X);
			Y = new RegistryInt(theKey, ParameterName + "_Y", DefaultValue.Y);
		}

		public Point Value
		{
			set 
			{
				X.Value = value.X;
				Y.Value = value.Y;
			}
			get {return new Point(X.Value, Y.Value);}
		}
	}

	public class RegistrySize
	{
		private RegistryInt Width, Height;
		public RegistrySize(RegistryKey theKey, string ParameterName, Size DefaultValue)
		{
			Init(theKey, ParameterName, DefaultValue);
		}

		public RegistrySize(RegistryKey theKey, string ParameterName)
		{
			Init(theKey, ParameterName, new Size(0, 0));
		}

		private void Init(RegistryKey theKey, string ParameterName, Size DefaultValue)
		{
			Width = new RegistryInt(theKey, ParameterName + "_Width", DefaultValue.Width);
			Height = new RegistryInt(theKey, ParameterName + "_Height", DefaultValue.Height);
		}

		public Size Value
		{
			set 
			{
				Width.Value = value.Width;
				Height.Value = value.Height;
			}
			get {return new Size(Width.Value, Height.Value);}
		}
	}

	public class RegistryRectangle
	{
		private RegistryPoint Location;
		private RegistrySize Size;
		public RegistryRectangle(RegistryKey theKey, string ParameterName, Rectangle DefaultValue)
		{
			Init(theKey, ParameterName, DefaultValue);
		}

		public RegistryRectangle(RegistryKey theKey, string ParameterName)
		{
			Init(theKey, ParameterName, new Rectangle(0, 0, 0, 0));
		}

		private void Init(RegistryKey theKey, string ParameterName, Rectangle DefaultValue)
		{
			Location = new RegistryPoint(theKey, ParameterName + "_Location", DefaultValue.Location);
			Size = new RegistrySize(theKey, ParameterName + "_Size", DefaultValue.Size);
		}

		public Rectangle Value
		{
			set 
			{
				Location.Value = value.Location;
				Size.Value = value.Size;
			}
			get {return new Rectangle(Location.Value, Size.Value);}
		}
	}

	public class RegistryFormWindowState: RegistryParameter
	{
		public RegistryFormWindowState(RegistryKey theKey, string ParameterName, FormWindowState DefaultValue) : base(theKey, ParameterName, (object)DefaultValue){}
		public RegistryFormWindowState(RegistryKey theKey, string ParameterName) : base(theKey, ParameterName){}

		public new FormWindowState Value
		{
			set {base.Value = (Object)((int)value);}
			get {return (FormWindowState)base.Value;}
		}
	}

}
