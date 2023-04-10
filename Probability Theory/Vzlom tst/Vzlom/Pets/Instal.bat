@ECHO ON
start regsvr32 Cfx32.ocx
REG ADD HKCR\.tst /ve /t REG_SZ  /d "TestPlayer.Document"
REG ADD HKCR\TestPlayer.Document /ve /t REG_SZ  /d "TestPlayer.Document"
REG ADD HKCR\TestPlayer.Document\shell\open\command /ve /t REG_SZ  /d "C:\Pets\TestPlayer.exe %1"
PAUSE

