using System;
using System.IO;
using System.Collections;
using System.Runtime.InteropServices;

namespace Earlab
{
	public delegate void LogCallback(String LogMessage);
	/// <summary>
	/// DesktopEarlabDLL is a wrapper class that wraps the DesktopEarlabDLL.dll functionality
	/// and exposes it to C# applications
	/// </summary>
	class DesktopEarlabDLL
	{
		IntPtr theModel;
		private const string DLLFileName = @"DesktopEarlabDLL.dll";
		private string SavedPath;

		[DllImport(DLLFileName, EntryPoint="CreateModel", CallingConvention=CallingConvention.Cdecl)]
		private static extern IntPtr CreateModelExternal();

		[DllImport(DLLFileName, EntryPoint="SetLogCallback", CallingConvention=CallingConvention.Cdecl)]
		private static extern void SetLogCallbackExternal(IntPtr ModelPtr, LogCallback theCallback);

		[DllImport(DLLFileName, EntryPoint="SetModuleDirectory", CallingConvention=CallingConvention.Cdecl)]
		private static extern int SetModuleDirectoryExternal(IntPtr ModelPtr, string ModuleDirectoryPath);

		[DllImport(DLLFileName, EntryPoint="SetInputDirectory", CallingConvention=CallingConvention.Cdecl)]
		private static extern int SetInputDirectoryExternal(IntPtr ModelPtr, string InputDirectoryPath);

		[DllImport(DLLFileName, EntryPoint="SetOutputDirectory", CallingConvention=CallingConvention.Cdecl)]
		private static extern int SetOutputDirectoryExternal(IntPtr ModelPtr, string OutputDirectoryPath);

		[DllImport(DLLFileName, EntryPoint="LoadModelConfigFile", CallingConvention=CallingConvention.Cdecl)]
		private static extern int LoadModelConfigFileExternal(IntPtr ModelPtr, string ModelConfigFileName, float FrameSize_uS);

		[DllImport(DLLFileName, EntryPoint="LoadModuleParameters", CallingConvention=CallingConvention.Cdecl)]
		private static extern int LoadModuleParametersExternal(IntPtr ModelPtr, string ModuleParameterFileName);

		[DllImport(DLLFileName, EntryPoint="StartModules", CallingConvention=CallingConvention.Cdecl)]
		private static extern int StartModulesExternal(IntPtr ModelPtr);

		[DllImport(DLLFileName, EntryPoint="RunModel", CallingConvention=CallingConvention.Cdecl)]
		private static extern int RunModelExternal(IntPtr ModelPtr, int NumFrames);

		[DllImport(DLLFileName, EntryPoint="AdvanceModules", CallingConvention=CallingConvention.Cdecl)]
		private static extern int AdvanceModulesExternal(IntPtr ModelPtr);

		[DllImport(DLLFileName, EntryPoint="StopModules", CallingConvention=CallingConvention.Cdecl)]
		private static extern int StopModulesExternal(IntPtr ModelPtr);

		[DllImport(DLLFileName, EntryPoint="UnloadModel", CallingConvention=CallingConvention.Cdecl)]
		private static extern int UnloadModelExternal(IntPtr ModelPtr);

		public DesktopEarlabDLL()
		{
			theModel = DesktopEarlabDLL.CreateModelExternal();
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Failed to initialize model");
		}

		public int SetModuleDirectory(string ModuleDirectoryPath)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.SetModuleDirectoryExternal(theModel, ModuleDirectoryPath);
		}

		public void SetInputDirectory(string InputDirectoryPath)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			DesktopEarlabDLL.SetInputDirectoryExternal(theModel, InputDirectoryPath);
		}

		public void SetOutputDirectory(string OutputDirectoryPath)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			DesktopEarlabDLL.SetOutputDirectoryExternal(theModel, OutputDirectoryPath);
		}

		public void SetLogCallback(LogCallback theCallback)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			DesktopEarlabDLL.SetLogCallbackExternal(theModel, theCallback);
		}

		public int LoadModelConfigFile(string ModelConfigFileName, float FrameSize_uS)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			SavedPath = Environment.CurrentDirectory;
			Environment.CurrentDirectory = Path.GetDirectoryName(ModelConfigFileName);
			return DesktopEarlabDLL.LoadModelConfigFileExternal(theModel, ModelConfigFileName, FrameSize_uS);
		}

		public int LoadModuleParameters(string ModuleParameterFileName)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.LoadModuleParametersExternal(theModel, ModuleParameterFileName);
		}

		public int StartModules()
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.StartModulesExternal(theModel);
		}

		public int RunModel(int NumFrames)
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.RunModelExternal(theModel, NumFrames);
		}

		public int AdvanceModules()
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.AdvanceModulesExternal(theModel);
		}

		public int StopModules()
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			return DesktopEarlabDLL.StopModulesExternal(theModel);
		}

		public int UnloadModel()
		{
			if (theModel == IntPtr.Zero)
				throw new ApplicationException("Model not initialized");
			Environment.CurrentDirectory = SavedPath;
			return DesktopEarlabDLL.UnloadModelExternal(theModel);
		}
	}
}
