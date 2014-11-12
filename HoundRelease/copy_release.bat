@echo off | @echo Copying PC release version...
xcopy /D /Y ..\Resources\*.* .\PC\
xcopy /D /Y ..\proj.win32\debug.win32\hound.exe .\PC\
xcopy /D /Y ..\proj.win32\debug.win32\*.dll .\PC\

@echo off | @echo Copying Android release verison...
xcopy /D /Y ..\proj.android\bin\hound.apk .\android\


@pause