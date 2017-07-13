# Microsoft Developer Studio Project File - Name="dragon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=dragon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dragon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dragon.mak" CFG="dragon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dragon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dragon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DragonRaja/Client", BXAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dragon - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib dxguid.lib ddraw.lib dinput.lib winmm.lib dsound.lib ws2_32.lib ijl11.lib amstrmid.lib strmiids.lib quartz.lib oldnames.lib DesDll.lib /nologo /subsystem:windows /incremental:yes /map:"..\Exe\dragonraja_c040407_Safe.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\Exe\Å]ªkÀs±Ú.exe" /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "dragon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x417 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib dxguid.lib ddraw.lib dinput.lib winmm.lib dsound.lib ws2_32.lib ijl11.lib amstrmid.lib strmiids.lib quartz.lib oldnames.lib DesDll.lib gdi32.lib advapi32.lib /nologo /subsystem:windows /map:"..\Exe\GM103p_t040407.map" /debug /machine:I386 /out:"..\Exe\GMÅ]ªkÀs±Ú.exe" /pdbtype:sept /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "dragon - Win32 Release"
# Name "dragon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Ability.cpp
# End Source File
# Begin Source File

SOURCE=.\ArenaManager.cpp
# End Source File
# Begin Source File

SOURCE=.\auction.cpp
# End Source File
# Begin Source File

SOURCE=.\Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\char.cpp
# End Source File
# Begin Source File

SOURCE=.\CharDataTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\CMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\CombatSys.cpp
# End Source File
# Begin Source File

SOURCE=.\Compress.cpp
# End Source File
# Begin Source File

SOURCE=.\convert565to555.cpp
# End Source File
# Begin Source File

SOURCE=.\Counseler.cpp
# End Source File
# Begin Source File

SOURCE=.\CrackMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CReadArgument.cpp
# End Source File
# Begin Source File

SOURCE=.\cScreenEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CurrentMsgMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugLog.cpp
# End Source File
# Begin Source File

SOURCE=.\directdraw.cpp
# End Source File
# Begin Source File

SOURCE=.\directinput.cpp
# End Source File
# Begin Source File

SOURCE=.\directsound.cpp
# End Source File
# Begin Source File

SOURCE=.\dragon.cpp
# End Source File
# Begin Source File

SOURCE=.\dragon.rc
# End Source File
# Begin Source File

SOURCE=.\DualMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWndMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Effect.cpp
# End Source File
# Begin Source File

SOURCE=.\exchangemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ftwbrk.cpp
# End Source File
# Begin Source File

SOURCE=.\gameproc.cpp
# End Source File
# Begin Source File

SOURCE=.\Hangul.cpp
# End Source File
# Begin Source File

SOURCE=.\Hong_Light.cpp
# End Source File
# Begin Source File

SOURCE=.\Hong_Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Hong_Sub.cpp
# End Source File
# Begin Source File

SOURCE=.\Horse.cpp
# End Source File
# Begin Source File

SOURCE=.\House.cpp
# End Source File
# Begin Source File

SOURCE=.\HunterSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\item.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemTable.cpp
# End Source File
# Begin Source File

SOURCE=.\kh_cpp.cpp
# End Source File
# Begin Source File

SOURCE=.\kh_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\kh_menuset.cpp
# End Source File
# Begin Source File

SOURCE=.\Language.cpp
# End Source File
# Begin Source File

SOURCE=.\LineCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\LottoMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LTSSupprot.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicSys.cpp
# End Source File
# Begin Source File

SOURCE=.\MailMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.cpp
# End Source File
# Begin Source File

SOURCE=.\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menufunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuStart.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuStartSet.cpp
# End Source File
# Begin Source File

SOURCE=.\MergeData.cpp
# End Source File
# Begin Source File

SOURCE=.\midi.cpp
# End Source File
# Begin Source File

SOURCE=.\MouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Music.cpp
# End Source File
# Begin Source File

SOURCE=.\nationsupport.cpp
# End Source File
# Begin Source File

