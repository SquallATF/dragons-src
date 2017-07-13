# Microsoft Developer Studio Project File - Name="ultra_server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ultra_server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RajaDB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RajaDB.mak" CFG="ultra_server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ultra_server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ultra_server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DragonRaja/DBDemon", EUAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ultra_server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /map:"../exe/RajaDB.map" /machine:I386 /nodefaultlib:"libc.lib" /out:"../exe/RajaDB.exe" /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ultra_server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib winmm.lib hsel.lib /nologo /subsystem:console /map:"..\exe\DBDemon.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\exe\DBDemon.exe" /pdbtype:sept /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ultra_server - Win32 Release"
# Name "ultra_server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "module"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\inetwork.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mainheader.h
# End Source File
# Begin Source File

SOURCE=.\monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\monitor.h
# End Source File
# Begin Source File

SOURCE=.\network_base.cpp
# End Source File
# Begin Source File

SOURCE=.\network_base.h
# End Source File
# Begin Source File

SOURCE=.\network_guid.h
# End Source File
# Begin Source File

SOURCE=.\packed.cpp
# End Source File
# Begin Source File

SOURCE=.\packed.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\recvmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\recvmsg.h
# End Source File
# Begin Source File

SOURCE=.\servertable.cpp
# End Source File
# Begin Source File

SOURCE=.\servertable.h
# End Source File
# Begin Source File

SOURCE=.\struct.cpp
# End Source File
# Begin Source File

SOURCE=.\Struct.h
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CHINA\ABSTRACTSOCK.CPP
# End Source File
# Begin Source File

SOURCE=.\ASYNC\ASYNC.CPP
# End Source File
# Begin Source File

SOURCE=.\Auction.cpp
# End Source File
# Begin Source File

SOURCE=.\Auction.h
# End Source File
# Begin Source File

SOURCE=.\ChrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\CONNECTION.CPP
# End Source File
# Begin Source File

SOURCE=.\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\DragonLogInServer.cpp
# End Source File
# Begin Source File

SOURCE=.\dragonLoginServer2.cpp
# End Source File
# Begin Source File

SOURCE=.\DrServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\HASHTABLE.CPP
# End Source File
# Begin Source File

SOURCE=.\Hong_Sub.cpp
# End Source File
# Begin Source File

SOURCE=.\Id.cpp
# End Source File
# Begin Source File

SOURCE=.\init_npc_table.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\kein_item_login.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalizingMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalizingMgr.h
# End Source File
# Begin Source File

SOURCE=.\LogInServer.cpp
# End Source File
# Begin Source File

SOURCE=.\LottoDBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\mailmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\MSGPROC.CPP
# End Source File
# Begin Source File

SOURCE=.\mylog.cpp
# End Source File
# Begin Source File

SOURCE=.\CHINA\MYSOCKET.CPP
# End Source File
# Begin Source File

SOURCE=.\MySql.cpp
# End Source File
# Begin Source File

SOURCE=.\NationSys.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\NETWORK.CPP
# End Source File
# Begin Source File

SOURCE=.\network6.cpp
# End Source File
# Begin Source File

SOURCE=.\OP_Chr_Status.cpp
# End Source File
# Begin Source File

SOURCE=.\CHINA\PACKETQ.CPP
# End Source File
# Begin Source File

SOURCE=.\Pay.cpp
# End Source File
# Begin Source File

SOURCE=.\CHINA\POINTDATASTRUCT.CPP
# End Source File
# Begin Source File

SOURCE=.\CHINA\QUERYDBSOCKET.CPP
# End Source File
# Begin Source File

SOURCE=.\r_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\RECVMSGASYNC.CPP
# End Source File
# Begin Source File

SOURCE=.\RMProc.cpp
# End Source File
# Begin Source File

SOURCE=.\Scrp_exe.cpp
# End Source File
# Begin Source File

