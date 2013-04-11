using System;
using System.Collections;
using System.Threading;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace EarLab.ColorBars
{
	/// <summary>
	/// Summary description for ColorBarStandard.
	/// </summary>
	public class ColorBarStandard
	{
		private Color [] mColors;
		private Color mOutOfRangeColor = Color.SlateGray;
		private int mColorCount;
		private double mMin, mMax, mCurMin, mCurMax, mStepSize;
		private static int [,] DefaultColors = {{0, 0, 143},
												{0, 0, 147},
												{0, 0, 151},
												{0, 0, 155},
												{0, 0, 159},
												{0, 0, 163},
												{0, 0, 167},
												{0, 0, 171},
												{0, 0, 175},
												{0, 0, 179},
												{0, 0, 183},
												{0, 0, 187},
												{0, 0, 191},
												{0, 0, 195},
												{0, 0, 199},
												{0, 0, 203},
												{0, 0, 207},
												{0, 0, 211},
												{0, 0, 215},
												{0, 0, 219},
												{0, 0, 223},
												{0, 0, 227},
												{0, 0, 231},
												{0, 0, 235},
												{0, 0, 239},
												{0, 0, 243},
												{0, 0, 247},
												{0, 0, 251},
												{0, 0, 255},
												{0, 3, 255},
												{0, 7, 255},
												{0, 11, 255},
												{0, 15, 255},
												{0, 19, 255},
												{0, 23, 255},
												{0, 27, 255},
												{0, 31, 255},
												{0, 35, 255},
												{0, 39, 255},
												{0, 43, 255},
												{0, 47, 255},
												{0, 51, 255},
												{0, 55, 255},
												{0, 59, 255},
												{0, 63, 255},
												{0, 67, 255},
												{0, 71, 255},
												{0, 75, 255},
												{0, 79, 255},
												{0, 83, 255},
												{0, 87, 255},
												{0, 91, 255},
												{0, 95, 255},
												{0, 99, 255},
												{0, 103, 255},
												{0, 107, 255},
												{0, 111, 255},
												{0, 115, 255},
												{0, 119, 255},
												{0, 123, 255},
												{0, 127, 255},
												{0, 131, 255},
												{0, 135, 255},
												{0, 139, 255},
												{0, 143, 255},
												{0, 147, 255},
												{0, 151, 255},
												{0, 155, 255},
												{0, 159, 255},
												{0, 163, 255},
												{0, 167, 255},
												{0, 171, 255},
												{0, 175, 255},
												{0, 179, 255},
												{0, 183, 255},
												{0, 187, 255},
												{0, 191, 255},
												{0, 195, 255},
												{0, 199, 255},
												{0, 203, 255},
												{0, 207, 255},
												{0, 211, 255},
												{0, 215, 255},
												{0, 219, 255},
												{0, 223, 255},
												{0, 227, 255},
												{0, 231, 255},
												{0, 235, 255},
												{0, 239, 255},
												{0, 243, 255},
												{0, 247, 255},
												{0, 251, 255},
												{0, 255, 255},
												{3, 255, 251},
												{7, 255, 247},
												{11, 255, 243},
												{15, 255, 239},
												{19, 255, 235},
												{23, 255, 231},
												{27, 255, 227},
												{31, 255, 223},
												{35, 255, 219},
												{39, 255, 215},
												{43, 255, 211},
												{47, 255, 207},
												{51, 255, 203},
												{55, 255, 199},
												{59, 255, 195},
												{63, 255, 191},
												{67, 255, 187},
												{71, 255, 183},
												{75, 255, 179},
												{79, 255, 175},
												{83, 255, 171},
												{87, 255, 167},
												{91, 255, 163},
												{95, 255, 159},
												{99, 255, 155},
												{103, 255, 151},
												{107, 255, 147},
												{111, 255, 143},
												{115, 255, 139},
												{119, 255, 135},
												{123, 255, 131},
												{127, 255, 127},
												{131, 255, 123},
												{135, 255, 119},
												{139, 255, 115},
												{143, 255, 111},
												{147, 255, 107},
												{151, 255, 103},
												{155, 255, 99},
												{159, 255, 95},
												{163, 255, 91},
												{167, 255, 87},
												{171, 255, 83},
												{175, 255, 79},
												{179, 255, 75},
												{183, 255, 71},
												{187, 255, 67},
												{191, 255, 63},
												{195, 255, 59},
												{199, 255, 55},
												{203, 255, 51},
												{207, 255, 47},
												{211, 255, 43},
												{215, 255, 39},
												{219, 255, 35},
												{223, 255, 31},
												{227, 255, 27},
												{231, 255, 23},
												{235, 255, 19},
												{239, 255, 15},
												{243, 255, 11},
												{247, 255, 7},
												{251, 255, 3},
												{255, 255, 0},
												{255, 251, 0},
												{255, 247, 0},
												{255, 243, 0},
												{255, 239, 0},
												{255, 235, 0},
												{255, 231, 0},
												{255, 227, 0},
												{255, 223, 0},
												{255, 219, 0},
												{255, 215, 0},
												{255, 211, 0},
												{255, 207, 0},
												{255, 203, 0},
												{255, 199, 0},
												{255, 195, 0},
												{255, 191, 0},
												{255, 187, 0},
												{255, 183, 0},
												{255, 179, 0},
												{255, 175, 0},
												{255, 171, 0},
												{255, 167, 0},
												{255, 163, 0},
												{255, 159, 0},
												{255, 155, 0},
												{255, 151, 0},
												{255, 147, 0},
												{255, 143, 0},
												{255, 139, 0},
												{255, 135, 0},
												{255, 131, 0},
												{255, 127, 0},
												{255, 123, 0},
												{255, 119, 0},
												{255, 115, 0},
												{255, 111, 0},
												{255, 107, 0},
												{255, 103, 0},
												{255, 99, 0},
												{255, 95, 0},
												{255, 91, 0},
												{255, 87, 0},
												{255, 83, 0},
												{255, 79, 0},
												{255, 75, 0},
												{255, 71, 0},
												{255, 67, 0},
												{255, 63, 0},
												{255, 59, 0},
												{255, 55, 0},
												{255, 51, 0},
												{255, 47, 0},
												{255, 43, 0},
												{255, 39, 0},
												{255, 35, 0},
												{255, 31, 0},
												{255, 27, 0},
												{255, 23, 0},
												{255, 19, 0},
												{255, 15, 0},
												{255, 11, 0},
												{255, 7, 0},
												{255, 3, 0},
												{255, 0, 0},
												{251, 0, 0},
												{247, 0, 0},
												{243, 0, 0},
												{239, 0, 0},
												{235, 0, 0},
												{231, 0, 0},
												{227, 0, 0},
												{223, 0, 0},
												{219, 0, 0},
												{215, 0, 0},
												{211, 0, 0},
												{207, 0, 0},
												{203, 0, 0},
												{199, 0, 0},
												{195, 0, 0},
												{191, 0, 0},
												{187, 0, 0},
												{183, 0, 0},
												{179, 0, 0},
												{175, 0, 0},
												{171, 0, 0},
												{167, 0, 0},
												{163, 0, 0},
												{159, 0, 0},
												{155, 0, 0},
												{151, 0, 0},
												{147, 0, 0},
												{143, 0, 0},
												{139, 0, 0},
												{135, 0, 0},
												{131, 0, 0},
												{127, 0, 0}};

		public Color [] Colors
		{
			get {return mColors;}
		}

		public delegate void ColorBarChangedHandler(object sender);
		public event ColorBarChangedHandler ColorsChanged;
		public event ColorBarChangedHandler MinMaxCurChanged;
		private void OnMinMaxCurChanged()
		{
			if (MinMaxCurChanged != null)
				MinMaxCurChanged(this);
		}
		
		private void OnColorsChanged()
		{
			if (ColorsChanged != null)
				ColorsChanged(this);
		}

		public void RenderDataToBitmap(int RenderStartX, long RenderWidth, double[,] Data, ref Bitmap bmp)
		{
			Bitmap tempBitmap = (Bitmap)bmp.Clone();

			int index;

			BitmapData bmData = tempBitmap.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb); 
			int stride = bmData.Stride; 
			System.IntPtr Scan0 = bmData.Scan0;

			unsafe 
			{ 
				byte* p = (byte*)Scan0.ToPointer();
				long nOffset = stride - (RenderWidth*3);
				p += RenderStartX*3;

				for (int i=bmp.Height-1; i>=0; i--)
				{
					for(int j=0; j<RenderWidth; j++)
					{
						index = Math.Min(mColors.Length-1, Math.Max(0,(int)((Data[j+RenderStartX, i] - mCurMin) / mStepSize)));

						p[0] = (byte)mColors[index].B;
						p[1] = (byte)mColors[index].G;
						p[2] = (byte)mColors[index].R;
						p += 3;
					}
					p += nOffset;
				}
			}

			tempBitmap.UnlockBits(bmData);
			bmp = (Bitmap)tempBitmap.Clone();
		}

		public Color this[int index]
        {
            get 
            {
                if ((index >= 0) && (index < mColorCount))
                    return mColors[(int)(index % mColorCount)];
                else
                    throw new IndexOutOfRangeException("Index must be less than " + mColorCount.ToString());
            }
        }

        public int ColorCount
        {
            get {return mColorCount;}
        }

		public Color GetColor(double Datum)
        {
			if ((Datum >= mCurMin) && (Datum <= mCurMax))
            {
                return this[(int)((double)(Datum - mCurMin) / mStepSize)];
            }
            else
            {
                if (Datum < mCurMin)
                    return this[0];
                else
                    return this[mColorCount - 1];
            }
        }

		public double Min
		{
			get {return mMin;}

			set 
			{
				if (mCurMin == mMin)
					mCurMin = mMin = value;
				else
				{
					mMin = value;
					mCurMin = Math.Max(mCurMin, mMin);
				}

				if (mCurMin >= mCurMax)
				{
					mCurMin = mMin;
					mCurMax = mMax;
				}

				mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
				this.OnMinMaxCurChanged();
			}
		}

		public double Max
		{
			get {return mMax;}
			
			set 
			{
				if (mCurMax == mMax)
					mCurMax = mMax = value;
				else
				{
					mMax = value;
					mCurMax = Math.Min(mCurMax, mMax);
				}

				if (mCurMax <= mMin)
				{
					mCurMin = mMin;
					mCurMax = mMax;
				}

				mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
				this.OnMinMaxCurChanged();
			}
		}

		public void SetMinMax(double minValue, double maxValue)
		{
			if (mCurMin == mMin)
				mCurMin = mMin = minValue;
			else
			{
				mMin = minValue;
				mCurMin = Math.Max(mCurMin, mMin);
			}

			if (mCurMax == mMax)
				mCurMax = mMax = maxValue;
			else
			{
				mMax = maxValue;
				mCurMax = Math.Min(mCurMax, mMax);
			}

			if (mCurMax <= mMin || mCurMin >= mCurMax)
			{
				mCurMin = mMin;
				mCurMax = mMax;
			}

			mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
			this.OnMinMaxCurChanged();
		}

		public void SetCurMinCurMax(double curMin, double curMax)
		{
			mCurMin = curMin;
			mCurMax = curMax;

			// check for some things that *should* never happen, and which would cause incorrect behavior
			if (curMin >= curMax)
				throw new System.Exception("ColorBar Class, SetCurMinCurMax(): Cannot assign CurMin >= CurMax");
			if (curMin < mMin || curMax > mMax)
				throw new System.Exception("ColorBar Class, SetCurMinCurMax(): Cannot have CurMin or CurMax oustide Min and Max range");

			mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
			this.OnMinMaxCurChanged();
		}

		public double CurMin
		{
			get {return mCurMin;}
			
			set 
			{
				mCurMin = value;
				mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
				this.OnMinMaxCurChanged();
			}
		}

		public double CurMax
		{
			get {return mCurMax;}
			
			set 
			{
				mCurMax = value;
				mStepSize = (mCurMax - mCurMin) / (double)mColorCount;
				this.OnMinMaxCurChanged();
			}
		}

		public double StepSize
        {
            get {return mStepSize;}
        }

		public ColorBarStandard(string ColorbarFilename)
		{
			mColors = null;
			mMin = mCurMin = 0;
			mMax = mCurMax = 1600;
			try
			{
				LoadColorBar(ColorbarFilename);
			}
			catch (FileNotFoundException)
			{
				LoadColorBarDefault();
			}
		}

		public ColorBarStandard()
		{
			mColors = null;
			mMin = mCurMin = 0;
			mMax = mCurMax = 1600;

			LoadColorBarDefault();
		}

		public void LoadColorBar(string fileName)
		{
			if (File.Exists(fileName))
			{
				string colorLine;
				string[] colorArray;
				bool redFilled, greenFilled, blueFilled;
				byte redColor, greenColor, blueColor;
				ArrayList colorsArray = new ArrayList();
				char splitDelimiter;

				using (StreamReader colorbarStream = new StreamReader(fileName))
				{
					
					colorLine = colorbarStream.ReadLine();
					if (colorLine.Split(',').Length > 1)
						splitDelimiter = ',';
					else if (colorLine.Split(' ').Length > 1)
						splitDelimiter = ' ';
					else
						throw new System.IO.FileLoadException("File was not of expected format.", fileName);
				}

				using (StreamReader colorbarStream = new StreamReader(fileName))
				{
					redFilled = greenFilled = blueFilled = false;
					redColor = greenColor = blueColor = byte.MinValue;
					while ((colorLine = colorbarStream.ReadLine()) != null)
					{
						//if there is a blank line, ignore it
						if (colorLine.Trim() != "")
						{

							colorArray = colorLine.Trim().Split(splitDelimiter);

							if (colorArray.Length < 3)
								throw new System.IO.FileLoadException("File was not of expected format.", fileName);

							for (int i=0; i<colorArray.Length; i++)
							{
								if (!redFilled && colorArray[i] != "")
								{
									redColor = (byte)(float.Parse(colorArray[i]) * 255.0f);
									redFilled = true;
								}
								else if (!greenFilled && colorArray[i] != "")
								{
									greenColor = (byte)(float.Parse(colorArray[i]) * 255.0f);
									greenFilled = true;
								}
								else if (colorArray[i] != "")
								{
									blueColor = (byte)(float.Parse(colorArray[i]) * 255.0f);
									blueFilled = true;
								}
							}

							if (!redFilled || !greenFilled || !blueFilled)
								throw new System.IO.FileLoadException("File was not of expected format.", fileName);

							colorsArray.Add(Color.FromArgb(redColor, greenColor, blueColor));
							redFilled = greenFilled = blueFilled = false;
						}
					}

					mColorCount = colorsArray.Count;
					mColors = new Color[mColorCount];
					colorsArray.CopyTo(0, mColors, 0, colorsArray.Count);

					this.OnColorsChanged();
				}
			}
			else
				throw new System.IO.FileNotFoundException("The file was not found.", fileName);
		}

		public void LoadColorBarDefault()
		{
			mColorCount = ColorBarStandard.DefaultColors.GetLength(0);
			mColors = new Color[mColorCount];
			for (int i=0; i<mColorCount; i++)
				mColors[i] = Color.FromArgb(ColorBarStandard.DefaultColors[i,0], ColorBarStandard.DefaultColors[i,1], ColorBarStandard.DefaultColors[i,2]);

			this.OnColorsChanged();
		}
    }
}