SOURCE=.\NationSys.cpp
# End Source File
# Begin Source File

SOURCE=.\network.cpp
# End Source File
# Begin Source File

SOURCE=.\network2.cpp
# End Source File
# Begin Source File

SOURCE=.\network6.cpp
# End Source File
# Begin Source File

SOURCE=.\network7.cpp
# End Source File
# Begin Source File

SOURCE=.\NPC_18.cpp
# End Source File
# Begin Source File

SOURCE=.\NPC_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\NumberAniMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\object.cpp
# End Source File
# Begin Source File

SOURCE=.\path.cpp
# End Source File
# Begin Source File

SOURCE=.\Profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\Quest.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\SecuritySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\skill_lsw.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillTool.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallMenuSet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrikeSys.cpp
# End Source File
# Begin Source File

SOURCE=.\sub_funtion.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolMgr_Client.cpp
# End Source File
# Begin Source File

SOURCE=.\teambattle.cpp
# End Source File
# Begin Source File

SOURCE=.\TextOutMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Tool.cpp
# End Source File
# Begin Source File

SOURCE=.\word.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Ability.h
# End Source File
# Begin Source File

SOURCE=.\ArenaManager.h
# End Source File
# Begin Source File

SOURCE=.\auction.h
# End Source File
# Begin Source File

SOURCE=.\Battle.h
# End Source File
# Begin Source File

SOURCE=.\BattleMgr.h
# End Source File
# Begin Source File

SOURCE=.\char.h
# End Source File
# Begin Source File

SOURCE=.\CharDataTable.h
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\chigamec.h
# End Source File
# Begin Source File

SOURCE=.\CMessage.h
# End Source File
# Begin Source File

SOURCE=.\CombatSys.h
# End Source File
# Begin Source File

SOURCE=.\Compress.h
# End Source File
# Begin Source File

SOURCE=.\convert565to555.h
# End Source File
# Begin Source File

SOURCE=.\Counseler.h
# End Source File
# Begin Source File

SOURCE=.\CrackMgr.h
# End Source File
# Begin Source File

SOURCE=.\CReadArgument.h
# End Source File
# Begin Source File

SOURCE=.\cScreenEffect.h
# End Source File
# Begin Source File

SOURCE=.\CurrentMsgMgr.h
# End Source File
# Begin Source File

SOURCE=.\DebugLog.h
# End Source File
# Begin Source File

SOURCE=.\directdraw.h
# End Source File
# Begin Source File

SOURCE=.\directinput.h
# End Source File
# Begin Source File

SOURCE=.\directsound.h
# End Source File
# Begin Source File

SOURCE=.\dragon.h
# End Source File
# Begin Source File

SOURCE=.\DualMgr.h
# End Source File
# Begin Source File

SOURCE=.\EditWndMgr.h
# End Source File
# Begin Source File

SOURCE=.\Effect.h
# End Source File
# Begin Source File

SOURCE=.\exchangemgr.h
# End Source File
# Begin Source File

SOURCE=.\FileMgr.h
# End Source File
# Begin Source File

SOURCE=.\FrameMgr.h
# End Source File
# Begin Source File

SOURCE=.\ftwbrk.h
# End Source File
# Begin Source File

SOURCE=.\gameproc.h
# End Source File
# Begin Source File

SOURCE=.\Hangul.h
# End Source File
# Begin Source File

SOURCE=.\Hong_Light.h
# End Source File
# Begin Source File

SOURCE=.\Hong_Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Hong_Sub.h
# End Source File
# Begin Source File

SOURCE=.\Horse.h
# End Source File
# Begin Source File

SOURCE=.\house.h
# End Source File
# Begin Source File

SOURCE=.\HunterSystem.h
# End Source File
# Begin Source File

SOURCE=.\ijl.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItemTable.h
# End Source File
# Begin Source File

SOURCE=.\Kein_stl.h
# End Source File
# Begin Source File

SOURCE=.\kh_cpp.h
# End Source File
# Begin Source File

