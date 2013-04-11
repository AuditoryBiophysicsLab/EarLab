using System;
using System.Windows.Forms;

namespace EarLab
{
	/// <summary>
	/// Summary description for NodeInfo.
	/// </summary>
	public class NodeInfo
	{
		public ToolTipType ToolTipType;
		public string ToolTip = null;
		public NodeInfoType NodeInfoType;
		public ContextMenu ContextMenu = null;
		public RunRecord RunRecord = null;
		public string FileName = null;
		public string ProgramName = null;

		public NodeInfo()
		{
			//
			// TODO: Add constructor logic here
			//
		}
	}

	public enum NodeInfoType
	{
		RunProgram,
		RunSimulation,
		ContextMenu,
		TextViewer,
	}

	public enum ToolTipType
	{
		Static,
		FilenameDependent,
	}
}
