call "C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\vsvars32.bat"
msbuild "SolutionFiles\Complete DesktopEarlab\Complete DesktopEarlab.sln" /t:clean,SupportClasses;build /p:Configuration=Release-Athlon
cd "Installer"
call ExportSVN
"c:\Program Files\Macrovision\IS12\System\ISBuild.exe" -p"Desktop EarLab Setup.ism"
cd ..
