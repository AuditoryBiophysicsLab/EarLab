using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ModuleUpdater.Controls
{
	/// <summary>
	/// Summary description for VersionControl.
	/// </summary>
	public class VersionControl : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.TabPage generalTabPage;
		private System.Windows.Forms.TextBox nameTextBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label descriptionLabel;
		private System.Windows.Forms.TextBox descriptionTextBox;
		private ModuleUpdater.InheritedControls.WholeNumberTextBox version1TextBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.GroupBox moduleGroupBox;
		private System.Windows.Forms.GroupBox versionGroupBox;
		private ModuleUpdater.InheritedControls.WholeNumberTextBox version2TextBox;
		private ModuleUpdater.InheritedControls.WholeNumberTextBox version3TextBox;
		private ModuleUpdater.InheritedControls.WholeNumberTextBox version4TextBox;
		private System.Windows.Forms.Label dateLabel;
		private System.Windows.Forms.Label contactLabel;
		private System.Windows.Forms.ComboBox contactComboBox;
		private System.Windows.Forms.Label dot1Label;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TabPage descriptionTabPage;
		private System.Windows.Forms.Label abstractLabel;
		private System.Windows.Forms.Label description2Label;
		private System.Windows.Forms.GroupBox abstractGroupBox;
		private System.Windows.Forms.TextBox abstractTextBox;
		private System.Windows.Forms.TextBox description2TextBox;
		private System.Windows.Forms.GroupBox classificationGroupBox;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid classificationDataGrid;
		private System.Windows.Forms.Label neuralLabel;
		private System.Windows.Forms.TextBox neuralTextBox;
		private System.Windows.Forms.LinkLabel classificationLinkLabel;
		private System.Windows.Forms.TextBox interfaceTextBox;
		private System.Windows.Forms.Label interfaceLabel;
		private System.Windows.Forms.TextBox implementationTextBox;
		private System.Windows.Forms.Label implementationLabel;
		private System.Windows.Forms.TextBox designTextBox;
		private System.Windows.Forms.Label designLabel;
		private System.Windows.Forms.GroupBox parametersGroupBox;
		private System.Windows.Forms.TabPage designTabPage;
		private System.Windows.Forms.TabPage parametersTabPage;
		private System.Windows.Forms.GroupBox programmingGroupBox;
		private System.Windows.Forms.Label languageLabel;
		private System.Windows.Forms.Label osLabel;
		private System.Windows.Forms.Label complianceLabel;
		private System.Windows.Forms.GroupBox prerequisiteGroupBox;
		private System.Windows.Forms.TextBox prerequisiteTextBox;
		private System.Windows.Forms.TabPage notesTabPage;
		private System.Windows.Forms.TextBox evaluationTextBox;
		private System.Windows.Forms.GroupBox evaluationGroupBox;
		private System.Windows.Forms.GroupBox notesGroupBox;
		private System.Windows.Forms.TextBox notesTextBox;
		private System.Windows.Forms.TabPage historyTabPage;
		private System.Windows.Forms.GroupBox historyGroupBox;
		private System.Windows.Forms.LinkLabel historyLinkLabel;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid historyDataGrid;
		private System.Windows.Forms.LinkLabel referenceLinkLabel;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid referenceDataGrid;
		private System.Windows.Forms.GroupBox referencesGroupBox;
		private System.Windows.Forms.TabPage restrictionsTabPage;
		private System.Windows.Forms.GroupBox bugGroupBox;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid bugDataGrid;
		private System.Windows.Forms.GroupBox restrictionGroupBox;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid restrictionDataGrid;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid datatypeDataGrid;
		private System.Windows.Forms.LinkLabel parameterLinkLabel;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid parameterDataGrid;
		private System.Windows.Forms.GroupBox datatypeGroupBox;
		private System.Windows.Forms.LinkLabel datatypeLinkLabel;
		private System.Windows.Forms.TabControl versionTabControl;
		private System.Windows.Forms.LinkLabel backLinkLabel;

		private long versionID;
		private long moduleID;
		private System.Windows.Forms.DateTimePicker releaseDateTimePicker;
		private System.Windows.Forms.DataGridTableStyle parameterStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn parameterNameColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn parameterDescriptionColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn parameterTypeColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn parameterUpdateColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn parameterDeleteColumn;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid osDataGrid;
		private System.Windows.Forms.ComboBox osComboBox;
		private System.Windows.Forms.LinkLabel osLinkLabel;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid languageDataGrid;
		private ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid complianceDataGrid;
		private System.Windows.Forms.LinkLabel languageLinkLabel;
		private System.Windows.Forms.ComboBox languageComboBox;
		private System.Windows.Forms.LinkLabel complianceLinkLabel;
		private System.Windows.Forms.ComboBox complianceComboBox;
		private System.Windows.Forms.DataGridTableStyle osStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn osColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn osDeleteColumn;
		private System.Windows.Forms.DataGridTableStyle languageStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn languageColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn languageDeleteColumn;
		private System.Windows.Forms.DataGridTableStyle complianceStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn complianceColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn complianceDeleteColumn;
		private System.Windows.Forms.TabPage programmingTabPage;
		private System.Windows.Forms.LinkLabel restrictionLinkLabel;
		private System.Windows.Forms.LinkLabel bugLinkLabel;
		private System.Windows.Forms.TextBox restrictionTextBox;
		private System.Windows.Forms.TextBox bugTextBox;
		private System.Windows.Forms.DataGridTableStyle restrictionStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn restrictionColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn restrictionDeleteColumn;
		private System.Windows.Forms.DataGridTableStyle bugStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn bugColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn bugDeleteColumn;
		private System.Windows.Forms.TextBox referenceTextBox;
		private System.Windows.Forms.TextBox historyTextBox;
		private System.Windows.Forms.DataGridTableStyle referenceStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn referenceColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn referenceDeleteColumn;
		private System.Windows.Forms.DataGridTableStyle historyStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn historyColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn historyDeleteColumn;
		private System.Windows.Forms.DataGridTableStyle classificationStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn structureColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn substructureColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn celltypeColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn classificationUpdateColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn classificationDeleteColumn;
		private DataSet versionDataSet;
		private System.Windows.Forms.Button okButton;
		private bool addMode = false;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn parameterMinColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn parameterMaxColumn;
		private System.Windows.Forms.DataGridTableStyle datatypeStyle;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn datatypeColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn directionColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn datatypeUpdateColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLinkColumn datatypeDeleteColumn;
		private ModuleUpdater.InheritedControls.ColorDataGridLabelColumn datatypeNameColumn;
		private bool setupDone = false;
		private System.Windows.Forms.DataGridTextBoxColumn indexColumn;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox versionLSIDTtextBox;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox moduleLSIDTextBox;
		private System.Windows.Forms.TabPage filesTabPage;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.TextBox dllTextBox;
		private System.Windows.Forms.Button dllButton;

		public enum VersionControlStatus {AddModule, AddVersion, UpdateVersion};
		private VersionControlStatus currentStatus;
		
		public VersionControl(bool addMode, long databaseID)
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			EnableVisualStyles.Enable(this);

			this.addMode = addMode;

			if (this.addMode)
				this.moduleID = databaseID;
			else
				this.versionID = databaseID;

			this.versionTabControl.TabPages.Clear();
			this.SetupControl();
		}

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.versionTabControl = new System.Windows.Forms.TabControl();
			this.generalTabPage = new System.Windows.Forms.TabPage();
			this.versionGroupBox = new System.Windows.Forms.GroupBox();
			this.version4TextBox = new ModuleUpdater.InheritedControls.WholeNumberTextBox();
			this.versionLSIDTtextBox = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.dot1Label = new System.Windows.Forms.Label();
			this.contactComboBox = new System.Windows.Forms.ComboBox();
			this.contactLabel = new System.Windows.Forms.Label();
			this.releaseDateTimePicker = new System.Windows.Forms.DateTimePicker();
			this.dateLabel = new System.Windows.Forms.Label();
			this.version3TextBox = new ModuleUpdater.InheritedControls.WholeNumberTextBox();
			this.version2TextBox = new ModuleUpdater.InheritedControls.WholeNumberTextBox();
			this.version1TextBox = new ModuleUpdater.InheritedControls.WholeNumberTextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.moduleGroupBox = new System.Windows.Forms.GroupBox();
			this.moduleLSIDTextBox = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.descriptionTextBox = new System.Windows.Forms.TextBox();
			this.nameTextBox = new System.Windows.Forms.TextBox();
			this.descriptionLabel = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.parametersTabPage = new System.Windows.Forms.TabPage();
			this.datatypeGroupBox = new System.Windows.Forms.GroupBox();
			this.datatypeLinkLabel = new System.Windows.Forms.LinkLabel();
			this.datatypeDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.datatypeStyle = new System.Windows.Forms.DataGridTableStyle();
			this.datatypeNameColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.indexColumn = new System.Windows.Forms.DataGridTextBoxColumn();
			this.datatypeColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.directionColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.datatypeUpdateColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.datatypeDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.parametersGroupBox = new System.Windows.Forms.GroupBox();
			this.parameterLinkLabel = new System.Windows.Forms.LinkLabel();
			this.parameterDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.parameterStyle = new System.Windows.Forms.DataGridTableStyle();
			this.parameterNameColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.parameterDescriptionColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.parameterTypeColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.parameterMinColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.parameterMaxColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.parameterUpdateColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.parameterDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.programmingTabPage = new System.Windows.Forms.TabPage();
			this.prerequisiteGroupBox = new System.Windows.Forms.GroupBox();
			this.prerequisiteTextBox = new System.Windows.Forms.TextBox();
			this.programmingGroupBox = new System.Windows.Forms.GroupBox();
			this.complianceLinkLabel = new System.Windows.Forms.LinkLabel();
			this.complianceComboBox = new System.Windows.Forms.ComboBox();
			this.languageLinkLabel = new System.Windows.Forms.LinkLabel();
			this.languageComboBox = new System.Windows.Forms.ComboBox();
			this.osLinkLabel = new System.Windows.Forms.LinkLabel();
			this.osComboBox = new System.Windows.Forms.ComboBox();
			this.complianceDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.complianceStyle = new System.Windows.Forms.DataGridTableStyle();
			this.complianceColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.complianceDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.languageDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.languageStyle = new System.Windows.Forms.DataGridTableStyle();
			this.languageColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.languageDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.osDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.osStyle = new System.Windows.Forms.DataGridTableStyle();
			this.osColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.osDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.complianceLabel = new System.Windows.Forms.Label();
			this.languageLabel = new System.Windows.Forms.Label();
			this.osLabel = new System.Windows.Forms.Label();
			this.designTabPage = new System.Windows.Forms.TabPage();
			this.designTextBox = new System.Windows.Forms.TextBox();
			this.designLabel = new System.Windows.Forms.Label();
			this.implementationTextBox = new System.Windows.Forms.TextBox();
			this.implementationLabel = new System.Windows.Forms.Label();
			this.interfaceTextBox = new System.Windows.Forms.TextBox();
			this.interfaceLabel = new System.Windows.Forms.Label();
			this.notesTabPage = new System.Windows.Forms.TabPage();
			this.notesGroupBox = new System.Windows.Forms.GroupBox();
			this.notesTextBox = new System.Windows.Forms.TextBox();
			this.evaluationGroupBox = new System.Windows.Forms.GroupBox();
			this.evaluationTextBox = new System.Windows.Forms.TextBox();
			this.filesTabPage = new System.Windows.Forms.TabPage();
			this.dllButton = new System.Windows.Forms.Button();
			this.dllTextBox = new System.Windows.Forms.TextBox();
			this.historyTabPage = new System.Windows.Forms.TabPage();
			this.historyGroupBox = new System.Windows.Forms.GroupBox();
			this.historyTextBox = new System.Windows.Forms.TextBox();
			this.historyLinkLabel = new System.Windows.Forms.LinkLabel();
			this.historyDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.historyStyle = new System.Windows.Forms.DataGridTableStyle();
			this.historyColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.historyDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.referencesGroupBox = new System.Windows.Forms.GroupBox();
			this.referenceTextBox = new System.Windows.Forms.TextBox();
			this.referenceDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.referenceStyle = new System.Windows.Forms.DataGridTableStyle();
			this.referenceColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.referenceDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.referenceLinkLabel = new System.Windows.Forms.LinkLabel();
			this.descriptionTabPage = new System.Windows.Forms.TabPage();
			this.classificationGroupBox = new System.Windows.Forms.GroupBox();
			this.classificationLinkLabel = new System.Windows.Forms.LinkLabel();
			this.neuralTextBox = new System.Windows.Forms.TextBox();
			this.neuralLabel = new System.Windows.Forms.Label();
			this.classificationDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.classificationStyle = new System.Windows.Forms.DataGridTableStyle();
			this.structureColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.substructureColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.celltypeColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.classificationUpdateColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.classificationDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.abstractGroupBox = new System.Windows.Forms.GroupBox();
			this.description2TextBox = new System.Windows.Forms.TextBox();
			this.abstractTextBox = new System.Windows.Forms.TextBox();
			this.description2Label = new System.Windows.Forms.Label();
			this.abstractLabel = new System.Windows.Forms.Label();
			this.restrictionsTabPage = new System.Windows.Forms.TabPage();
			this.bugGroupBox = new System.Windows.Forms.GroupBox();
			this.bugTextBox = new System.Windows.Forms.TextBox();
			this.bugDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.bugStyle = new System.Windows.Forms.DataGridTableStyle();
			this.bugColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.bugDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.bugLinkLabel = new System.Windows.Forms.LinkLabel();
			this.restrictionGroupBox = new System.Windows.Forms.GroupBox();
			this.restrictionTextBox = new System.Windows.Forms.TextBox();
			this.restrictionLinkLabel = new System.Windows.Forms.LinkLabel();
			this.restrictionDataGrid = new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid();
			this.restrictionStyle = new System.Windows.Forms.DataGridTableStyle();
			this.restrictionColumn = new ModuleUpdater.InheritedControls.ColorDataGridLabelColumn();
			this.restrictionDeleteColumn = new ModuleUpdater.InheritedControls.ColorDataGridLinkColumn();
			this.okButton = new System.Windows.Forms.Button();
			this.backLinkLabel = new System.Windows.Forms.LinkLabel();
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.versionTabControl.SuspendLayout();
			this.generalTabPage.SuspendLayout();
			this.versionGroupBox.SuspendLayout();
			this.moduleGroupBox.SuspendLayout();
			this.parametersTabPage.SuspendLayout();
			this.datatypeGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.datatypeDataGrid)).BeginInit();
			this.parametersGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.parameterDataGrid)).BeginInit();
			this.programmingTabPage.SuspendLayout();
			this.prerequisiteGroupBox.SuspendLayout();
			this.programmingGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.complianceDataGrid)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.languageDataGrid)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.osDataGrid)).BeginInit();
			this.designTabPage.SuspendLayout();
			this.notesTabPage.SuspendLayout();
			this.notesGroupBox.SuspendLayout();
			this.evaluationGroupBox.SuspendLayout();
			this.filesTabPage.SuspendLayout();
			this.historyTabPage.SuspendLayout();
			this.historyGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.historyDataGrid)).BeginInit();
			this.referencesGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.referenceDataGrid)).BeginInit();
			this.descriptionTabPage.SuspendLayout();
			this.classificationGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.classificationDataGrid)).BeginInit();
			this.abstractGroupBox.SuspendLayout();
			this.restrictionsTabPage.SuspendLayout();
			this.bugGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.bugDataGrid)).BeginInit();
			this.restrictionGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.restrictionDataGrid)).BeginInit();
			this.SuspendLayout();
			// 
			// versionTabControl
			// 
			this.versionTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionTabControl.Controls.Add(this.generalTabPage);
			this.versionTabControl.Controls.Add(this.filesTabPage);
			this.versionTabControl.Controls.Add(this.parametersTabPage);
			this.versionTabControl.Controls.Add(this.programmingTabPage);
			this.versionTabControl.Controls.Add(this.designTabPage);
			this.versionTabControl.Controls.Add(this.notesTabPage);
			this.versionTabControl.Controls.Add(this.historyTabPage);
			this.versionTabControl.Controls.Add(this.descriptionTabPage);
			this.versionTabControl.Controls.Add(this.restrictionsTabPage);
			this.versionTabControl.Location = new System.Drawing.Point(0, 0);
			this.versionTabControl.Name = "versionTabControl";
			this.versionTabControl.SelectedIndex = 0;
			this.versionTabControl.Size = new System.Drawing.Size(648, 336);
			this.versionTabControl.TabIndex = 0;
			// 
			// generalTabPage
			// 
			this.generalTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.generalTabPage.Controls.Add(this.versionGroupBox);
			this.generalTabPage.Controls.Add(this.moduleGroupBox);
			this.generalTabPage.Location = new System.Drawing.Point(4, 22);
			this.generalTabPage.Name = "generalTabPage";
			this.generalTabPage.Size = new System.Drawing.Size(640, 310);
			this.generalTabPage.TabIndex = 0;
			this.generalTabPage.Text = "General Info.";
			// 
			// versionGroupBox
			// 
			this.versionGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionGroupBox.Controls.Add(this.version4TextBox);
			this.versionGroupBox.Controls.Add(this.versionLSIDTtextBox);
			this.versionGroupBox.Controls.Add(this.label5);
			this.versionGroupBox.Controls.Add(this.label4);
			this.versionGroupBox.Controls.Add(this.label3);
			this.versionGroupBox.Controls.Add(this.dot1Label);
			this.versionGroupBox.Controls.Add(this.contactComboBox);
			this.versionGroupBox.Controls.Add(this.contactLabel);
			this.versionGroupBox.Controls.Add(this.releaseDateTimePicker);
			this.versionGroupBox.Controls.Add(this.dateLabel);
			this.versionGroupBox.Controls.Add(this.version3TextBox);
			this.versionGroupBox.Controls.Add(this.version2TextBox);
			this.versionGroupBox.Controls.Add(this.version1TextBox);
			this.versionGroupBox.Controls.Add(this.label2);
			this.versionGroupBox.Location = new System.Drawing.Point(8, 168);
			this.versionGroupBox.Name = "versionGroupBox";
			this.versionGroupBox.Size = new System.Drawing.Size(624, 128);
			this.versionGroupBox.TabIndex = 8;
			this.versionGroupBox.TabStop = false;
			this.versionGroupBox.Text = "Version Info";
			// 
			// version4TextBox
			// 
			this.version4TextBox.Location = new System.Drawing.Point(240, 24);
			this.version4TextBox.MaxLength = 4;
			this.version4TextBox.Name = "version4TextBox";
			this.version4TextBox.Size = new System.Drawing.Size(32, 20);
			this.version4TextBox.TabIndex = 8;
			this.version4TextBox.Text = "0";
			this.version4TextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// versionLSIDTtextBox
			// 
			this.versionLSIDTtextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.versionLSIDTtextBox.Location = new System.Drawing.Point(120, 96);
			this.versionLSIDTtextBox.Name = "versionLSIDTtextBox";
			this.versionLSIDTtextBox.ReadOnly = true;
			this.versionLSIDTtextBox.Size = new System.Drawing.Size(488, 20);
			this.versionLSIDTtextBox.TabIndex = 17;
			this.versionLSIDTtextBox.Text = "";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(16, 96);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(80, 16);
			this.label5.TabIndex = 16;
			this.label5.Text = "Version LSID:";
			this.label5.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(232, 36);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(8, 8);
			this.label4.TabIndex = 15;
			this.label4.Text = ".";
			this.label4.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(192, 36);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(8, 8);
			this.label3.TabIndex = 14;
			this.label3.Text = ".";
			this.label3.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			// 
			// dot1Label
			// 
			this.dot1Label.Location = new System.Drawing.Point(152, 36);
			this.dot1Label.Name = "dot1Label";
			this.dot1Label.Size = new System.Drawing.Size(8, 8);
			this.dot1Label.TabIndex = 13;
			this.dot1Label.Text = ".";
			this.dot1Label.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
			// 
			// contactComboBox
			// 
			this.contactComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.contactComboBox.Location = new System.Drawing.Point(120, 72);
			this.contactComboBox.Name = "contactComboBox";
			this.contactComboBox.Size = new System.Drawing.Size(224, 21);
			this.contactComboBox.TabIndex = 12;
			this.contactComboBox.SelectedIndexChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// contactLabel
			// 
			this.contactLabel.Location = new System.Drawing.Point(16, 72);
			this.contactLabel.Name = "contactLabel";
			this.contactLabel.Size = new System.Drawing.Size(88, 16);
			this.contactLabel.TabIndex = 11;
			this.contactLabel.Text = "Version Contact:";
			this.contactLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// releaseDateTimePicker
			// 
			this.releaseDateTimePicker.Location = new System.Drawing.Point(120, 48);
			this.releaseDateTimePicker.Name = "releaseDateTimePicker";
			this.releaseDateTimePicker.Size = new System.Drawing.Size(224, 20);
			this.releaseDateTimePicker.TabIndex = 10;
			this.releaseDateTimePicker.ValueChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// dateLabel
			// 
			this.dateLabel.Location = new System.Drawing.Point(16, 48);
			this.dateLabel.Name = "dateLabel";
			this.dateLabel.Size = new System.Drawing.Size(96, 16);
			this.dateLabel.TabIndex = 9;
			this.dateLabel.Text = "Version Release:";
			this.dateLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// version3TextBox
			// 
			this.version3TextBox.Location = new System.Drawing.Point(200, 24);
			this.version3TextBox.MaxLength = 4;
			this.version3TextBox.Name = "version3TextBox";
			this.version3TextBox.Size = new System.Drawing.Size(32, 20);
			this.version3TextBox.TabIndex = 7;
			this.version3TextBox.Text = "0";
			this.version3TextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// version2TextBox
			// 
			this.version2TextBox.Location = new System.Drawing.Point(160, 24);
			this.version2TextBox.MaxLength = 4;
			this.version2TextBox.Name = "version2TextBox";
			this.version2TextBox.Size = new System.Drawing.Size(32, 20);
			this.version2TextBox.TabIndex = 6;
			this.version2TextBox.Text = "0";
			this.version2TextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// version1TextBox
			// 
			this.version1TextBox.Location = new System.Drawing.Point(120, 24);
			this.version1TextBox.MaxLength = 4;
			this.version1TextBox.Name = "version1TextBox";
			this.version1TextBox.Size = new System.Drawing.Size(32, 20);
			this.version1TextBox.TabIndex = 4;
			this.version1TextBox.Text = "0";
			this.version1TextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(16, 24);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(96, 16);
			this.label2.TabIndex = 5;
			this.label2.Text = "Version Number:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// moduleGroupBox
			// 
			this.moduleGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.moduleGroupBox.Controls.Add(this.moduleLSIDTextBox);
			this.moduleGroupBox.Controls.Add(this.label6);
			this.moduleGroupBox.Controls.Add(this.descriptionTextBox);
			this.moduleGroupBox.Controls.Add(this.nameTextBox);
			this.moduleGroupBox.Controls.Add(this.descriptionLabel);
			this.moduleGroupBox.Controls.Add(this.label1);
			this.moduleGroupBox.Location = new System.Drawing.Point(8, 8);
			this.moduleGroupBox.Name = "moduleGroupBox";
			this.moduleGroupBox.Size = new System.Drawing.Size(624, 152);
			this.moduleGroupBox.TabIndex = 7;
			this.moduleGroupBox.TabStop = false;
			this.moduleGroupBox.Text = "Module Info";
			// 
			// moduleLSIDTextBox
			// 
			this.moduleLSIDTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.moduleLSIDTextBox.Location = new System.Drawing.Point(120, 114);
			this.moduleLSIDTextBox.Name = "moduleLSIDTextBox";
			this.moduleLSIDTextBox.ReadOnly = true;
			this.moduleLSIDTextBox.Size = new System.Drawing.Size(488, 20);
			this.moduleLSIDTextBox.TabIndex = 5;
			this.moduleLSIDTextBox.Text = "";
			// 
			// label6
			// 
			this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label6.Location = new System.Drawing.Point(16, 114);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(80, 16);
			this.label6.TabIndex = 4;
			this.label6.Text = "Module LSID:";
			this.label6.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// descriptionTextBox
			// 
			this.descriptionTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.descriptionTextBox.Location = new System.Drawing.Point(120, 48);
			this.descriptionTextBox.MaxLength = 65536;
			this.descriptionTextBox.Multiline = true;
			this.descriptionTextBox.Name = "descriptionTextBox";
			this.descriptionTextBox.ReadOnly = true;
			this.descriptionTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.descriptionTextBox.Size = new System.Drawing.Size(488, 64);
			this.descriptionTextBox.TabIndex = 3;
			this.descriptionTextBox.Text = "";
			// 
			// nameTextBox
			// 
			this.nameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.nameTextBox.Location = new System.Drawing.Point(120, 24);
			this.nameTextBox.MaxLength = 255;
			this.nameTextBox.Name = "nameTextBox";
			this.nameTextBox.ReadOnly = true;
			this.nameTextBox.Size = new System.Drawing.Size(488, 20);
			this.nameTextBox.TabIndex = 0;
			this.nameTextBox.Text = "";
			// 
			// descriptionLabel
			// 
			this.descriptionLabel.Location = new System.Drawing.Point(16, 48);
			this.descriptionLabel.Name = "descriptionLabel";
			this.descriptionLabel.Size = new System.Drawing.Size(112, 16);
			this.descriptionLabel.TabIndex = 2;
			this.descriptionLabel.Text = "Module Description:";
			this.descriptionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Module Name:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// parametersTabPage
			// 
			this.parametersTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.parametersTabPage.Controls.Add(this.datatypeGroupBox);
			this.parametersTabPage.Controls.Add(this.parametersGroupBox);
			this.parametersTabPage.Location = new System.Drawing.Point(4, 22);
			this.parametersTabPage.Name = "parametersTabPage";
			this.parametersTabPage.Size = new System.Drawing.Size(640, 310);
			this.parametersTabPage.TabIndex = 3;
			this.parametersTabPage.Text = "Parameters/Data Types";
			// 
			// datatypeGroupBox
			// 
			this.datatypeGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.datatypeGroupBox.Controls.Add(this.datatypeLinkLabel);
			this.datatypeGroupBox.Controls.Add(this.datatypeDataGrid);
			this.datatypeGroupBox.Location = new System.Drawing.Point(8, 160);
			this.datatypeGroupBox.Name = "datatypeGroupBox";
			this.datatypeGroupBox.Size = new System.Drawing.Size(624, 136);
			this.datatypeGroupBox.TabIndex = 1;
			this.datatypeGroupBox.TabStop = false;
			this.datatypeGroupBox.Text = "DataTypes";
			// 
			// datatypeLinkLabel
			// 
			this.datatypeLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.datatypeLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.datatypeLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.datatypeLinkLabel.Location = new System.Drawing.Point(528, 112);
			this.datatypeLinkLabel.Name = "datatypeLinkLabel";
			this.datatypeLinkLabel.Size = new System.Drawing.Size(80, 16);
			this.datatypeLinkLabel.TabIndex = 1;
			this.datatypeLinkLabel.TabStop = true;
			this.datatypeLinkLabel.Text = "Add Data Type";
			this.datatypeLinkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.datatypeLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.datatypeLinkLabel_LinkClicked);
			// 
			// datatypeDataGrid
			// 
			this.datatypeDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.datatypeDataGrid.CaptionVisible = false;
			this.datatypeDataGrid.DataMember = "";
			this.datatypeDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.datatypeDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.datatypeDataGrid.Location = new System.Drawing.Point(16, 24);
			this.datatypeDataGrid.Name = "datatypeDataGrid";
			this.datatypeDataGrid.ParentRowsVisible = false;
			this.datatypeDataGrid.ReadOnly = true;
			this.datatypeDataGrid.RowHeadersVisible = false;
			this.datatypeDataGrid.Size = new System.Drawing.Size(592, 88);
			this.datatypeDataGrid.TabIndex = 0;
			this.datatypeDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										 this.datatypeStyle});
			this.datatypeDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.datatypeDataGrid_LinkLabelClicked);
			// 
			// datatypeStyle
			// 
			this.datatypeStyle.DataGrid = this.datatypeDataGrid;
			this.datatypeStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											this.datatypeNameColumn,
																											this.indexColumn,
																											this.datatypeColumn,
																											this.directionColumn,
																											this.datatypeUpdateColumn,
																											this.datatypeDeleteColumn});
			this.datatypeStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.datatypeStyle.MappingName = "tblVersionDataType";
			this.datatypeStyle.ReadOnly = true;
			this.datatypeStyle.RowHeadersVisible = false;
			// 
			// datatypeNameColumn
			// 
			this.datatypeNameColumn.Format = "";
			this.datatypeNameColumn.FormatInfo = null;
			this.datatypeNameColumn.HeaderText = "Name";
			this.datatypeNameColumn.MappingName = "fldName";
			this.datatypeNameColumn.ReadOnly = true;
			this.datatypeNameColumn.Width = 75;
			// 
			// indexColumn
			// 
			this.indexColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.indexColumn.Format = "";
			this.indexColumn.FormatInfo = null;
			this.indexColumn.HeaderText = "#";
			this.indexColumn.MappingName = "fldIndex";
			this.indexColumn.ReadOnly = true;
			this.indexColumn.Width = 30;
			// 
			// datatypeColumn
			// 
			this.datatypeColumn.Format = "";
			this.datatypeColumn.FormatInfo = null;
			this.datatypeColumn.HeaderText = "Data Type";
			this.datatypeColumn.MappingName = "fldDataType";
			this.datatypeColumn.ReadOnly = true;
			this.datatypeColumn.Width = 270;
			// 
			// directionColumn
			// 
			this.directionColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.directionColumn.Format = "";
			this.directionColumn.FormatInfo = null;
			this.directionColumn.HeaderText = "Direction";
			this.directionColumn.MappingName = "fldDirection";
			this.directionColumn.ReadOnly = true;
			this.directionColumn.Width = 60;
			// 
			// datatypeUpdateColumn
			// 
			this.datatypeUpdateColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.datatypeUpdateColumn.Format = "";
			this.datatypeUpdateColumn.FormatInfo = null;
			this.datatypeUpdateColumn.HeaderText = "Update";
			this.datatypeUpdateColumn.MappingName = "fldUpdate";
			this.datatypeUpdateColumn.NullText = "Update";
			this.datatypeUpdateColumn.ReadOnly = true;
			this.datatypeUpdateColumn.Width = 50;
			// 
			// datatypeDeleteColumn
			// 
			this.datatypeDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.datatypeDeleteColumn.Format = "";
			this.datatypeDeleteColumn.FormatInfo = null;
			this.datatypeDeleteColumn.HeaderText = "Delete";
			this.datatypeDeleteColumn.MappingName = "fldDelete";
			this.datatypeDeleteColumn.NullText = "Delete";
			this.datatypeDeleteColumn.ReadOnly = true;
			this.datatypeDeleteColumn.Width = 50;
			// 
			// parametersGroupBox
			// 
			this.parametersGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.parametersGroupBox.Controls.Add(this.parameterLinkLabel);
			this.parametersGroupBox.Controls.Add(this.parameterDataGrid);
			this.parametersGroupBox.Location = new System.Drawing.Point(8, 8);
			this.parametersGroupBox.Name = "parametersGroupBox";
			this.parametersGroupBox.Size = new System.Drawing.Size(624, 144);
			this.parametersGroupBox.TabIndex = 0;
			this.parametersGroupBox.TabStop = false;
			this.parametersGroupBox.Text = "Parameters";
			// 
			// parameterLinkLabel
			// 
			this.parameterLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.parameterLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.parameterLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.parameterLinkLabel.Location = new System.Drawing.Point(520, 120);
			this.parameterLinkLabel.Name = "parameterLinkLabel";
			this.parameterLinkLabel.Size = new System.Drawing.Size(88, 16);
			this.parameterLinkLabel.TabIndex = 1;
			this.parameterLinkLabel.TabStop = true;
			this.parameterLinkLabel.Text = "Add Parameter";
			this.parameterLinkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.parameterLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.parameterLinkLabel_LinkClicked);
			// 
			// parameterDataGrid
			// 
			this.parameterDataGrid.AllowNavigation = false;
			this.parameterDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.parameterDataGrid.CaptionVisible = false;
			this.parameterDataGrid.DataMember = "";
			this.parameterDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.parameterDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.parameterDataGrid.Location = new System.Drawing.Point(16, 24);
			this.parameterDataGrid.Name = "parameterDataGrid";
			this.parameterDataGrid.ParentRowsVisible = false;
			this.parameterDataGrid.ReadOnly = true;
			this.parameterDataGrid.RowHeadersVisible = false;
			this.parameterDataGrid.Size = new System.Drawing.Size(592, 96);
			this.parameterDataGrid.TabIndex = 0;
			this.parameterDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										  this.parameterStyle});
			this.parameterDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.parameterDataGrid_LinkLabelClicked);
			// 
			// parameterStyle
			// 
			this.parameterStyle.DataGrid = this.parameterDataGrid;
			this.parameterStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											 this.parameterNameColumn,
																											 this.parameterDescriptionColumn,
																											 this.parameterTypeColumn,
																											 this.parameterMinColumn,
																											 this.parameterMaxColumn,
																											 this.parameterUpdateColumn,
																											 this.parameterDeleteColumn});
			this.parameterStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.parameterStyle.MappingName = "tblParameter";
			this.parameterStyle.ReadOnly = true;
			this.parameterStyle.RowHeadersVisible = false;
			// 
			// parameterNameColumn
			// 
			this.parameterNameColumn.Format = "";
			this.parameterNameColumn.FormatInfo = null;
			this.parameterNameColumn.HeaderText = "Name";
			this.parameterNameColumn.MappingName = "fldName";
			this.parameterNameColumn.ReadOnly = true;
			this.parameterNameColumn.Width = 120;
			// 
			// parameterDescriptionColumn
			// 
			this.parameterDescriptionColumn.Format = "";
			this.parameterDescriptionColumn.FormatInfo = null;
			this.parameterDescriptionColumn.HeaderText = "Description";
			this.parameterDescriptionColumn.MappingName = "fldDescription";
			this.parameterDescriptionColumn.ReadOnly = true;
			this.parameterDescriptionColumn.Width = 200;
			// 
			// parameterTypeColumn
			// 
			this.parameterTypeColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.parameterTypeColumn.Format = "";
			this.parameterTypeColumn.FormatInfo = null;
			this.parameterTypeColumn.HeaderText = "Type";
			this.parameterTypeColumn.MappingName = "fldParameterType";
			this.parameterTypeColumn.ReadOnly = true;
			this.parameterTypeColumn.Width = 70;
			// 
			// parameterMinColumn
			// 
			this.parameterMinColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.parameterMinColumn.Format = "";
			this.parameterMinColumn.FormatInfo = null;
			this.parameterMinColumn.HeaderText = "Min";
			this.parameterMinColumn.MappingName = "fldMinimum";
			this.parameterMinColumn.NullText = "n/a";
			this.parameterMinColumn.ReadOnly = true;
			this.parameterMinColumn.Width = 50;
			// 
			// parameterMaxColumn
			// 
			this.parameterMaxColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.parameterMaxColumn.Format = "";
			this.parameterMaxColumn.FormatInfo = null;
			this.parameterMaxColumn.HeaderText = "Max";
			this.parameterMaxColumn.MappingName = "fldMaximum";
			this.parameterMaxColumn.NullText = "n/a";
			this.parameterMaxColumn.ReadOnly = true;
			this.parameterMaxColumn.Width = 50;
			// 
			// parameterUpdateColumn
			// 
			this.parameterUpdateColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.parameterUpdateColumn.Format = "";
			this.parameterUpdateColumn.FormatInfo = null;
			this.parameterUpdateColumn.HeaderText = "Update";
			this.parameterUpdateColumn.MappingName = "fldUpdate";
			this.parameterUpdateColumn.NullText = "Update";
			this.parameterUpdateColumn.ReadOnly = true;
			this.parameterUpdateColumn.Width = 50;
			// 
			// parameterDeleteColumn
			// 
			this.parameterDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.parameterDeleteColumn.Format = "";
			this.parameterDeleteColumn.FormatInfo = null;
			this.parameterDeleteColumn.HeaderText = "Delete";
			this.parameterDeleteColumn.MappingName = "fldDelete";
			this.parameterDeleteColumn.NullText = "Delete";
			this.parameterDeleteColumn.ReadOnly = true;
			this.parameterDeleteColumn.Width = 50;
			// 
			// programmingTabPage
			// 
			this.programmingTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.programmingTabPage.Controls.Add(this.prerequisiteGroupBox);
			this.programmingTabPage.Controls.Add(this.programmingGroupBox);
			this.programmingTabPage.Location = new System.Drawing.Point(4, 22);
			this.programmingTabPage.Name = "programmingTabPage";
			this.programmingTabPage.Size = new System.Drawing.Size(640, 310);
			this.programmingTabPage.TabIndex = 4;
			this.programmingTabPage.Text = "Programming/Prerequisites";
			// 
			// prerequisiteGroupBox
			// 
			this.prerequisiteGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.prerequisiteGroupBox.Controls.Add(this.prerequisiteTextBox);
			this.prerequisiteGroupBox.Location = new System.Drawing.Point(8, 192);
			this.prerequisiteGroupBox.Name = "prerequisiteGroupBox";
			this.prerequisiteGroupBox.Size = new System.Drawing.Size(624, 112);
			this.prerequisiteGroupBox.TabIndex = 2;
			this.prerequisiteGroupBox.TabStop = false;
			this.prerequisiteGroupBox.Text = "Prerequisites";
			// 
			// prerequisiteTextBox
			// 
			this.prerequisiteTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.prerequisiteTextBox.Location = new System.Drawing.Point(16, 24);
			this.prerequisiteTextBox.MaxLength = 65536;
			this.prerequisiteTextBox.Multiline = true;
			this.prerequisiteTextBox.Name = "prerequisiteTextBox";
			this.prerequisiteTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.prerequisiteTextBox.Size = new System.Drawing.Size(592, 72);
			this.prerequisiteTextBox.TabIndex = 0;
			this.prerequisiteTextBox.Text = "";
			this.prerequisiteTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// programmingGroupBox
			// 
			this.programmingGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.programmingGroupBox.Controls.Add(this.complianceLinkLabel);
			this.programmingGroupBox.Controls.Add(this.complianceComboBox);
			this.programmingGroupBox.Controls.Add(this.languageLinkLabel);
			this.programmingGroupBox.Controls.Add(this.languageComboBox);
			this.programmingGroupBox.Controls.Add(this.osLinkLabel);
			this.programmingGroupBox.Controls.Add(this.osComboBox);
			this.programmingGroupBox.Controls.Add(this.complianceDataGrid);
			this.programmingGroupBox.Controls.Add(this.languageDataGrid);
			this.programmingGroupBox.Controls.Add(this.osDataGrid);
			this.programmingGroupBox.Controls.Add(this.complianceLabel);
			this.programmingGroupBox.Controls.Add(this.languageLabel);
			this.programmingGroupBox.Controls.Add(this.osLabel);
			this.programmingGroupBox.Location = new System.Drawing.Point(8, 8);
			this.programmingGroupBox.Name = "programmingGroupBox";
			this.programmingGroupBox.Size = new System.Drawing.Size(624, 176);
			this.programmingGroupBox.TabIndex = 1;
			this.programmingGroupBox.TabStop = false;
			this.programmingGroupBox.Text = "Programming";
			// 
			// complianceLinkLabel
			// 
			this.complianceLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.complianceLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.complianceLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.complianceLinkLabel.Location = new System.Drawing.Point(448, 144);
			this.complianceLinkLabel.Name = "complianceLinkLabel";
			this.complianceLinkLabel.Size = new System.Drawing.Size(152, 16);
			this.complianceLinkLabel.TabIndex = 13;
			this.complianceLinkLabel.TabStop = true;
			this.complianceLinkLabel.Text = "Add Compliance";
			this.complianceLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
			this.complianceLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.complianceLinkLabel_LinkClicked);
			// 
			// complianceComboBox
			// 
			this.complianceComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.complianceComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.complianceComboBox.Location = new System.Drawing.Point(440, 120);
			this.complianceComboBox.Name = "complianceComboBox";
			this.complianceComboBox.Size = new System.Drawing.Size(160, 21);
			this.complianceComboBox.TabIndex = 12;
			// 
			// languageLinkLabel
			// 
			this.languageLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.languageLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.languageLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.languageLinkLabel.Location = new System.Drawing.Point(448, 96);
			this.languageLinkLabel.Name = "languageLinkLabel";
			this.languageLinkLabel.Size = new System.Drawing.Size(152, 16);
			this.languageLinkLabel.TabIndex = 11;
			this.languageLinkLabel.TabStop = true;
			this.languageLinkLabel.Text = "Add Language";
			this.languageLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
			this.languageLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.languageLinkLabel_LinkClicked);
			// 
			// languageComboBox
			// 
			this.languageComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.languageComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.languageComboBox.Location = new System.Drawing.Point(440, 72);
			this.languageComboBox.Name = "languageComboBox";
			this.languageComboBox.Size = new System.Drawing.Size(160, 21);
			this.languageComboBox.TabIndex = 10;
			// 
			// osLinkLabel
			// 
			this.osLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.osLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.osLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.osLinkLabel.Location = new System.Drawing.Point(448, 48);
			this.osLinkLabel.Name = "osLinkLabel";
			this.osLinkLabel.Size = new System.Drawing.Size(152, 16);
			this.osLinkLabel.TabIndex = 9;
			this.osLinkLabel.TabStop = true;
			this.osLinkLabel.Text = "Add Operating System";
			this.osLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
			this.osLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.osLinkLabel_LinkClicked);
			// 
			// osComboBox
			// 
			this.osComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.osComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.osComboBox.Location = new System.Drawing.Point(440, 24);
			this.osComboBox.Name = "osComboBox";
			this.osComboBox.Size = new System.Drawing.Size(160, 21);
			this.osComboBox.TabIndex = 8;
			// 
			// complianceDataGrid
			// 
			this.complianceDataGrid.AllowNavigation = false;
			this.complianceDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.complianceDataGrid.CaptionVisible = false;
			this.complianceDataGrid.ColumnHeadersVisible = false;
			this.complianceDataGrid.DataMember = "";
			this.complianceDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.complianceDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.complianceDataGrid.Location = new System.Drawing.Point(88, 120);
			this.complianceDataGrid.Name = "complianceDataGrid";
			this.complianceDataGrid.ParentRowsVisible = false;
			this.complianceDataGrid.ReadOnly = true;
			this.complianceDataGrid.RowHeadersVisible = false;
			this.complianceDataGrid.Size = new System.Drawing.Size(336, 40);
			this.complianceDataGrid.TabIndex = 7;
			this.complianceDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										   this.complianceStyle});
			this.complianceDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// complianceStyle
			// 
			this.complianceStyle.ColumnHeadersVisible = false;
			this.complianceStyle.DataGrid = this.complianceDataGrid;
			this.complianceStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											  this.complianceColumn,
																											  this.complianceDeleteColumn});
			this.complianceStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.complianceStyle.LinkColor = System.Drawing.SystemColors.Highlight;
			this.complianceStyle.MappingName = "tblVersionCompliance";
			this.complianceStyle.ReadOnly = true;
			this.complianceStyle.RowHeadersVisible = false;
			// 
			// complianceColumn
			// 
			this.complianceColumn.Format = "";
			this.complianceColumn.FormatInfo = null;
			this.complianceColumn.HeaderText = "Compliance";
			this.complianceColumn.MappingName = "fldCompliance";
			this.complianceColumn.ReadOnly = true;
			this.complianceColumn.Width = 261;
			// 
			// complianceDeleteColumn
			// 
			this.complianceDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.complianceDeleteColumn.Format = "";
			this.complianceDeleteColumn.FormatInfo = null;
			this.complianceDeleteColumn.HeaderText = "Delete";
			this.complianceDeleteColumn.MappingName = "fldDelete";
			this.complianceDeleteColumn.NullText = "Delete";
			this.complianceDeleteColumn.ReadOnly = true;
			this.complianceDeleteColumn.Width = 70;
			// 
			// languageDataGrid
			// 
			this.languageDataGrid.AllowNavigation = false;
			this.languageDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.languageDataGrid.CaptionVisible = false;
			this.languageDataGrid.ColumnHeadersVisible = false;
			this.languageDataGrid.DataMember = "";
			this.languageDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.languageDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.languageDataGrid.Location = new System.Drawing.Point(88, 72);
			this.languageDataGrid.Name = "languageDataGrid";
			this.languageDataGrid.ParentRowsVisible = false;
			this.languageDataGrid.ReadOnly = true;
			this.languageDataGrid.RowHeadersVisible = false;
			this.languageDataGrid.Size = new System.Drawing.Size(336, 40);
			this.languageDataGrid.TabIndex = 6;
			this.languageDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										 this.languageStyle});
			this.languageDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// languageStyle
			// 
			this.languageStyle.ColumnHeadersVisible = false;
			this.languageStyle.DataGrid = this.languageDataGrid;
			this.languageStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											this.languageColumn,
																											this.languageDeleteColumn});
			this.languageStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.languageStyle.LinkColor = System.Drawing.SystemColors.Highlight;
			this.languageStyle.MappingName = "tblVersionLanguage";
			this.languageStyle.ReadOnly = true;
			this.languageStyle.RowHeadersVisible = false;
			// 
			// languageColumn
			// 
			this.languageColumn.Format = "";
			this.languageColumn.FormatInfo = null;
			this.languageColumn.HeaderText = "Language";
			this.languageColumn.MappingName = "fldLanguage";
			this.languageColumn.ReadOnly = true;
			this.languageColumn.Width = 261;
			// 
			// languageDeleteColumn
			// 
			this.languageDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.languageDeleteColumn.Format = "";
			this.languageDeleteColumn.FormatInfo = null;
			this.languageDeleteColumn.HeaderText = "Delete";
			this.languageDeleteColumn.MappingName = "fldDelete";
			this.languageDeleteColumn.NullText = "Delete";
			this.languageDeleteColumn.ReadOnly = true;
			this.languageDeleteColumn.Width = 70;
			// 
			// osDataGrid
			// 
			this.osDataGrid.AllowNavigation = false;
			this.osDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.osDataGrid.CaptionVisible = false;
			this.osDataGrid.ColumnHeadersVisible = false;
			this.osDataGrid.DataMember = "";
			this.osDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.osDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.osDataGrid.Location = new System.Drawing.Point(88, 24);
			this.osDataGrid.Name = "osDataGrid";
			this.osDataGrid.ParentRowsVisible = false;
			this.osDataGrid.ReadOnly = true;
			this.osDataGrid.RowHeadersVisible = false;
			this.osDataGrid.Size = new System.Drawing.Size(336, 40);
			this.osDataGrid.TabIndex = 5;
			this.osDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																								   this.osStyle});
			this.osDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// osStyle
			// 
			this.osStyle.ColumnHeadersVisible = false;
			this.osStyle.DataGrid = this.osDataGrid;
			this.osStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																									  this.osColumn,
																									  this.osDeleteColumn});
			this.osStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.osStyle.LinkColor = System.Drawing.SystemColors.Highlight;
			this.osStyle.MappingName = "tblVersionOS";
			this.osStyle.ReadOnly = true;
			this.osStyle.RowHeadersVisible = false;
			// 
			// osColumn
			// 
			this.osColumn.Format = "";
			this.osColumn.FormatInfo = null;
			this.osColumn.HeaderText = "Oper. System";
			this.osColumn.MappingName = "fldOS";
			this.osColumn.ReadOnly = true;
			this.osColumn.Width = 261;
			// 
			// osDeleteColumn
			// 
			this.osDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.osDeleteColumn.Format = "";
			this.osDeleteColumn.FormatInfo = null;
			this.osDeleteColumn.HeaderText = "Delete";
			this.osDeleteColumn.MappingName = "fldDelete";
			this.osDeleteColumn.NullText = "Delete";
			this.osDeleteColumn.ReadOnly = true;
			this.osDeleteColumn.Width = 70;
			// 
			// complianceLabel
			// 
			this.complianceLabel.Location = new System.Drawing.Point(16, 120);
			this.complianceLabel.Name = "complianceLabel";
			this.complianceLabel.Size = new System.Drawing.Size(72, 16);
			this.complianceLabel.TabIndex = 4;
			this.complianceLabel.Text = "Compliance:";
			this.complianceLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// languageLabel
			// 
			this.languageLabel.Location = new System.Drawing.Point(16, 72);
			this.languageLabel.Name = "languageLabel";
			this.languageLabel.Size = new System.Drawing.Size(64, 16);
			this.languageLabel.TabIndex = 1;
			this.languageLabel.Text = "Language:";
			this.languageLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// osLabel
			// 
			this.osLabel.Location = new System.Drawing.Point(16, 24);
			this.osLabel.Name = "osLabel";
			this.osLabel.Size = new System.Drawing.Size(80, 16);
			this.osLabel.TabIndex = 0;
			this.osLabel.Text = "Oper. System:";
			this.osLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// designTabPage
			// 
			this.designTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.designTabPage.Controls.Add(this.designTextBox);
			this.designTabPage.Controls.Add(this.designLabel);
			this.designTabPage.Controls.Add(this.implementationTextBox);
			this.designTabPage.Controls.Add(this.implementationLabel);
			this.designTabPage.Controls.Add(this.interfaceTextBox);
			this.designTabPage.Controls.Add(this.interfaceLabel);
			this.designTabPage.Location = new System.Drawing.Point(4, 22);
			this.designTabPage.Name = "designTabPage";
			this.designTabPage.Size = new System.Drawing.Size(640, 310);
			this.designTabPage.TabIndex = 2;
			this.designTabPage.Text = "Design Info.";
			// 
			// designTextBox
			// 
			this.designTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.designTextBox.Location = new System.Drawing.Point(96, 16);
			this.designTextBox.MaxLength = 65536;
			this.designTextBox.Multiline = true;
			this.designTextBox.Name = "designTextBox";
			this.designTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.designTextBox.Size = new System.Drawing.Size(528, 88);
			this.designTextBox.TabIndex = 9;
			this.designTextBox.Text = "";
			this.designTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// designLabel
			// 
			this.designLabel.Location = new System.Drawing.Point(16, 16);
			this.designLabel.Name = "designLabel";
			this.designLabel.Size = new System.Drawing.Size(48, 16);
			this.designLabel.TabIndex = 8;
			this.designLabel.Text = "Design:";
			// 
			// implementationTextBox
			// 
			this.implementationTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.implementationTextBox.Location = new System.Drawing.Point(96, 112);
			this.implementationTextBox.MaxLength = 65536;
			this.implementationTextBox.Multiline = true;
			this.implementationTextBox.Name = "implementationTextBox";
			this.implementationTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.implementationTextBox.Size = new System.Drawing.Size(528, 88);
			this.implementationTextBox.TabIndex = 7;
			this.implementationTextBox.Text = "";
			this.implementationTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// implementationLabel
			// 
			this.implementationLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.implementationLabel.Location = new System.Drawing.Point(16, 112);
			this.implementationLabel.Name = "implementationLabel";
			this.implementationLabel.Size = new System.Drawing.Size(88, 16);
			this.implementationLabel.TabIndex = 6;
			this.implementationLabel.Text = "Implementation:";
			// 
			// interfaceTextBox
			// 
			this.interfaceTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.interfaceTextBox.Location = new System.Drawing.Point(96, 208);
			this.interfaceTextBox.MaxLength = 65536;
			this.interfaceTextBox.Multiline = true;
			this.interfaceTextBox.Name = "interfaceTextBox";
			this.interfaceTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.interfaceTextBox.Size = new System.Drawing.Size(528, 88);
			this.interfaceTextBox.TabIndex = 5;
			this.interfaceTextBox.Text = "";
			this.interfaceTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// interfaceLabel
			// 
			this.interfaceLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.interfaceLabel.Location = new System.Drawing.Point(16, 208);
			this.interfaceLabel.Name = "interfaceLabel";
			this.interfaceLabel.Size = new System.Drawing.Size(56, 16);
			this.interfaceLabel.TabIndex = 4;
			this.interfaceLabel.Text = "Interface:";
			// 
			// notesTabPage
			// 
			this.notesTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.notesTabPage.Controls.Add(this.notesGroupBox);
			this.notesTabPage.Controls.Add(this.evaluationGroupBox);
			this.notesTabPage.Location = new System.Drawing.Point(4, 22);
			this.notesTabPage.Name = "notesTabPage";
			this.notesTabPage.Size = new System.Drawing.Size(640, 310);
			this.notesTabPage.TabIndex = 5;
			this.notesTabPage.Text = "Evaluation/Notes";
			// 
			// notesGroupBox
			// 
			this.notesGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.notesGroupBox.Controls.Add(this.notesTextBox);
			this.notesGroupBox.Location = new System.Drawing.Point(8, 160);
			this.notesGroupBox.Name = "notesGroupBox";
			this.notesGroupBox.Size = new System.Drawing.Size(624, 144);
			this.notesGroupBox.TabIndex = 2;
			this.notesGroupBox.TabStop = false;
			this.notesGroupBox.Text = "Notes";
			// 
			// notesTextBox
			// 
			this.notesTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.notesTextBox.Location = new System.Drawing.Point(16, 24);
			this.notesTextBox.MaxLength = 65536;
			this.notesTextBox.Multiline = true;
			this.notesTextBox.Name = "notesTextBox";
			this.notesTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.notesTextBox.Size = new System.Drawing.Size(592, 104);
			this.notesTextBox.TabIndex = 3;
			this.notesTextBox.Text = "";
			this.notesTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// evaluationGroupBox
			// 
			this.evaluationGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.evaluationGroupBox.Controls.Add(this.evaluationTextBox);
			this.evaluationGroupBox.Location = new System.Drawing.Point(8, 8);
			this.evaluationGroupBox.Name = "evaluationGroupBox";
			this.evaluationGroupBox.Size = new System.Drawing.Size(624, 144);
			this.evaluationGroupBox.TabIndex = 1;
			this.evaluationGroupBox.TabStop = false;
			this.evaluationGroupBox.Text = "Evaluation";
			// 
			// evaluationTextBox
			// 
			this.evaluationTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.evaluationTextBox.Location = new System.Drawing.Point(16, 24);
			this.evaluationTextBox.MaxLength = 65536;
			this.evaluationTextBox.Multiline = true;
			this.evaluationTextBox.Name = "evaluationTextBox";
			this.evaluationTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.evaluationTextBox.Size = new System.Drawing.Size(592, 104);
			this.evaluationTextBox.TabIndex = 2;
			this.evaluationTextBox.Text = "";
			this.evaluationTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// filesTabPage
			// 
			this.filesTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.filesTabPage.Controls.Add(this.dllButton);
			this.filesTabPage.Controls.Add(this.dllTextBox);
			this.filesTabPage.Location = new System.Drawing.Point(4, 22);
			this.filesTabPage.Name = "filesTabPage";
			this.filesTabPage.Size = new System.Drawing.Size(640, 310);
			this.filesTabPage.TabIndex = 8;
			this.filesTabPage.Text = "Files";
			// 
			// dllButton
			// 
			this.dllButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.dllButton.BackColor = System.Drawing.SystemColors.Control;
			this.dllButton.Location = new System.Drawing.Point(464, 128);
			this.dllButton.Name = "dllButton";
			this.dllButton.Size = new System.Drawing.Size(75, 24);
			this.dllButton.TabIndex = 1;
			this.dllButton.Text = "Select File";
			this.dllButton.Click += new System.EventHandler(this.dllButton_Click);
			// 
			// dllTextBox
			// 
			this.dllTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.dllTextBox.Location = new System.Drawing.Point(96, 128);
			this.dllTextBox.Name = "dllTextBox";
			this.dllTextBox.ReadOnly = true;
			this.dllTextBox.Size = new System.Drawing.Size(360, 20);
			this.dllTextBox.TabIndex = 0;
			this.dllTextBox.Text = "";
			// 
			// historyTabPage
			// 
			this.historyTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.historyTabPage.Controls.Add(this.historyGroupBox);
			this.historyTabPage.Controls.Add(this.referencesGroupBox);
			this.historyTabPage.Location = new System.Drawing.Point(4, 22);
			this.historyTabPage.Name = "historyTabPage";
			this.historyTabPage.Size = new System.Drawing.Size(640, 310);
			this.historyTabPage.TabIndex = 6;
			this.historyTabPage.Text = "References/History";
			// 
			// historyGroupBox
			// 
			this.historyGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.historyGroupBox.Controls.Add(this.historyTextBox);
			this.historyGroupBox.Controls.Add(this.historyLinkLabel);
			this.historyGroupBox.Controls.Add(this.historyDataGrid);
			this.historyGroupBox.Location = new System.Drawing.Point(8, 160);
			this.historyGroupBox.Name = "historyGroupBox";
			this.historyGroupBox.Size = new System.Drawing.Size(624, 144);
			this.historyGroupBox.TabIndex = 3;
			this.historyGroupBox.TabStop = false;
			this.historyGroupBox.Text = "History";
			// 
			// historyTextBox
			// 
			this.historyTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.historyTextBox.Location = new System.Drawing.Point(16, 116);
			this.historyTextBox.MaxLength = 255;
			this.historyTextBox.Name = "historyTextBox";
			this.historyTextBox.Size = new System.Drawing.Size(504, 20);
			this.historyTextBox.TabIndex = 3;
			this.historyTextBox.Text = "";
			// 
			// historyLinkLabel
			// 
			this.historyLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.historyLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.historyLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.historyLinkLabel.Location = new System.Drawing.Point(528, 116);
			this.historyLinkLabel.Name = "historyLinkLabel";
			this.historyLinkLabel.Size = new System.Drawing.Size(80, 16);
			this.historyLinkLabel.TabIndex = 1;
			this.historyLinkLabel.TabStop = true;
			this.historyLinkLabel.Text = "Add History";
			this.historyLinkLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
			this.historyLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.historyLinkLabel_LinkClicked);
			// 
			// historyDataGrid
			// 
			this.historyDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.historyDataGrid.CaptionVisible = false;
			this.historyDataGrid.ColumnHeadersVisible = false;
			this.historyDataGrid.DataMember = "";
			this.historyDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.historyDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.historyDataGrid.Location = new System.Drawing.Point(16, 24);
			this.historyDataGrid.Name = "historyDataGrid";
			this.historyDataGrid.ParentRowsVisible = false;
			this.historyDataGrid.ReadOnly = true;
			this.historyDataGrid.RowHeadersVisible = false;
			this.historyDataGrid.Size = new System.Drawing.Size(592, 90);
			this.historyDataGrid.TabIndex = 0;
			this.historyDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										this.historyStyle});
			this.historyDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// historyStyle
			// 
			this.historyStyle.ColumnHeadersVisible = false;
			this.historyStyle.DataGrid = this.historyDataGrid;
			this.historyStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																										   this.historyColumn,
																										   this.historyDeleteColumn});
			this.historyStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.historyStyle.MappingName = "tblHistory";
			this.historyStyle.ReadOnly = true;
			this.historyStyle.RowHeadersVisible = false;
			// 
			// historyColumn
			// 
			this.historyColumn.Format = "";
			this.historyColumn.FormatInfo = null;
			this.historyColumn.HeaderText = "History";
			this.historyColumn.MappingName = "fldItem";
			this.historyColumn.ReadOnly = true;
			this.historyColumn.Width = 517;
			// 
			// historyDeleteColumn
			// 
			this.historyDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.historyDeleteColumn.Format = "";
			this.historyDeleteColumn.FormatInfo = null;
			this.historyDeleteColumn.HeaderText = "Delete";
			this.historyDeleteColumn.MappingName = "fldDelete";
			this.historyDeleteColumn.NullText = "Delete";
			this.historyDeleteColumn.ReadOnly = true;
			this.historyDeleteColumn.Width = 70;
			// 
			// referencesGroupBox
			// 
			this.referencesGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.referencesGroupBox.Controls.Add(this.referenceTextBox);
			this.referencesGroupBox.Controls.Add(this.referenceDataGrid);
			this.referencesGroupBox.Controls.Add(this.referenceLinkLabel);
			this.referencesGroupBox.Location = new System.Drawing.Point(8, 8);
			this.referencesGroupBox.Name = "referencesGroupBox";
			this.referencesGroupBox.Size = new System.Drawing.Size(624, 144);
			this.referencesGroupBox.TabIndex = 2;
			this.referencesGroupBox.TabStop = false;
			this.referencesGroupBox.Text = "References";
			// 
			// referenceTextBox
			// 
			this.referenceTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.referenceTextBox.Location = new System.Drawing.Point(16, 116);
			this.referenceTextBox.MaxLength = 255;
			this.referenceTextBox.Name = "referenceTextBox";
			this.referenceTextBox.Size = new System.Drawing.Size(504, 20);
			this.referenceTextBox.TabIndex = 2;
			this.referenceTextBox.Text = "";
			// 
			// referenceDataGrid
			// 
			this.referenceDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.referenceDataGrid.CaptionVisible = false;
			this.referenceDataGrid.ColumnHeadersVisible = false;
			this.referenceDataGrid.DataMember = "";
			this.referenceDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.referenceDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.referenceDataGrid.Location = new System.Drawing.Point(16, 24);
			this.referenceDataGrid.Name = "referenceDataGrid";
			this.referenceDataGrid.ParentRowsVisible = false;
			this.referenceDataGrid.ReadOnly = true;
			this.referenceDataGrid.RowHeadersVisible = false;
			this.referenceDataGrid.Size = new System.Drawing.Size(592, 90);
			this.referenceDataGrid.TabIndex = 0;
			this.referenceDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																										  this.referenceStyle});
			this.referenceDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// referenceStyle
			// 
			this.referenceStyle.ColumnHeadersVisible = false;
			this.referenceStyle.DataGrid = this.referenceDataGrid;
			this.referenceStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											 this.referenceColumn,
																											 this.referenceDeleteColumn});
			this.referenceStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.referenceStyle.MappingName = "tblReference";
			this.referenceStyle.ReadOnly = true;
			this.referenceStyle.RowHeadersVisible = false;
			// 
			// referenceColumn
			// 
			this.referenceColumn.Format = "";
			this.referenceColumn.FormatInfo = null;
			this.referenceColumn.HeaderText = "Reference";
			this.referenceColumn.MappingName = "fldItem";
			this.referenceColumn.ReadOnly = true;
			this.referenceColumn.Width = 517;
			// 
			// referenceDeleteColumn
			// 
			this.referenceDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.referenceDeleteColumn.Format = "";
			this.referenceDeleteColumn.FormatInfo = null;
			this.referenceDeleteColumn.HeaderText = "Delete";
			this.referenceDeleteColumn.MappingName = "fldDelete";
			this.referenceDeleteColumn.NullText = "Delete";
			this.referenceDeleteColumn.ReadOnly = true;
			this.referenceDeleteColumn.Width = 70;
			// 
			// referenceLinkLabel
			// 
			this.referenceLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.referenceLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.referenceLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.referenceLinkLabel.Location = new System.Drawing.Point(520, 116);
			this.referenceLinkLabel.Name = "referenceLinkLabel";
			this.referenceLinkLabel.Size = new System.Drawing.Size(88, 16);
			this.referenceLinkLabel.TabIndex = 1;
			this.referenceLinkLabel.TabStop = true;
			this.referenceLinkLabel.Text = "Add Reference";
			this.referenceLinkLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
			this.referenceLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.referenceLinkLabel_LinkClicked);
			// 
			// descriptionTabPage
			// 
			this.descriptionTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.descriptionTabPage.Controls.Add(this.classificationGroupBox);
			this.descriptionTabPage.Controls.Add(this.abstractGroupBox);
			this.descriptionTabPage.Location = new System.Drawing.Point(4, 22);
			this.descriptionTabPage.Name = "descriptionTabPage";
			this.descriptionTabPage.Size = new System.Drawing.Size(640, 310);
			this.descriptionTabPage.TabIndex = 1;
			this.descriptionTabPage.Text = "Description/Classification";
			// 
			// classificationGroupBox
			// 
			this.classificationGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.classificationGroupBox.Controls.Add(this.classificationLinkLabel);
			this.classificationGroupBox.Controls.Add(this.neuralTextBox);
			this.classificationGroupBox.Controls.Add(this.neuralLabel);
			this.classificationGroupBox.Controls.Add(this.classificationDataGrid);
			this.classificationGroupBox.Location = new System.Drawing.Point(8, 152);
			this.classificationGroupBox.Name = "classificationGroupBox";
			this.classificationGroupBox.Size = new System.Drawing.Size(624, 152);
			this.classificationGroupBox.TabIndex = 1;
			this.classificationGroupBox.TabStop = false;
			this.classificationGroupBox.Text = "Classification";
			// 
			// classificationLinkLabel
			// 
			this.classificationLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.classificationLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.classificationLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.classificationLinkLabel.Location = new System.Drawing.Point(512, 99);
			this.classificationLinkLabel.Name = "classificationLinkLabel";
			this.classificationLinkLabel.Size = new System.Drawing.Size(96, 16);
			this.classificationLinkLabel.TabIndex = 3;
			this.classificationLinkLabel.TabStop = true;
			this.classificationLinkLabel.Text = "Add Classification";
			this.classificationLinkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.classificationLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.classificationLinkLabel_LinkClicked);
			// 
			// neuralTextBox
			// 
			this.neuralTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.neuralTextBox.Location = new System.Drawing.Point(88, 120);
			this.neuralTextBox.MaxLength = 255;
			this.neuralTextBox.Name = "neuralTextBox";
			this.neuralTextBox.Size = new System.Drawing.Size(520, 20);
			this.neuralTextBox.TabIndex = 2;
			this.neuralTextBox.Text = "";
			this.neuralTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// neuralLabel
			// 
			this.neuralLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.neuralLabel.Location = new System.Drawing.Point(16, 120);
			this.neuralLabel.Name = "neuralLabel";
			this.neuralLabel.Size = new System.Drawing.Size(72, 16);
			this.neuralLabel.TabIndex = 1;
			this.neuralLabel.Text = "Neural Entity:";
			this.neuralLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
			// 
			// classificationDataGrid
			// 
			this.classificationDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.classificationDataGrid.CaptionVisible = false;
			this.classificationDataGrid.DataMember = "";
			this.classificationDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.classificationDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.classificationDataGrid.Location = new System.Drawing.Point(16, 24);
			this.classificationDataGrid.Name = "classificationDataGrid";
			this.classificationDataGrid.ParentRowsVisible = false;
			this.classificationDataGrid.ReadOnly = true;
			this.classificationDataGrid.RowHeadersVisible = false;
			this.classificationDataGrid.Size = new System.Drawing.Size(592, 75);
			this.classificationDataGrid.TabIndex = 0;
			this.classificationDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																											   this.classificationStyle});
			this.classificationDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.classificationDataGrid_LinkLabelClicked);
			// 
			// classificationStyle
			// 
			this.classificationStyle.DataGrid = this.classificationDataGrid;
			this.classificationStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																												  this.structureColumn,
																												  this.substructureColumn,
																												  this.celltypeColumn,
																												  this.classificationUpdateColumn,
																												  this.classificationDeleteColumn});
			this.classificationStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.classificationStyle.MappingName = "tblClassification";
			this.classificationStyle.ReadOnly = true;
			this.classificationStyle.RowHeadersVisible = false;
			// 
			// structureColumn
			// 
			this.structureColumn.Format = "";
			this.structureColumn.FormatInfo = null;
			this.structureColumn.HeaderText = "Structure";
			this.structureColumn.MappingName = "fldStructure";
			this.structureColumn.NullText = "Unknown";
			this.structureColumn.ReadOnly = true;
			this.structureColumn.Width = 180;
			// 
			// substructureColumn
			// 
			this.substructureColumn.Format = "";
			this.substructureColumn.FormatInfo = null;
			this.substructureColumn.HeaderText = "Substructure";
			this.substructureColumn.MappingName = "fldSubstructure";
			this.substructureColumn.NullText = "Unknown";
			this.substructureColumn.ReadOnly = true;
			this.substructureColumn.Width = 180;
			// 
			// celltypeColumn
			// 
			this.celltypeColumn.Format = "";
			this.celltypeColumn.FormatInfo = null;
			this.celltypeColumn.HeaderText = "Cell Type";
			this.celltypeColumn.MappingName = "fldCellType";
			this.celltypeColumn.NullText = "Unknown";
			this.celltypeColumn.ReadOnly = true;
			this.celltypeColumn.Width = 110;
			// 
			// classificationUpdateColumn
			// 
			this.classificationUpdateColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.classificationUpdateColumn.Format = "";
			this.classificationUpdateColumn.FormatInfo = null;
			this.classificationUpdateColumn.HeaderText = "Update";
			this.classificationUpdateColumn.MappingName = "fldUpdate";
			this.classificationUpdateColumn.NullText = "Update";
			this.classificationUpdateColumn.ReadOnly = true;
			this.classificationUpdateColumn.Width = 50;
			// 
			// classificationDeleteColumn
			// 
			this.classificationDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.classificationDeleteColumn.Format = "";
			this.classificationDeleteColumn.FormatInfo = null;
			this.classificationDeleteColumn.HeaderText = "Delete";
			this.classificationDeleteColumn.MappingName = "fldDelete";
			this.classificationDeleteColumn.NullText = "Delete";
			this.classificationDeleteColumn.ReadOnly = true;
			this.classificationDeleteColumn.Width = 50;
			// 
			// abstractGroupBox
			// 
			this.abstractGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.abstractGroupBox.Controls.Add(this.description2TextBox);
			this.abstractGroupBox.Controls.Add(this.abstractTextBox);
			this.abstractGroupBox.Controls.Add(this.description2Label);
			this.abstractGroupBox.Controls.Add(this.abstractLabel);
			this.abstractGroupBox.Location = new System.Drawing.Point(8, 8);
			this.abstractGroupBox.Name = "abstractGroupBox";
			this.abstractGroupBox.Size = new System.Drawing.Size(624, 136);
			this.abstractGroupBox.TabIndex = 0;
			this.abstractGroupBox.TabStop = false;
			this.abstractGroupBox.Text = "Abstract/Description";
			// 
			// description2TextBox
			// 
			this.description2TextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.description2TextBox.Location = new System.Drawing.Point(80, 80);
			this.description2TextBox.MaxLength = 65536;
			this.description2TextBox.Multiline = true;
			this.description2TextBox.Name = "description2TextBox";
			this.description2TextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.description2TextBox.Size = new System.Drawing.Size(528, 48);
			this.description2TextBox.TabIndex = 3;
			this.description2TextBox.Text = "";
			this.description2TextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// abstractTextBox
			// 
			this.abstractTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.abstractTextBox.Location = new System.Drawing.Point(80, 24);
			this.abstractTextBox.MaxLength = 65536;
			this.abstractTextBox.Multiline = true;
			this.abstractTextBox.Name = "abstractTextBox";
			this.abstractTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.abstractTextBox.Size = new System.Drawing.Size(528, 48);
			this.abstractTextBox.TabIndex = 2;
			this.abstractTextBox.Text = "";
			this.abstractTextBox.TextChanged += new System.EventHandler(this.Changed_DoCheck);
			// 
			// description2Label
			// 
			this.description2Label.Location = new System.Drawing.Point(16, 80);
			this.description2Label.Name = "description2Label";
			this.description2Label.Size = new System.Drawing.Size(64, 16);
			this.description2Label.TabIndex = 1;
			this.description2Label.Text = "Description:";
			this.description2Label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// abstractLabel
			// 
			this.abstractLabel.Location = new System.Drawing.Point(16, 24);
			this.abstractLabel.Name = "abstractLabel";
			this.abstractLabel.Size = new System.Drawing.Size(56, 16);
			this.abstractLabel.TabIndex = 0;
			this.abstractLabel.Text = "Abstract:";
			this.abstractLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// restrictionsTabPage
			// 
			this.restrictionsTabPage.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.restrictionsTabPage.Controls.Add(this.bugGroupBox);
			this.restrictionsTabPage.Controls.Add(this.restrictionGroupBox);
			this.restrictionsTabPage.Location = new System.Drawing.Point(4, 22);
			this.restrictionsTabPage.Name = "restrictionsTabPage";
			this.restrictionsTabPage.Size = new System.Drawing.Size(640, 310);
			this.restrictionsTabPage.TabIndex = 7;
			this.restrictionsTabPage.Text = "Restrictions/Bugs";
			// 
			// bugGroupBox
			// 
			this.bugGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.bugGroupBox.Controls.Add(this.bugTextBox);
			this.bugGroupBox.Controls.Add(this.bugDataGrid);
			this.bugGroupBox.Controls.Add(this.bugLinkLabel);
			this.bugGroupBox.Location = new System.Drawing.Point(8, 160);
			this.bugGroupBox.Name = "bugGroupBox";
			this.bugGroupBox.Size = new System.Drawing.Size(624, 144);
			this.bugGroupBox.TabIndex = 5;
			this.bugGroupBox.TabStop = false;
			this.bugGroupBox.Text = "Bugs";
			// 
			// bugTextBox
			// 
			this.bugTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.bugTextBox.Location = new System.Drawing.Point(16, 116);
			this.bugTextBox.MaxLength = 255;
			this.bugTextBox.Name = "bugTextBox";
			this.bugTextBox.Size = new System.Drawing.Size(496, 20);
			this.bugTextBox.TabIndex = 2;
			this.bugTextBox.Text = "";
			// 
			// bugDataGrid
			// 
			this.bugDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.bugDataGrid.CaptionVisible = false;
			this.bugDataGrid.ColumnHeadersVisible = false;
			this.bugDataGrid.DataMember = "";
			this.bugDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.bugDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.bugDataGrid.Location = new System.Drawing.Point(16, 24);
			this.bugDataGrid.Name = "bugDataGrid";
			this.bugDataGrid.ParentRowsVisible = false;
			this.bugDataGrid.ReadOnly = true;
			this.bugDataGrid.RowHeadersVisible = false;
			this.bugDataGrid.Size = new System.Drawing.Size(592, 90);
			this.bugDataGrid.TabIndex = 0;
			this.bugDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																									this.bugStyle});
			this.bugDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// bugStyle
			// 
			this.bugStyle.ColumnHeadersVisible = false;
			this.bugStyle.DataGrid = this.bugDataGrid;
			this.bugStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																									   this.bugColumn,
																									   this.bugDeleteColumn});
			this.bugStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.bugStyle.MappingName = "tblBug";
			this.bugStyle.ReadOnly = true;
			this.bugStyle.RowHeadersVisible = false;
			// 
			// bugColumn
			// 
			this.bugColumn.Format = "";
			this.bugColumn.FormatInfo = null;
			this.bugColumn.HeaderText = "Bug";
			this.bugColumn.MappingName = "fldItem";
			this.bugColumn.ReadOnly = true;
			this.bugColumn.Width = 512;
			// 
			// bugDeleteColumn
			// 
			this.bugDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.bugDeleteColumn.Format = "";
			this.bugDeleteColumn.FormatInfo = null;
			this.bugDeleteColumn.HeaderText = "Delete";
			this.bugDeleteColumn.MappingName = "fldDelete";
			this.bugDeleteColumn.NullText = "Delete";
			this.bugDeleteColumn.ReadOnly = true;
			this.bugDeleteColumn.Width = 70;
			// 
			// bugLinkLabel
			// 
			this.bugLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.bugLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.bugLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.bugLinkLabel.Location = new System.Drawing.Point(528, 116);
			this.bugLinkLabel.Name = "bugLinkLabel";
			this.bugLinkLabel.Size = new System.Drawing.Size(72, 16);
			this.bugLinkLabel.TabIndex = 4;
			this.bugLinkLabel.TabStop = true;
			this.bugLinkLabel.Text = "Add Bug";
			this.bugLinkLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
			this.bugLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.bugLinkLabel_LinkClicked);
			// 
			// restrictionGroupBox
			// 
			this.restrictionGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.restrictionGroupBox.Controls.Add(this.restrictionTextBox);
			this.restrictionGroupBox.Controls.Add(this.restrictionLinkLabel);
			this.restrictionGroupBox.Controls.Add(this.restrictionDataGrid);
			this.restrictionGroupBox.Location = new System.Drawing.Point(8, 8);
			this.restrictionGroupBox.Name = "restrictionGroupBox";
			this.restrictionGroupBox.Size = new System.Drawing.Size(624, 144);
			this.restrictionGroupBox.TabIndex = 4;
			this.restrictionGroupBox.TabStop = false;
			this.restrictionGroupBox.Text = "Restrictions";
			// 
			// restrictionTextBox
			// 
			this.restrictionTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.restrictionTextBox.Location = new System.Drawing.Point(16, 116);
			this.restrictionTextBox.MaxLength = 255;
			this.restrictionTextBox.Name = "restrictionTextBox";
			this.restrictionTextBox.Size = new System.Drawing.Size(496, 20);
			this.restrictionTextBox.TabIndex = 2;
			this.restrictionTextBox.Text = "";
			// 
			// restrictionLinkLabel
			// 
			this.restrictionLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.restrictionLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.restrictionLinkLabel.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.restrictionLinkLabel.Location = new System.Drawing.Point(504, 116);
			this.restrictionLinkLabel.Name = "restrictionLinkLabel";
			this.restrictionLinkLabel.Size = new System.Drawing.Size(100, 16);
			this.restrictionLinkLabel.TabIndex = 3;
			this.restrictionLinkLabel.TabStop = true;
			this.restrictionLinkLabel.Text = "Add Restriction";
			this.restrictionLinkLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
			this.restrictionLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.restrictionLinkLabel_LinkClicked);
			// 
			// restrictionDataGrid
			// 
			this.restrictionDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.restrictionDataGrid.CaptionVisible = false;
			this.restrictionDataGrid.ColumnHeadersVisible = false;
			this.restrictionDataGrid.DataMember = "";
			this.restrictionDataGrid.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.restrictionDataGrid.LinkColor = System.Drawing.SystemColors.ActiveCaption;
			this.restrictionDataGrid.Location = new System.Drawing.Point(16, 24);
			this.restrictionDataGrid.Name = "restrictionDataGrid";
			this.restrictionDataGrid.ParentRowsVisible = false;
			this.restrictionDataGrid.ReadOnly = true;
			this.restrictionDataGrid.RowHeadersVisible = false;
			this.restrictionDataGrid.Size = new System.Drawing.Size(592, 90);
			this.restrictionDataGrid.TabIndex = 0;
			this.restrictionDataGrid.TableStyles.AddRange(new System.Windows.Forms.DataGridTableStyle[] {
																											this.restrictionStyle});
			this.restrictionDataGrid.LinkLabelClicked += new ModuleUpdater.InheritedControls.ColorUpdateViewDeleteDataGrid.LinkLabelClickedHandler(this.DataGridDelete_LinkLabelClicked);
			// 
			// restrictionStyle
			// 
			this.restrictionStyle.ColumnHeadersVisible = false;
			this.restrictionStyle.DataGrid = this.restrictionDataGrid;
			this.restrictionStyle.GridColumnStyles.AddRange(new System.Windows.Forms.DataGridColumnStyle[] {
																											   this.restrictionColumn,
																											   this.restrictionDeleteColumn});
			this.restrictionStyle.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.restrictionStyle.MappingName = "tblRestriction";
			this.restrictionStyle.ReadOnly = true;
			this.restrictionStyle.RowHeadersVisible = false;
			// 
			// restrictionColumn
			// 
			this.restrictionColumn.Format = "";
			this.restrictionColumn.FormatInfo = null;
			this.restrictionColumn.HeaderText = "Restriction";
			this.restrictionColumn.MappingName = "fldItem";
			this.restrictionColumn.ReadOnly = true;
			this.restrictionColumn.Width = 512;
			// 
			// restrictionDeleteColumn
			// 
			this.restrictionDeleteColumn.Alignment = System.Windows.Forms.HorizontalAlignment.Center;
			this.restrictionDeleteColumn.Format = "";
			this.restrictionDeleteColumn.FormatInfo = null;
			this.restrictionDeleteColumn.HeaderText = "Delete";
			this.restrictionDeleteColumn.MappingName = "fldDelete";
			this.restrictionDeleteColumn.NullText = "Delete";
			this.restrictionDeleteColumn.ReadOnly = true;
			this.restrictionDeleteColumn.Width = 70;
			// 
			// okButton
			// 
			this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okButton.Location = new System.Drawing.Point(568, 344);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(80, 23);
			this.okButton.TabIndex = 1;
			this.okButton.Text = "Update";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// backLinkLabel
			// 
			this.backLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.backLinkLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.backLinkLabel.LinkBehavior = System.Windows.Forms.LinkBehavior.HoverUnderline;
			this.backLinkLabel.LinkColor = System.Drawing.SystemColors.Highlight;
			this.backLinkLabel.Location = new System.Drawing.Point(0, 344);
			this.backLinkLabel.Name = "backLinkLabel";
			this.backLinkLabel.Size = new System.Drawing.Size(64, 16);
			this.backLinkLabel.TabIndex = 4;
			this.backLinkLabel.TabStop = true;
			this.backLinkLabel.Text = "< back";
			this.backLinkLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.backLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.backLinkLabel_LinkClicked);
			// 
			// openFileDialog
			// 
			this.openFileDialog.Filter = "Dynamic Link Library|*.dll";
			this.openFileDialog.Title = "Select File";
			// 
			// VersionControl
			// 
			this.Controls.Add(this.backLinkLabel);
			this.Controls.Add(this.okButton);
			this.Controls.Add(this.versionTabControl);
			this.Name = "VersionControl";
			this.Size = new System.Drawing.Size(648, 368);
			this.versionTabControl.ResumeLayout(false);
			this.generalTabPage.ResumeLayout(false);
			this.versionGroupBox.ResumeLayout(false);
			this.moduleGroupBox.ResumeLayout(false);
			this.parametersTabPage.ResumeLayout(false);
			this.datatypeGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.datatypeDataGrid)).EndInit();
			this.parametersGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.parameterDataGrid)).EndInit();
			this.programmingTabPage.ResumeLayout(false);
			this.prerequisiteGroupBox.ResumeLayout(false);
			this.programmingGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.complianceDataGrid)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.languageDataGrid)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.osDataGrid)).EndInit();
			this.designTabPage.ResumeLayout(false);
			this.notesTabPage.ResumeLayout(false);
			this.notesGroupBox.ResumeLayout(false);
			this.evaluationGroupBox.ResumeLayout(false);
			this.filesTabPage.ResumeLayout(false);
			this.historyTabPage.ResumeLayout(false);
			this.historyGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.historyDataGrid)).EndInit();
			this.referencesGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.referenceDataGrid)).EndInit();
			this.descriptionTabPage.ResumeLayout(false);
			this.classificationGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.classificationDataGrid)).EndInit();
			this.abstractGroupBox.ResumeLayout(false);
			this.restrictionsTabPage.ResumeLayout(false);
			this.bugGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.bugDataGrid)).EndInit();
			this.restrictionGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.restrictionDataGrid)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		private void DoCheck()
		{
			this.okButton.Enabled = false;

			if (this.addMode && this.setupDone)
			{
				if (this.moduleID == 0)
				{
					if (this.nameTextBox.Text != "" && this.descriptionTextBox.Text != ""
						&& (this.version1TextBox.Text != "0" || this.version2TextBox.Text != "0"
						|| this.version3TextBox.Text != "0" || this.version4TextBox.Text != "0")
						&& this.contactComboBox.SelectedIndex != 0)
						this.okButton.Enabled = true;
				}
				else 
				{
					string[] versionArray = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"].ToString().Split('.');
					
					if ((this.version1TextBox.Text != versionArray[0] || this.version2TextBox.Text != versionArray[1]
						|| this.version3TextBox.Text != versionArray[2] || this.version4TextBox.Text != versionArray[3])
						//&& this.releaseDateTimePicker.Value != DateTime.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"].ToString()).Date
						&& this.contactComboBox.SelectedIndex != 0)
						this.okButton.Enabled = true;
				}
			}
			else if (this.setupDone)
			{
				string[] versionArray = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"].ToString().Split('.');

				if (this.version1TextBox.Text != versionArray[0] || this.version2TextBox.Text != versionArray[1]
					|| this.version3TextBox.Text != versionArray[2] || this.version4TextBox.Text != versionArray[3]
					|| this.releaseDateTimePicker.Value != DateTime.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"].ToString()).Date
					|| this.contactComboBox.SelectedValue.ToString() != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldContactID"].ToString()
					
					|| this.abstractTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldAbstract"].ToString()
					|| this.description2TextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDescription"].ToString()
					|| this.neuralTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNeuralEntity"].ToString()
					
					|| this.prerequisiteTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldPrerequisite"].ToString()

					|| this.evaluationTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldEvaluation"].ToString()
					|| this.notesTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNote"].ToString()

					|| this.designTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDesign"].ToString()
					|| this.interfaceTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldInterface"].ToString()
					|| this.implementationTextBox.Text != this.versionDataSet.Tables["tblVersion"].Rows[0]["fldImplementation"].ToString()
					
					|| this.versionDataSet.GetChanges() != null
					)
					this.okButton.Enabled = true;	
			}
		}
		
		private void SetupControl()
		{
			ModuleService.EarDevModuleService moduleService = new ModuleService.EarDevModuleService();

			if (this.addMode)
			{
				this.okButton.Text = "Add";
				this.versionTabControl.TabPages.Clear();
				this.versionTabControl.TabPages.Add(this.generalTabPage);

				if (this.moduleID != 0)
				{
					this.currentStatus = VersionControlStatus.AddVersion;
					this.versionDataSet = moduleService.GetVersion(this.moduleID);
					//this.versionDataSet = moduleService.GetPreviousVersion(this.moduleID);
				}
				else
				{
					this.currentStatus = VersionControlStatus.AddModule;
					this.versionDataSet = moduleService.GetContactList();
					this.nameTextBox.ReadOnly = false;
					this.descriptionTextBox.ReadOnly = false;
				}
			}
			else
			{
				this.currentStatus = VersionControlStatus.UpdateVersion;
				this.okButton.Text = "Update";

				//if (this.versionTabControl.TabPages.Count < 8)
				//{
					this.versionTabControl.TabPages.Clear();
					this.versionTabControl.TabPages.AddRange(new TabPage[] { this.generalTabPage, this.descriptionTabPage, this.parametersTabPage,
																			   this.designTabPage, this.programmingTabPage, this.restrictionsTabPage,
																			   this.notesTabPage, this.historyTabPage, this.filesTabPage});
				//}

				this.versionDataSet = moduleService.GetVersion(this.versionID);
				this.moduleID = long.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleID"].ToString());
			}

			this.contactComboBox.DataSource = this.versionDataSet.Tables["tblContact"];
			this.contactComboBox.DisplayMember = "fldContactName";
			this.contactComboBox.ValueMember = "fldContactID";
			
			if (this.moduleID != 0)
			{
				// General Tab --------------------------------------------------------------------------------------------------------------------
				this.nameTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleName"].ToString();
				this.descriptionTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleDescription"].ToString();
				this.moduleLSIDTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleLSID"].ToString();

				string[] versionArray = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"].ToString().Split('.');
				this.version1TextBox.Text = versionArray[0];
				this.version2TextBox.Text = versionArray[1];
				this.version3TextBox.Text = versionArray[2];
				this.version4TextBox.Text = versionArray[3];

				this.releaseDateTimePicker.Value = DateTime.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"].ToString());
				this.contactComboBox.SelectedValue = long.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldContactID"].ToString());
			}

			if (!this.addMode)
			{
				this.versionLSIDTtextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionLSID"].ToString();

				// Description Tab ------------------------------------------------------------------------------------------------------------------
				this.abstractTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldAbstract"].ToString();
				this.description2TextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDescription"].ToString();
				this.neuralTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNeuralEntity"].ToString();
				this.versionDataSet.Tables["tblClassification"].Columns.Add("fldUpdate", typeof(string));
				this.versionDataSet.Tables["tblClassification"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblClassification"].AcceptChanges();
				this.classificationDataGrid.DataSource = this.versionDataSet.Tables["tblClassification"].DefaultView;

				// Parameter/Data Type Tab ----------------------------------------------------------------------------------------------------------
				this.versionDataSet.Tables["tblParameter"].Columns.Add("fldUpdate", typeof(string));
				this.versionDataSet.Tables["tblParameter"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblParameter"].AcceptChanges();
				this.parameterDataGrid.DataSource = this.versionDataSet.Tables["tblParameter"];
				this.versionDataSet.Tables["tblVersionDataType"].Columns.Add("fldUpdate", typeof(string));
				this.versionDataSet.Tables["tblVersionDataType"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblVersionDataType"].AcceptChanges();
				this.datatypeDataGrid.DataSource = this.versionDataSet.Tables["tblVersionDataType"];

				// Programming Tab -------------------------------------------------------------------------------------------------------------------
				this.versionDataSet.Tables["tblVersionOS"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblVersionOS"].AcceptChanges();
				this.osDataGrid.DataSource = this.versionDataSet.Tables["tblVersionOS"];
				this.osComboBox.ValueMember = "fldOSID";
				this.osComboBox.DisplayMember = "fldItem";
				this.osComboBox.DataSource = this.versionDataSet.Tables["tblOS"];
				this.versionDataSet.Tables["tblVersionLanguage"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblVersionLanguage"].AcceptChanges();
				this.languageDataGrid.DataSource = this.versionDataSet.Tables["tblVersionLanguage"];
				this.languageComboBox.ValueMember = "fldLanguageID";
				this.languageComboBox.DisplayMember = "fldItem";
				this.languageComboBox.DataSource = this.versionDataSet.Tables["tblLanguage"];
				this.versionDataSet.Tables["tblVersionCompliance"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblVersionCompliance"].AcceptChanges();
				this.complianceDataGrid.DataSource = this.versionDataSet.Tables["tblVersionCompliance"];
				this.complianceComboBox.ValueMember = "fldComplianceID";
				this.complianceComboBox.DisplayMember = "fldItem";
				this.complianceComboBox.DataSource = this.versionDataSet.Tables["tblCompliance"];
				this.prerequisiteTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldPrerequisite"].ToString();

				// Evaluation/Notes Tab --------------------------------------------------------------------------------------------------------------
				this.evaluationTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldEvaluation"].ToString();
				this.notesTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNote"].ToString();

				// Design Info. Tab ------------------------------------------------------------------------------------------------------------------
				this.designTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDesign"].ToString();
				this.interfaceTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldInterface"].ToString();
				this.implementationTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldImplementation"].ToString();

				// Restrictions/Bugs Tab -------------------------------------------------------------------------------------------------------------
				this.versionDataSet.Tables["tblRestriction"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblRestriction"].AcceptChanges();
				this.restrictionDataGrid.DataSource = this.versionDataSet.Tables["tblRestriction"].DefaultView;
				this.versionDataSet.Tables["tblBug"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblBug"].AcceptChanges();
				this.bugDataGrid.DataSource = this.versionDataSet.Tables["tblBug"].DefaultView;

				// References/History Tab ------------------------------------------------------------------------------------------------------------
				this.versionDataSet.Tables["tblReference"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblReference"].AcceptChanges();
				this.referenceDataGrid.DataSource = this.versionDataSet.Tables["tblReference"].DefaultView;
				this.versionDataSet.Tables["tblHistory"].Columns.Add("fldDelete", typeof(string));
				this.versionDataSet.Tables["tblHistory"].AcceptChanges();
				this.historyDataGrid.DataSource = this.versionDataSet.Tables["tblHistory"].DefaultView;

				// Files Tab ------------------------------------------------------------------------------------------------------------------
				this.dllTextBox.Text = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldExecutableFile"].ToString();
			}

			this.setupDone = true;
			this.DoCheck();
		}	

		#region Shared Functions/Events
		public event NavigateClickedHandler NavigateClicked;
		private void backLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.NavigateClicked != null)
				this.NavigateClicked(this, "Back", this.moduleID);
		}

		private void DataGridDelete_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (MessageBox.Show(this.ParentForm, "Are you sure you want do delete this item?", "Confirm Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
			{
				row.Delete();
				this.DoCheck();
			}
		}

		private void Changed_DoCheck(object sender, System.EventArgs e)
		{
			this.DoCheck();
		}
		#endregion

		#region Description Tab
		private void classificationDataGrid_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (columnName == "fldUpdate")
			{
				ModuleUpdater.Dialogs.ClassificationDialog classificationDialog = new ModuleUpdater.Dialogs.ClassificationDialog(false, ref row, ref this.versionDataSet);
				classificationDialog.ShowDialog(this.ParentForm);
				this.DoCheck();
			}
			else
				this.DataGridDelete_LinkLabelClicked(columnName, row);
		}

		private void classificationLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			DataRow classificationRow = this.versionDataSet.Tables["tblClassification"].NewRow();
			ModuleUpdater.Dialogs.ClassificationDialog classificationDialog = new ModuleUpdater.Dialogs.ClassificationDialog(true, ref classificationRow, ref this.versionDataSet);
			if (classificationDialog.ShowDialog(this.ParentForm) == DialogResult.OK)
			{
				this.versionDataSet.Tables["tblClassification"].Rows.Add(classificationRow);
				this.DoCheck();
			}
		}
		#endregion

		#region Parameter/Data Type Tab
		private void parameterDataGrid_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (columnName == "fldUpdate")
			{
				ModuleUpdater.Dialogs.ParameterDialog parameterDialog = new ModuleUpdater.Dialogs.ParameterDialog(false, ref row, this.versionDataSet.Tables["tblParameterType"]);
				parameterDialog.ShowDialog(this.ParentForm);
				this.DoCheck();
			}
			else
				this.DataGridDelete_LinkLabelClicked(columnName, row);
		}

		private void parameterLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			DataRow parameterRow = this.versionDataSet.Tables["tblParameter"].NewRow();
			ModuleUpdater.Dialogs.ParameterDialog parameterDialog = new ModuleUpdater.Dialogs.ParameterDialog(true, ref parameterRow, this.versionDataSet.Tables["tblParameterType"]);
			if (parameterDialog.ShowDialog(this.ParentForm) == DialogResult.OK)
			{
				this.versionDataSet.Tables["tblParameter"].Rows.Add(parameterRow);
				this.DoCheck();
			}
		}

		private void datatypeDataGrid_LinkLabelClicked(string columnName, System.Data.DataRow row)
		{
			if (columnName == "fldUpdate")
			{
				ModuleUpdater.Dialogs.DataTypeDialog datatypeDialog = new ModuleUpdater.Dialogs.DataTypeDialog(false, ref row, ref this.versionDataSet);
				datatypeDialog.ShowDialog(this.ParentForm);
				this.DoCheck();
			}
			else
				this.DataGridDelete_LinkLabelClicked(columnName, row);
		}

		private void datatypeLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			DataRow datatypeRow = this.versionDataSet.Tables["tblVersionDataType"].NewRow();
			ModuleUpdater.Dialogs.DataTypeDialog datatypeDialog = new ModuleUpdater.Dialogs.DataTypeDialog(true, ref datatypeRow, ref this.versionDataSet);
			if (datatypeDialog.ShowDialog(this.ParentForm) == DialogResult.OK)
			{
				this.versionDataSet.Tables["tblVersionDataType"].Rows.Add(datatypeRow);
				this.DoCheck();
			}
		}
		#endregion

		#region Programming Tab
		private void osLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.versionDataSet.Tables["tblVersionOS"].Select("fldOSID = " + this.osComboBox.SelectedValue.ToString()).Length > 0)
				MessageBox.Show(this.ParentForm, "This operating system has already been added.", "Duplicated Item", MessageBoxButtons.OK, MessageBoxIcon.Hand);
			else
			{
				DataRow osRow = this.versionDataSet.Tables["tblVersionOS"].NewRow();
				osRow["fldVersionID"] = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionID"];
				osRow["fldOSID"] = this.osComboBox.SelectedValue;
				osRow["fldOS"] = ((DataRowView)this.osComboBox.SelectedItem).Row["fldItem"].ToString();
				this.versionDataSet.Tables["tblVersionOS"].Rows.Add(osRow);
				this.DoCheck();
			}
		}

		private void languageLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.versionDataSet.Tables["tblVersionLanguage"].Select("fldLanguageID = " + this.languageComboBox.SelectedValue.ToString()).Length > 0)
				MessageBox.Show(this.ParentForm, "This language has already been added.", "Duplicated Item", MessageBoxButtons.OK, MessageBoxIcon.Hand);
			else
			{
				DataRow languageRow = this.versionDataSet.Tables["tblVersionLanguage"].NewRow();
				languageRow["fldVersionID"] = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionID"];
				languageRow["fldLanguageID"] = this.languageComboBox.SelectedValue;
				languageRow["fldLanguage"] = ((DataRowView)this.languageComboBox.SelectedItem).Row["fldItem"].ToString();
				this.versionDataSet.Tables["tblVersionLanguage"].Rows.Add(languageRow);
				this.DoCheck();
			}
		}

		private void complianceLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (this.versionDataSet.Tables["tblVersionCompliance"].Select("fldComplianceID = " + this.complianceComboBox.SelectedValue.ToString()).Length > 0)
				MessageBox.Show(this.ParentForm, "This compliance has already been added.", "Duplicated Item", MessageBoxButtons.OK, MessageBoxIcon.Hand);
			else
			{
				DataRow complianceRow = this.versionDataSet.Tables["tblVersionCompliance"].NewRow();
				complianceRow["fldVersionID"] = this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionID"];
				complianceRow["fldComplianceID"] = this.complianceComboBox.SelectedValue;
				complianceRow["fldCompliance"] = ((DataRowView)this.complianceComboBox.SelectedItem).Row["fldItem"].ToString();
				this.versionDataSet.Tables["tblVersionCompliance"].Rows.Add(complianceRow);
				this.DoCheck();
			}
		}
		#endregion

		#region Restrictions/Bugs Tab
		private void restrictionLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (restrictionTextBox.Text != "")
			{
				DataRow restrictionRow = this.versionDataSet.Tables["tblRestriction"].NewRow();
				restrictionRow["fldItem"] = this.restrictionTextBox.Text;
				this.versionDataSet.Tables["tblRestriction"].Rows.Add(restrictionRow);
				this.DoCheck();
				this.restrictionTextBox.Text = "";
				this.restrictionTextBox.Focus();
			}
		}

		private void bugLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (bugTextBox.Text != "")
			{
				DataRow bugRow = this.versionDataSet.Tables["tblBug"].NewRow();
				bugRow["fldItem"] = this.bugTextBox.Text;
				this.versionDataSet.Tables["tblBug"].Rows.Add(bugRow);
				this.DoCheck();
				this.bugTextBox.Text = "";
				this.bugTextBox.Focus();
			}
		}
		#endregion

		#region References/History Tab
		private void referenceLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (referenceTextBox.Text != "")
			{
				DataRow referenceRow = this.versionDataSet.Tables["tblReference"].NewRow();
				referenceRow["fldItem"] = this.referenceTextBox.Text;
				this.versionDataSet.Tables["tblReference"].Rows.Add(referenceRow);
				this.DoCheck();
				this.referenceTextBox.Text = "";
				this.referenceTextBox.Focus();
			}
		}

		private void historyLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			if (historyTextBox.Text != "")
			{
				DataRow historyRow = this.versionDataSet.Tables["tblHistory"].NewRow();
				historyRow["fldItem"] = this.historyTextBox.Text;
				this.versionDataSet.Tables["tblHistory"].Rows.Add(historyRow);
				this.DoCheck();
				this.historyTextBox.Text = "";
				this.historyTextBox.Focus();
			}
		}
		#endregion

		public VersionControlStatus Status
		{
			get { return this.currentStatus; }
		}
		
		private void okButton_Click(object sender, System.EventArgs e)
		{
			ModuleService.EarDevModuleService moduleService = new ModuleService.EarDevModuleService();
			ModuleService.AuthHeader authHeader = new ModuleService.AuthHeader();
			authHeader.userName = authHeader.passWord = "EarLab";
			moduleService.AuthHeaderValue = authHeader;

			if (addMode)
			{
				if (this.moduleID != 0)
				{
					if (MessageBox.Show(this.ParentForm, "Are you sure you want add a new version?", "Confirm New Version", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
					{
						string versionNumber = this.version1TextBox.Text + "." + this.version2TextBox.Text + "." + this.version3TextBox.Text + "." + this.version4TextBox.Text;
						this.versionID = moduleService.AddVersion(long.Parse(this.versionDataSet.Tables["tblVersion"].Rows[0]["fldModuleID"].ToString()), versionNumber, this.releaseDateTimePicker.Value, long.Parse(this.contactComboBox.SelectedValue.ToString()));

						this.addMode = false;
						this.SetupControl();
					}
				}
				else
				{
					if (MessageBox.Show(this.ParentForm, "Are you sure you want add a new Module and version?", "Confirm New Module and Version", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
					{
						string versionNumber = this.version1TextBox.Text + "." + this.version2TextBox.Text + "." + this.version3TextBox.Text + "." + this.version4TextBox.Text;
						this.versionID = moduleService.AddModuleVersion(this.nameTextBox.Text, this.descriptionTextBox.Text, versionNumber, this.releaseDateTimePicker.Value, long.Parse(this.contactComboBox.SelectedValue.ToString()));
						this.nameTextBox.ReadOnly = this.descriptionTextBox.ReadOnly = true;

						this.addMode = false;
						this.SetupControl();
					}
				}
			}
			else
			{
				this.Cursor = Cursors.WaitCursor;
				if (MessageBox.Show(this.ParentForm, "Are you sure you want update version?", "Confirm Update", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
				{
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldReleaseDate"] = this.releaseDateTimePicker.Value.ToShortDateString();
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldVersionNumber"] = this.version1TextBox.Text + "." + this.version2TextBox.Text + "." + this.version3TextBox.Text + "." + this.version4TextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldContactID"] = long.Parse(this.contactComboBox.SelectedValue.ToString());

					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldAbstract"] = this.abstractTextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDescription"] = this.description2TextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNeuralEntity"] = this.neuralTextBox.Text;

					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldDesign"] = this.designTextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldImplementation"] = this.implementationTextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldInterface"] = this.interfaceTextBox.Text;

					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldPrerequisite"] = this.prerequisiteTextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldEvaluation"] = this.evaluationTextBox.Text;
					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldNote"] = this.notesTextBox.Text;

					this.versionDataSet.Tables["tblVersion"].Rows[0]["fldExecutableFile"] = this.dllTextBox.Text;

					moduleService.UpdateVersion(ref this.versionDataSet);
					this.SetupControl();
				}
				this.Cursor = Cursors.Default;
			}
		}

		private void dllButton_Click(object sender, System.EventArgs e)
		{
			if (this.openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.dllTextBox.Text = this.openFileDialog.FileName;
			}
		}
	}
}