SOURCE=.\Scrp_int.cpp
# End Source File
# Begin Source File

SOURCE=.\SealStone.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerTable2.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopDemon.cpp
# End Source File
# Begin Source File

SOURCE=.\StepMigration.cpp
# End Source File
# Begin Source File

SOURCE=.\ASYNC\STRUCT_F.CPP
# End Source File
# Begin Source File

SOURCE=.\ultra_server.rc
# End Source File
# Begin Source File

SOURCE=.\WinZS.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CHINA\ABSTRACTSOCK.H
# End Source File
# Begin Source File

SOURCE=.\ASYNC\ASYNC.H
# End Source File
# Begin Source File

SOURCE=.\CharList.h
# End Source File
# Begin Source File

SOURCE=.\ChrLog.h
# End Source File
# Begin Source File

SOURCE=.\CItem.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\CONNECTION.H
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\DEFINE.H
# End Source File
# Begin Source File

SOURCE=.\dragonLoginServer2.h
# End Source File
# Begin Source File

SOURCE=.\DrServerManager.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\HASHTABLE.H
# End Source File
# Begin Source File

SOURCE=.\Hong_Sub.h
# End Source File
# Begin Source File

SOURCE=.\HSEL.h
# End Source File
# Begin Source File

SOURCE=.\Id.h
# End Source File
# Begin Source File

SOURCE=.\ItemList.h
# End Source File
# Begin Source File

SOURCE=.\kein_item.h
# End Source File
# Begin Source File

SOURCE=.\LottoDBMgr.h
# End Source File
# Begin Source File

SOURCE=.\mailmgr.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\MenuDefine.h
# End Source File
# Begin Source File

SOURCE=.\MenuServer.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\MSGPROC.H
# End Source File
# Begin Source File

SOURCE=.\mylog.h
# End Source File
# Begin Source File

SOURCE=.\CHINA\MYSOCKET.H
# End Source File
# Begin Source File

SOURCE=.\MySql.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\NETWORK.H
# End Source File
# Begin Source File

SOURCE=.\Network.h
# End Source File
# Begin Source File

SOURCE=.\Network2.h
# End Source File
# Begin Source File

SOURCE=.\network3.h
# End Source File
# Begin Source File

SOURCE=.\NetWork4.h
# End Source File
# Begin Source File

SOURCE=.\network6.h
# End Source File
# Begin Source File

SOURCE=.\NPC_Pattern.h
# End Source File
# Begin Source File

SOURCE=.\Op_chr_status.h
# End Source File
# Begin Source File

SOURCE=.\CHINA\PACKETQ.H
# End Source File
# Begin Source File

SOURCE=.\pay.h
# End Source File
# Begin Source File

SOURCE=.\CHINA\POINTDATASTRUCT.H
# End Source File
# Begin Source File

SOURCE=.\ASYNC\PROTOCOL.H
# End Source File
# Begin Source File

SOURCE=.\CHINA\QUERYDBSOCKET.H
# End Source File
# Begin Source File

SOURCE=.\ASYNC\RECVMSG.H
# End Source File
# Begin Source File

SOURCE=.\RMDefine.h
# End Source File
# Begin Source File

SOURCE=.\Scrp_exe.h
# End Source File
# Begin Source File

SOURCE=.\Scrp_int.h
# End Source File
# Begin Source File

SOURCE=.\SealStone.h
# End Source File
# Begin Source File

SOURCE=.\ServerIdentity.h
# End Source File
# Begin Source File

SOURCE=.\ServerTable2.h
# End Source File
# Begin Source File

SOURCE=.\ShopDemon.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\StepMigration.h
# End Source File
# Begin Source File

SOURCE=.\ASYNC\STRUCT.H
# End Source File
# Begin Source File

SOURCE=.\WinZS.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\dragon.ico
# End Source File
# Begin Source File

SOURCE=.\HSEL.lib
# End Source File
# End Group
# End Target
# End Project
