
set MATLAB=D:\matlab\R2021a

cd .

if "%1"=="" ("D:\matlab\R2021a\bin\win64\gmake"  -f ask.mk all) else ("D:\matlab\R2021a\bin\win64\gmake"  -f ask.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1