SOURCE=.\language.h
# End Source File
# Begin Source File

SOURCE=.\LineCommand.h
# End Source File
# Begin Source File

SOURCE=.\LottoMgr.h
# End Source File
# Begin Source File

SOURCE=.\MagicSys.h
# End Source File
# Begin Source File

SOURCE=.\MailMgr.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\MapInfo.h
# End Source File
# Begin Source File

SOURCE=.\MD5Checksum.h
# End Source File
# Begin Source File

SOURCE=.\MD5ChecksumDefines.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\MenuDef.h
# End Source File
# Begin Source File

SOURCE=.\menudef2.h
# End Source File
# Begin Source File

SOURCE=.\MenuDefine.h
# End Source File
# Begin Source File

SOURCE=.\menufunctions.h
# End Source File
# Begin Source File

SOURCE=.\MenuNetwork.h
# End Source File
# Begin Source File

SOURCE=.\MenuSet.h
# End Source File
# Begin Source File

SOURCE=.\MenuStartSet.h
# End Source File
# Begin Source File

SOURCE=.\MergeData.h
# End Source File
# Begin Source File

SOURCE=.\midi.h
# End Source File
# Begin Source File

SOURCE=.\MouseCursor.h
# End Source File
# Begin Source File

SOURCE=.\Music.h
# End Source File
# Begin Source File

SOURCE=.\mytypes.h
# End Source File
# Begin Source File

SOURCE=.\nationsupport.H
# End Source File
# Begin Source File

SOURCE=.\NationSys.h
# End Source File
# Begin Source File

SOURCE=.\network.h
# End Source File
# Begin Source File

SOURCE=.\Network2.h
# End Source File
# Begin Source File

SOURCE=.\Network3.h
# End Source File
# Begin Source File

SOURCE=.\NetWork4.h
# End Source File
# Begin Source File

SOURCE=.\Network5.h
# End Source File
# Begin Source File

SOURCE=.\Network6.h
# End Source File
# Begin Source File

SOURCE=.\network7.h
# End Source File
# Begin Source File

SOURCE=.\NPC_Pattern.h
# End Source File
# Begin Source File

SOURCE=.\NumberAniMgr.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\path.h
# End Source File
# Begin Source File

SOURCE=.\Profiler.h
# End Source File
# Begin Source File

SOURCE=.\QuestFunction.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SDDIALOG.H
# End Source File
# Begin Source File

SOURCE=.\Sdlang.h
# End Source File
# Begin Source File

SOURCE=.\SealStone.h
# End Source File
# Begin Source File

SOURCE=.\SecuritySystem.h
# End Source File
# Begin Source File

SOURCE=.\Skill.h
# End Source File
# Begin Source File

SOURCE=.\skill_lsw.h
# End Source File
# Begin Source File

SOURCE=.\SkillTool.h
# End Source File
# Begin Source File

SOURCE=.\SmallMenu.h
# End Source File
# Begin Source File

SOURCE=.\SmallMenuSet.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrikeSys.h
# End Source File
# Begin Source File

SOURCE=.\SymbolMgr_Client.h
# End Source File
# Begin Source File

SOURCE=.\teambattle.h
# End Source File
# Begin Source File

SOURCE=.\TextOutMgr.h
# End Source File
# Begin Source File

SOURCE=.\Tool.h
# End Source File
# Begin Source File

SOURCE=.\word.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor4.cur
# End Source File
# Begin Source File

SOURCE=.\dragon.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\ijl11.lib
# End Source File
# Begin Source File

SOURCE=.\DesDll.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ClientLogon.tlh
# End Source File
# Begin Source File

SOURCE=.\ClientLogon.tli
# End Source File
# Begin Source File

SOURCE=.\NoKillDll.dll
# End Source File
# Begin Source File

SOURCE=.\SafeMem_040317.exe
# End Source File
# Begin Source File

SOURCE=.\thidchk.tlh
# End Source File
# Begin Source File

SOURCE=.\thidchk.tli
# End Source File
# End Target
# End Project
