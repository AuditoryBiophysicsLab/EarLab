using System;
using System.IO;
using ICSharpCode.SharpZipLib.Zip;

namespace EarLab.Utilities
{
	/// <summary>
	/// Summary description for Zip.
	/// </summary>
	public class Zip
	{
		public static byte[] ZipStreams(Stream[] streamsArray, string[] namesArray)
		{
			try
			{
				MemoryStream memoryStream = new MemoryStream();
				ZipOutputStream zipStream = new ZipOutputStream(memoryStream);
				zipStream.SetLevel(9);

				for (int i=0; i<streamsArray.Length; i++)
				{
					zipStream.PutNextEntry(new ZipEntry(namesArray[i]));
					byte[] streamBuffer = new byte[streamsArray[i].Length];
					streamsArray[i].Read(streamBuffer, 0, streamBuffer.Length);
					zipStream.Write(streamBuffer, 0, streamBuffer.Length);
				}

				zipStream.Flush();
				zipStream.Finish();

				byte[] byteArray = memoryStream.GetBuffer();
				memoryStream.Close();
				zipStream.Close();
				
				return byteArray;
			}
			catch (Exception e)
			{
				System.Diagnostics.Debug.WriteLine(e.Message);
				return null;
			}	
		}

		public static bool UnZipFile(byte[] byteArray, string outputDirectory)
		{
			try
			{
				MemoryStream memoryStream = new MemoryStream(byteArray, false);
				ZipInputStream zipStream = new ZipInputStream(memoryStream);
				ZipEntry zipEntry;

				while ((zipEntry = zipStream.GetNextEntry()) != null) 
				{
					FileStream fileStream = new FileStream(outputDirectory+"\\"+zipEntry.Name, FileMode.CreateNew, FileAccess.Write, FileShare.None);
					byte[] byteBuffer = new byte[zipEntry.Size];
					zipStream.Read(byteBuffer, 0, byteBuffer.Length);
					fileStream.Write(byteBuffer, 0, byteBuffer.Length);
					fileStream.Close();
				}

				memoryStream.Close();
				zipStream.Close();
				return true;
			}
			catch
			{
				return false;
			}
		}
		
		public static bool ZipFiles(string[] fileArray, string savePath)
		{
			try
			{
				ZipOutputStream zipStream = new ZipOutputStream(File.Create(savePath));
				zipStream.SetLevel(9);

				foreach (string file in fileArray)
				{
					zipStream.PutNextEntry(new ZipEntry(Path.GetFileName(file)));
					FileStream fileStream = File.OpenRead(file);
					byte[] fileBuffer = new byte[fileStream.Length];
					fileStream.Read(fileBuffer, 0, fileBuffer.Length);
					fileStream.Close();
					zipStream.Write(fileBuffer, 0, fileBuffer.Length);
				}

				zipStream.Finish();
				zipStream.Close();

				return true;
			}
			catch
			{
				return false;
			}
		}
	}
}
