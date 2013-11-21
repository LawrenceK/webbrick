CALL ..\..\..\HomeGateway2\Trunk\env.bat

mkdir build
del /F /Q build\*.*
copy web55\* build\*
DEL build\*.rnc
REM
REM The default one is a test file.
DEL build\Values.inc
RENAME build\ValuesLive.inc Values.inc
DEL build\Cfg.inc
RENAME build\CfgLive.inc Cfg.inc
DEL build\Sch.inc
RENAME build\SchLive.inc Sch.inc

svn info | grep Revision > build\Ver
sed -i -e 's/Revision: //' build\Ver

CD ..
CALL UpdateVersion.bat
CD SitePlayer

REM first remove white space, then join all lines then prepend HTTP header
sed -f WhiteSpace.sed <.\web55\wbstatus.xml  | tr -s "\r\n" "  " | sed -f xmlEol.sed >.\build\wbstatus.xml
sed -f WhiteSpace.sed <.\web55\wbcfg.xml  | tr -s "\r\n" "  " | sed -f xmlEol.sed >.\build\wbcfg.xml
REM sed -i -f xmlEol.sed <.\web55\wbstatus.xml >.\build\wbstatus.xml
REM sed -i -f xmlEol.sed <.\web55\wbcfg.xml >.\build\wbcfg.xml

sed -i -f WhiteSpace.sed .\build\webbrick.css
sed -i -f web55.sed .\build\lib.js
sed -i -f web55.sed .\build\Values.inc
sed -i -f web55.sed .\build\Cfg.inc
sed -i -f web55.sed .\build\Sch.inc

..\..\..\..\vendor\netmedia\sitelinker\SiteLinker.exe WB55lib.spd
REM G:\WIN32APP\Siteplayer\SiteLinker.exe WB55lib.spd
