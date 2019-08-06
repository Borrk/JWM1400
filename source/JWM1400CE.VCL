<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: JWM1400CE - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/JWM1400CE.res" /d UNDER_CE=500 /d _WIN32_WCE=500 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_QT2440" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /d "_AFXDLL" /r "\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\JWM1400CE.rc"" 
Creating temporary file "C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP335.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=500 /D _WIN32_WCE=500 /D "WCE_PLATFORM_QT2440" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "_AFXDLL" /Fp"ARMV4IRel/JWM1400CE.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\CSeriesPortPro.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\DataFile.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\JWM1400CE.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\CommuMrg.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\PropSheet.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\SettingDlg.cpp"
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\ShowDlg.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP335.tmp" 
Creating temporary file "C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP336.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=500 /D _WIN32_WCE=500 /D "WCE_PLATFORM_QT2440" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "_AFXDLL" /Fp"ARMV4IRel/JWM1400CE.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP336.tmp" 
Creating temporary file "C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP337.tmp" with contents
[
/nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/JWM1400CE.pdb" /out:"ARMV4IRel/JWM1400CE.exe" /subsystem:windowsce,5.00 /MACHINE:THUMB 
".\ARMV4IRel\clPlot.obj"
".\ARMV4IRel\CSeriesPortPro.obj"
".\ARMV4IRel\DataFile.obj"
".\ARMV4IRel\JWM1400CE.obj"
".\ARMV4IRel\StdAfx.obj"
".\ARMV4IRel\WaveAnaylse.obj"
".\ARMV4IRel\CommuMrg.obj"
".\ARMV4IRel\PropSheet.obj"
".\ARMV4IRel\SettingDlg.obj"
".\ARMV4IRel\ShowDlg.obj"
".\ARMV4IRel\JWM1400CE.res"
]
Creating command line "link.exe @C:\DOCUME~1\B_OPM_~1\LOCALS~1\Temp\RSP337.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
clPlot.cpp
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(195) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(414) : warning C4244: 'initializing' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(425) : warning C4244: '+=' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(493) : warning C4244: 'initializing' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(871) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(889) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(893) : warning C4244: 'initializing' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(896) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(902) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(906) : warning C4244: 'initializing' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(919) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1005) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1006) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1016) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1021) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1023) : warning C4244: '=' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1064) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1093) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1119) : warning C4244: 'initializing' : conversion from 'double' to 'long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1141) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1142) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\clPlot.cpp(1145) : warning C4244: 'argument' : conversion from 'double' to 'unsigned long', possible loss of data
CSeriesPortPro.cpp
DataFile.cpp
JWM1400CE.cpp
WaveAnaylse.cpp
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(913) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(914) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(923) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(924) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(936) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(937) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(948) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\WaveAnaylse.cpp(949) : warning C4244: 'initializing' : conversion from 'double' to 'int', possible loss of data
CommuMrg.cpp
PropSheet.cpp
SettingDlg.cpp
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\SettingDlg.cpp(311) : warning C4101: 'szBuf' : unreferenced local variable
ShowDlg.cpp
\\domain\Ins\B_OPM\B_OPM_RD\B_OPM_SW1\工作目录\code\Jwm1400PC软件\CE\历史\200912011947\JWM1400CE\ShowDlg.cpp(18) : warning C4005: 'EXTEND_Y' : macro redefinition
        \\domain\ins\b_opm\b_opm_rd\b_opm_sw1\工作目录\code\jwm1400pc软件\ce\历史\200912011947\jwm1400ce\commumrg.h(18) : see previous definition of 'EXTEND_Y'
Generating Code...
Linking...




<h3>Results</h3>
JWM1400CE.exe - 0 error(s), 32 warning(s)
</pre>
</body>
</html>
