using System;
using System.IO;
using System.Drawing;
using System.Collections;

namespace EarLab.ReaderWriters
{
	/// <summary>
	/// Summary description for ReaderWriterASCIIColorbar.
	/// </summary>
	public class ReaderWriterASCIIColorbar
	{
		public static Color[] Read(string fileName)
		{
			try
			{
				StreamReader colorbarReader = new StreamReader(fileName, System.Text.Encoding.ASCII);

				string colorLine;
				ArrayList tempArray = new ArrayList();
				while ((colorLine = colorbarReader.ReadLine()) != null)
				{
					string[] colorArray = colorLine.Split(',');

					// get the 3 components of each color
					byte red = byte.Parse(colorArray[0].Trim());
					byte green = byte.Parse(colorArray[1].Trim());
					byte blue = byte.Parse(colorArray[2].Trim());
				
					// insert items into array
					tempArray.Add(Color.FromArgb(red, green, blue));
				}

				// store the results from temp array into our Color array
				Color[] colorsArray = new Color[tempArray.Count];
				tempArray.CopyTo(colorsArray, 0);
				return colorsArray;
			}
			catch
			{
				return null;
			}
		}

		public static Color[] ReadInternal(string fileName)
		{
			try
			{
				// get stream to internal embedded colorbar
				Stream colorbarStream = System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream("EarLab.ColorBars." + fileName + ".txt");
				StreamReader colorbarReader = new StreamReader(colorbarStream, System.Text.Encoding.ASCII);

				string colorLine;
				ArrayList tempArray = new ArrayList();
				while ((colorLine = colorbarReader.ReadLine()) != null)
				{
					string[] colorArray = colorLine.Split(',');

					// get the 3 components of each color
					byte red = byte.Parse(colorArray[0].Trim());
					byte green = byte.Parse(colorArray[1].Trim());
					byte blue = byte.Parse(colorArray[2].Trim());
				
					// insert items into array
					tempArray.Add(Color.FromArgb(red, green, blue));
				}

				// store the results from temp array into our Color array
				Color[] colorsArray = new Color[tempArray.Count];
				tempArray.CopyTo(colorsArray, 0);
				return colorsArray;
			}
			catch
			{
				return null;
			}
		}

		public static Color[] Read2(string fileName)
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

					int mColorCount = colorsArray.Count;
					Color[] mColors = new Color[mColorCount];
					colorsArray.CopyTo(0, mColors, 0, colorsArray.Count);

					StreamWriter outputWriter = new StreamWriter("colorbar.txt", false, System.Text.Encoding.ASCII);
					foreach (Color color in mColors)
						outputWriter.WriteLine("{0}, {1}, {2}", color.R.ToString(), color.G.ToString(), color.B.ToString());
					outputWriter.Flush();
					outputWriter.Close();

					return null;
				}
			}
			else
				throw new System.IO.FileNotFoundException("The file was not found.", fileName);
		}
	}
}
