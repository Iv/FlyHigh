; NSIS installer script
; Builds the flyhigh windows installer
; Copyright J. Allemann, 2011

; Helper defines
!define PRODUCT_NAME "flyhigh"
!define PRODUCT_VERSION "0.9.0-RC1"
!define PRODUCT_PUBLISHER "flyhigh project"
!define PRODUCT_WEB_SITE "http://flyhigh.sourceforge.net"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

; use bz2 compression
SetCompressor bzip2

; Use Modern UI
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
; Refers to a file named 'COPYING' one level up
!insertmacro MUI_PAGE_LICENSE "..\COPYING"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
var ICONS_GROUP
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
; Offers to lauch flyhigh immediately and read valuable informations
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Run ${PRODUCT_NAME}-${PRODUCT_VERSION}"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchLink"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
; just english
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}\"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "files" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  ; flyhigh binary is expected to reside there:
  File "..\build\src\flyhigh.exe"
  File "flyhigh.ico"
  ; Qt libraries, just the required ones (from Qt SDK)
  File "lib\QtCore4.dll"
  File "lib\QtGui4.dll"
  File "lib\QtNetwork4.dll"
  File "lib\QtSql4.dll"
  File "lib\QtXml4.dll"
  File "lib\QtWebKit4.dll"
  File "lib\Qt3Support4.dll"
  File "lib\phonon4.dll"
  ; some mingw32 libraries (from Qt SDK)
  File "lib\mingwm10.dll"
  File "lib\libgcc_s_dw2-1.dll"
  ; sqlite library, available from www.sqlite.org
  File "lib\sqlite3.dll"
  ; mysql library, availabel from www.mysql.com
  File "lib\libmysql.dll"
  ; sql driver plugins (sqlite from Qt SDK, mysql homebrewed)
  ; must be located in a 'sqldrivers' subdirectory
  SetOutPath "$INSTDIR\sqldrivers"
  File "lib\qsqlmysql4.dll"
  File "lib\qsqlite4.dll"
  SetOutPath "$INSTDIR"
  ; qt path configuration file
  File "qt.conf"
  ; famous readme file
  File "..\README"
  ; convert to dos line-ends
  Push "$INSTDIR\README"
  Push "$INSTDIR\README.txt"
  Call unix2dos

;  File /oname=README.txt "..\README"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk" "$INSTDIR\flyhigh.exe" "" "$INSTDIR\flyhigh.ico" "" "" "" "Flight planning and analysis"
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\flyhigh.exe" "" "$INSTDIR\flyhigh.ico" "" "" "" "Flight planning and analysis"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\flyhigh.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Path" "$INSTDIR"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\flyhigh.ico"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was removed successfully."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you want to remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\flyhigh.exe"
  Delete "$INSTDIR\flyhigh.ico"
  Delete "$INSTDIR\README.txt"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\Qt3Support4.dll"
  Delete "$INSTDIR\phonon4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlmysql4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\sqlite3.dll"
  Delete "$INSTDIR\libmysql.dll"
  Delete "$INSTDIR\qt.conf"
  
  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR"
  
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk"


  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

; helper function for launching a application shortcut
Function LaunchLink
  ExecShell "" "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk"
FunctionEnd

; helper function for converting unix line ends
; lamely stolen from http://nsis.sourceforge.net/UNIX_to_DOS_text_file_format_conversion_script
Function unix2dos
    ; strips all CRs
    ; and then converts all LFs into CRLFs
    ; (this is roughly equivalent to "cat file | dos2unix | unix2dos")
    ;
    ; usage:
    ;    Push "infile"
    ;    Push "outfile"
    ;    Call unix2dos
    ;
    ; beware that this function destroys $0 $1 $2

    ClearErrors

    Pop $2
    FileOpen $1 $2 w 

    Pop $2
    FileOpen $0 $2 r

    Push $2 ; save name for deleting

    IfErrors unix2dos_done

    ; $0 = file input (opened for reading)
    ; $1 = file output (opened for writing)

unix2dos_loop:
    ; read a byte (stored in $2)
    FileReadByte $0 $2
    IfErrors unix2dos_done ; EOL
    ; skip CR
    StrCmp $2 13 unix2dos_loop
    ; if LF write an extra CR
    StrCmp $2 10 unix2dos_cr unix2dos_write

unix2dos_cr:
    FileWriteByte $1 13

unix2dos_write:
    ; write byte
    FileWriteByte $1 $2
    ; read next byte
    Goto unix2dos_loop

unix2dos_done:

    ; close files
    FileClose $0
    FileClose $1

    ; delete original
    Pop $0
    Delete $0

FunctionEnd