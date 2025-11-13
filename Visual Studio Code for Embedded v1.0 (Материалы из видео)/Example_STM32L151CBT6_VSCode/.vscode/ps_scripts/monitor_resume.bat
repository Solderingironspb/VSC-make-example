set SCRIPT_DIR=%~dp0
Powershell.exe -executionpolicy bypass -File "%SCRIPT_DIR%monitor_resume_part1.ps1" %name%
Powershell.exe -executionpolicy bypass -File "%SCRIPT_DIR%monitor_resume_part2.ps1" %name%