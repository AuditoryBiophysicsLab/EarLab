using System;
using System.Runtime.InteropServices;
using System.Drawing;

namespace EarLab.Utilities
{
	/// <summary>
	/// Summary description for NativeMethods.
	/// </summary>
	internal sealed class NativeMethods
	{
		[DllImportAttribute("gdi32.dll")]
		public static extern bool BitBlt(
			IntPtr hdcDest, // handle to destination DC
			int nXDest, // x-coord of destination upper-left corner
			int nYDest, // y-coord of destination upper-left corner
			int nWidth, // width of destination rectangle
			int nHeight, // height of destination rectangle
			IntPtr hdcSrc, // handle to source DC
			int nXSrc, // x-coordinate of source upper-left corner
			int nYSrc, // y-coordinate of source upper-left corner
			System.Int32 dwRop // raster operation code
			);

		/// <summary>
		/// Tests if a visual style for the current application is active.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern int IsThemeActive();
		
		/// <summary>
		/// Opens the theme data for a window and its associated class.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern IntPtr OpenThemeData(IntPtr hWnd, [MarshalAs(UnmanagedType.LPTStr)] string classList);
		
		/// <summary>Closes the theme data handle.</summary>
		/// <remarks>The CloseThemeData function should be called when a window that has a visual style applied is destroyed.</remarks> 
		[DllImport("uxtheme.dll")]
		public static extern void CloseThemeData(IntPtr hTheme);
		
		/// <summary>
		/// Draws the background image defined by the visual style for the specified control part.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void DrawThemeBackground(IntPtr hTheme, IntPtr hDC, int partId, int stateId, ref RECT rect, ref RECT clipRect);
		
		/// <summary>
		/// Draws one or more edges defined by the visual style of a rectangle.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void DrawThemeEdge(IntPtr hTheme, IntPtr hDC, int partId, int stateId, ref RECT destRect, uint edge, uint flags, ref RECT contentRect);
		
		/// <summary>
		/// Draws an image from an image list with the icon effect defined by the visual style.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void DrawThemeIcon(IntPtr hTheme, IntPtr hDC, int partId, int stateId, ref RECT rect, IntPtr hIml, int imageIndex);
		
		/// <summary>
		/// Draws text using the color and font defined by the visual style.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void DrawThemeText(IntPtr hTheme, IntPtr hDC, int partId, int stateId, [MarshalAs(UnmanagedType.LPTStr)] string text, int charCount, uint textFlags, uint textFlags2, ref RECT rect);
		
		/// <summary>
		/// Draws the part of a parent control that is covered by a partially-transparent or alpha-blended child control.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void DrawThemeParentBackground(IntPtr hWnd, IntPtr hDC, ref RECT rect);
		
		/// <summary>
		/// Causes a window to use a different set of visual style information than its class normally uses.
		/// </summary>
		[DllImport("uxtheme.dll")]
		public static extern void SetWindowTheme(IntPtr hWnd, string subAppName, string subIdList);

		/// <summary>
		/// Gets the name of the current theme and color style.
		/// </summary>
		[DllImport("UxTheme.dll",CharSet=CharSet.Auto)]
		public static extern int GetCurrentThemeName(string pszThemeFileName, int dwMaxNameChars, string pszColorBuff, int cchMaxColorChars, string pszSizeBuff, int cchMaxSizeChars);

		// we use kernel32.dll functions to extract resources from *.msstyle themes files on windows xp.
		public const long LOAD_LIBRARY_AS_DATAFILE		= 0x00000002;
		[DllImport("kernel32.dll")]
		public static extern System.IntPtr LoadLibraryEx(string lpfFileName, System.IntPtr hFile, long dwFlags);
		[DllImport("kernel32.dll")]
		public static extern bool FreeLibrary(System.IntPtr hModule);

		public NativeMethods() {}
	}

	/// <summary>
	/// Summary description for RECT.
	/// </summary>
	[StructLayout(LayoutKind.Explicit)] 
	public struct RECT
	{
		[FieldOffset(0)] public int Left;

		[FieldOffset(4)] public int Top;

		[FieldOffset(8)] public int Right;

		[FieldOffset(12)] public int Bottom;

		public RECT(int left, int top, int right, int bottom) 
		{
			Left = left;
			Top = top;
			Right = right;
			Bottom = bottom;
		}

		public RECT(Rectangle rect) 
		{
			Left = rect.Left; 
			Top = rect.Top;
			Right = rect.Right;
			Bottom = rect.Bottom;
		}

		public Rectangle ToRectangle() 
		{
			return new Rectangle(Left, Top, Right, Bottom - 1);
		}
	}
}
