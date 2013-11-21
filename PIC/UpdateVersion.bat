cp version_base.h version.h
sed -i -e '/DF_SW_VER_BUILD/d' version.h

REM or COMPUTERNAME
IF "%USERDOMAIN%" == "SILENT" GOTO End

echo #define DF_SW_VER_MAJOR   0 > version.h
echo #define DF_SW_VER_MINOR   0 >> version.h

:End

svn info | grep Revision >> version.h
sed -i -e 's/Revision: /#define DF_SW_VER_BUILD /' version.h
