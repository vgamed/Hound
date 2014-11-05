@echo off | @echo Copying PC release version...
copy ..\Resources\*.* .\PC\
copy ..\proj.win32\release.win32\hound.exe .\PC\
copy ..\proj.win32\release.win32\*.dll .\PC\

@echo off | @echo Copying Android release verison...
copy ..\proj.android\bin\hound.apk .\android\


@pause