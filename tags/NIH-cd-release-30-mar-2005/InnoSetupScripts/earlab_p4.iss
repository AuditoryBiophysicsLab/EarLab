#include "isxdl.iss"

[Setup]
VersionInfoVersion=2.0.0
VersionInfoCompany=EarLab
Compression=zip
AppCopyright=Boston University
AppName=Desktop EarLab
AppVerName=Desktop EarLab 2.0.0
ChangesEnvironment=true
DefaultDirName={pf}\EarLab
ShowLanguageDialog=no
DefaultGroupName=EarLab\Desktop EarLab
SetupIconFile=..\InstallDepends\earlab.ico
AppID={{11CF1BB3-563E-4AA9-B2C2-515BBF1A7345}
UninstallDisplayIcon={app}\Desktop EarLab\earlab.ico
InternalCompressLevel=ultra
OutputBaseFilename=DesktopEarLabSetup
SolidCompression=true
ChangesAssociations=true
MergeDuplicateFiles=false
OutputDir=../Setups/P4
RestartIfNeededByRun=true
AppPublisherURL=http://earlab.bu.edu
UninstallFilesDir={app}\Desktop EarLab
WizardImageFile=..\InstallDepends\earlab_large.bmp
WizardSmallImageFile=..\InstallDepends\earlab_small.bmp
LicenseFile=..\InstallDepends\license.txt

[Components]
Name: desktopEarlab; Description: Desktop EarLab; Types: full custom compact; Flags: fixed
#ifexist "..\DataViewer\DataViewerInstaller.msi"
Name: dataViewer; Description: EarLab DataViewer; Types: full
#endif
Name: dotNetFramework; Description: .Net Framework 1.1; Types: full; Check: dotNetDownload


[Files]
Source: ..\..\out\Modules\Release-P4\*.*; DestDir: {app}\Modules; Flags: replacesameversion ignoreversion
Source: ..\..\out\Desktop EarLab\Release-P4\*.*; DestDir: {app}\Desktop EarLab; Flags: replacesameversion ignoreversion
Source: ..\Documentation\*.*; DestDir: {app}\Desktop EarLab; Excludes: .svn; Flags: skipifsourcedoesntexist recursesubdirs ignoreversion createallsubdirs replacesameversion
Source: ..\ModelFolders\*.*; DestDir: {userdocs}\EarLab; Excludes: .svn; Flags: skipifsourcedoesntexist recursesubdirs createallsubdirs uninsneveruninstall replacesameversion ignoreversion
Source: ..\InstallDepends\dotnetfx.exe; DestDir: {tmp}; Flags: skipifsourcedoesntexist replacesameversion ignoreversion; Components: dotNetFramework
Source: ..\InstallDepends\earlabsetuphelper.exe; DestDir: {app}\Desktop EarLab; Flags: skipifsourcedoesntexist replacesameversion ignoreversion
Source: ..\InstallDepends\earlab.ico; DestDir: {app}\Desktop EarLab; Flags: replacesameversion ignoreversion
Source: ..\DataViewer\DataViewerInstaller.msi; DestDir: {tmp}; Flags: skipifsourcedoesntexist replacesameversion ignoreversion

[Tasks]
Name: DesktopIcon; Description: Install Desktop Icon; GroupDescription: Icons

[Registry]
Root: HKCU; Subkey: Software\EarLab; ValueType: string; ValueName: ModelsDirectory; ValueData: {userdocs}\EarLab\Models
Root: HKCU; Subkey: Software\EarLab; ValueType: string; ValueName: InputDirectory; ValueData: {userdocs}\EarLab\Signals
Root: HKLM; Subkey: Software\EarLab; ValueType: string; ValueName: ModulesPath; ValueData: {app}\Modules
Root: HKCU; Subkey: Software\EarLab; ValueType: dword; ValueName: FrameCount; ValueData: 100
Root: HKCU; Subkey: Software\EarLab; ValueType: string; ValueName: ModuleParameterFile; ValueData: {userdocs}\EarLab\Models\AN Models\MonauralNonlinear.parameters
Root: HKCU; Subkey: Software\EarLab; ValueType: string; ValueName: RunParameterFile; ValueData: {userdocs}\EarLab\Models\AN Models\MonauralNonlinear.diagram
Root: HKCU; Subkey: Software\EarLab; ValueType: string; ValueName: OutputDirectory; ValueData: {userdocs}\EarLab\Output\AN Output\AN Nonlinear Output

[Icons]
Name: {group}\EarLab Simulator; Filename: {app}\Desktop EarLab\simulator.exe; WorkingDir: {app}\Desktop EarLab; IconFilename: {app}\Desktop Earlab\earlab.ico; IconIndex: 0
Name: {group}\EarLab Experiment Manager; Filename: {app}\Desktop EarLab\experimentmanager.exe; WorkingDir: {app}\Desktop EarLab; IconFilename: {app}\Desktop Earlab\earlab.ico; IconIndex: 0
#ifexist "..\Documentation\readme.html"
Name: {group}\EarLab Readme; Filename: {app}\Desktop EarLab\readme.html; WorkingDir: {app}\Desktop EarLab
#endif
Name: {commondesktop}\EarLab Experiment Manager; Filename: {app}\Desktop EarLab\experimentmanager.exe; WorkingDir: {app}\Desktop EarLab; IconFilename: {app}\Desktop Earlab\earlab.ico; IconIndex: 0; Tasks: DesktopIcon


[_ISTool]
EnableISX=true

[Run]
Filename: {ini:{tmp}\dep.ini,install,dotnet11Redist}; Parameters: "/Q /T:{tmp}\dotnetfx /C:""install /q"""; Description: .NET 1.1 Install; Flags: skipifdoesntexist; StatusMsg: Installing Microsoft .NET 1.1... (This may take a few minutes); Components: dotNetFramework
#ifexist "..\DataViewer\DataViewerInstaller.msi"
Filename: msiexec.exe; Parameters: "-i ""{tmp}\DataViewerInstaller.msi"" -qn"; WorkingDir: {tmp}; StatusMsg: Installing EarLab DataViewer...; Flags: skipifdoesntexist; Components: dataViewer
#endif
Filename: {app}\Desktop EarLab\earlabsetuphelper.exe; Parameters: "-i """"""{app}\Desktop EarLab"""""""; WorkingDir: {tmp}; StatusMsg: Updating Path Variable...; Flags: skipifdoesntexist runhidden; Components: desktopEarlab
Filename: {tmp}\earlabdelaytemp.txt; WorkingDir: {tmp}; StatusMsg: Finalizing...; Flags: skipifdoesntexist; Components: desktopEarlab
Filename: {app}\Desktop EarLab\experimentmanager.exe; WorkingDir: {app}\Desktop EarLab; Description: Launch EarLab Experiment Manager; Flags: postinstall nowait; Components: desktopEarlab
#ifexist "..\Documentation\readme.html"
Filename: {app}\Desktop EarLab\readme.html; WorkingDir: {app}\Desktop EarLab; Description: Show Installation Notes; Flags: skipifdoesntexist postinstall shellexec nowait; Components: desktopEarlab
#endif

[UninstallRun]
#ifexist "..\DataViewer\DataViewerInstaller.msi"
Filename: {app}\Desktop EarLab\earlabsetuphelper.exe; Parameters: -removedataviewer {{4DAC4024-ACBA-4B07-9896-D96C553E5164}; WorkingDir: {app}\Desktop EarLab; Flags: skipifdoesntexist runminimized
#endif
Filename: {app}\Desktop EarLab\earlabsetuphelper.exe; Parameters: "-u ""{app}\Desktop EarLab"" -deletefolders ""{userdocs}\EarLab"""; WorkingDir: {app}\Desktop EarLab; Flags: skipifdoesntexist runminimized

[Code]
var
  dotnet11RedistPath: string;
  downloadNeeded: boolean;

  memoDependenciesNeeded: string;

const
  dotnet11RedistURL = 'http://download.microsoft.com/download/a/a/c/aac39226-8825-44ce-90e3-bf8203e74006/dotnetfx.exe';

function IsDotNET11Detected(): boolean;
// Indicates whether .NET Framework 1.1 is installed.
var
    success: boolean;
    install: cardinal;
begin
    success := RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\NET Framework Setup\NDP\v1.1.4322', 'Install', install);
    Result := success and (install = 1);
end;

function dotNetDownload(): Boolean;
begin
  Result := false;

  // Check for required netfx 1.1 installation
  if not IsDotNET11Detected() then begin
    memoDependenciesNeeded := memoDependenciesNeeded + '      .NET Framework 1.1' #13;

    dotnet11RedistPath := ExpandConstant('{tmp}\dotnetfx.exe');
    if not FileExists(dotnet11RedistPath) then begin
      isxdl_AddFile(dotnet11RedistURL, dotnet11RedistPath);
      downloadNeeded := true;
    end;

    SetIniString('install', 'dotnet11Redist', dotnet11RedistPath, ExpandConstant('{tmp}\dep.ini'));
    Result := true;
  end;
end;

function NextButtonClick(CurPage: Integer): Boolean;
var
  hWnd: Integer;
begin
  Result := true;

  if CurPage = (wpSelectTasks+1) then begin

    hWnd := StrToInt(ExpandConstant('{wizardhwnd}'));

    // don't try to init isxdl if it's not needed because it will error on < ie 3
    if downloadNeeded then
      if isxdl_DownloadFiles(hWnd) = 0 then Result := false;
  end;
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
var
  s: string;
begin
  if memoDependenciesNeeded <> '' then s := s + 'Dependencies to install:' + NewLine + memoDependenciesNeeded + NewLine;
  s := s + MemoDirInfo + NewLine + NewLine;
  Result := s
end;
