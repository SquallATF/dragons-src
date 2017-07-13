# Microsoft Developer Studio Project File - Name="MapServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MapServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapServer.mak" CFG="MapServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MapServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DR103/Source/Server/MapServer", RDIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapServer - Win32 Release"

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
# ADD CPP /nologo /G5 /MT /W3 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Release/vc60.pdb" /map:"..\exe\MapServer.map" /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmtd.lib" /out:"../exe/MapServer.exe" /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MapServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Debug/vc60.pdb" /map:"..\exe\MapServer.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"nafxcw.lib" /out:"..\exe\MapServer.exe" /pdbtype:sept /MAPINFO:EXPORTS /MAPINFO:LINES
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MapServer - Win32 Release"
# Name "MapServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "HigherLayers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HigherLayers\Ability.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Ability.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AdventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AdventManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AIDefine.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AIManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AIManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Area.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Area.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ArenaDef.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ArenaDef.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ArenaManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ArenaManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AssistanceMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AssistanceMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AttackMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\AttackMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Auction.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Auction.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\BaseArena.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\BaseArena.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Battle.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\BattleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\BattleManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CharInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CharInfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CharList.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Chatting.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Chatting.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ChrLog.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ChrLog.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Cinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Cinfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CItem.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CItem.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CoreArena.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CoreArena.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CrackMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CrackMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CursingMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\CursingMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DarkCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DarkCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Debug.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DefaultHeader.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Dr_Network.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DragonServer.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\dragonserver2.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DualManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\DualManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\dungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\dungeon.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\EarthCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\EarthCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\eventmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\eventmgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\EventTreasureBoxMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\EventTreasureBoxMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\exchangemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\exchangemgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ExtraMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ExtraMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\FileMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\FlameCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\FlameCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\FreezeCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\FreezeCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\gamblerate.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\GMMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\GMMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\GuildManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\GuildManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Hong_sub.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Hong_sub.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ImportantListMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ImportantListMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\init_magic_tbl.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\init_npc_table.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Itemlist.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ItemMallManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ItemMallManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ItemMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ItemMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Jnpcenum.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Kein_item.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Kein_item.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\KickOffAll.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\KickOffAll.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LocalizingMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LocalizingMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LogManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LogManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LottoSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LottoSystem.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LTSSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\LTSSupport.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MagicState.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MagicState.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\mailmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\mailmgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Map.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MapInfo.h
# End Source File
# Begin Source File

SOURCE=.\MapServer.cpp
# End Source File
# Begin Source File

SOURCE=.\mapserver.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MenuDefine.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MenuFuction.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Menuserver.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MenuServer.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MenuServer2.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MonsterArena.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MonsterArena.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\MySql.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Mysql.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\nation.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\nation.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\nationsupport.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\NationSys.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\NationSys.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\NationSys_Define.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Network2.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Network3.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\NetWork4.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Network5.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Network6.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Network6.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\network7.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\network7.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\NPC_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Npc_pattern.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Npclist.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Npclist.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Object.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\OP_Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Op_Battle.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\OP_Chr_Status.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Op_chr_status.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\OP_Inv.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\OP_Magic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Op_magic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Path.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\PhysicalAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\PhysicalAttack.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Prison.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Prison.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ProtectionMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ProtectionMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\quest_cpp.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\quest_cpp.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\QuestFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\QuestFunction.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RaidManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RaidManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RajaSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Rajasystem.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RareEffectMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RareEffectMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RecoveryMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RecoveryMagic.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RegenManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\RegenManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ResistState.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ResistState.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\resource.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Scrp_exe.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Scrp_exe.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Scrp_int.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Scrp_int.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SealStone.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SealStone.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SealStoneGameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SealStoneGameServer.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\skill_lsw.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\skill_lsw.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SkillMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SkillMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\StepMigration.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\StepMigration.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\StoneArena.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\StoneArena.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SymbolItemMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\SymbolItemMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TalkWithGM.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TalkWithGM.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TalkWithGMMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TalkWithGMMgr.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\teambattle.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\teambattle.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ThunderCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\ThunderCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\UserManager.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\UserManager.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarStartInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarStartInfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Weather.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WeatherControl.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WeatherControl.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WindCombat.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WindCombat.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WinZS.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Winzs.h
# End Source File
# End Group
# Begin Group "LowerLayers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LowerLayers\define.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\DrServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\DrServerManager.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\GameTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\GameTimer.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\inetwork.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\monitor.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\mylog.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\mylog.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\net_define.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\network_guid.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\packed.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\protocol.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\recvmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\recvmsg.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\Resource.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\ServerIdentity.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\servertable.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\servertable.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\ServerTable2.cpp
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\ServerTable2.h
# End Source File
# Begin Source File

SOURCE=.\LowerLayers\typedef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CoreException.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\GameFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\MapServer.rc
# End Source File
# Begin Source File

SOURCE=.\RMProc.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CoreException.h
# End Source File
# Begin Source File

SOURCE=.\GameDefine.h
# End Source File
# Begin Source File

SOURCE=.\GameFactory.h
# End Source File
# Begin Source File

SOURCE=.\GameSystem.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\RMDefine.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\dragon.ico
# End Source File
# End Group
# Begin Group "Hades War"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HigherLayers\GuardInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\GuardInfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HadesWarfield.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HadesWarfield.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HWState.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HWState.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HWStateFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\HWStateFactory.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\IFactoryDefine.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\IWarfieldDefine.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TeamInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\TeamInfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldFactory.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldInfo.h
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\HigherLayers\WarfieldMgr.h
# End Source File
# End Group
# End Target
# End Project
