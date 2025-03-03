# VSC-make-example
Примеры работ по сборке и отладке програм для windows и микроконтроллеров

Обязательно измените политику запуска скриптов Powersheell.
Сначала узнаем, какая политика сейча: Get-ExecutionPolicy
Если Restricted, то можно заменить ее на Remotesigned или Bypass:
Команда: Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Bypass

Если не хотите этого делать, то придется каждый раз запускать скрипт по типу:
Powershell.exe -executionpolicy bypass -File "C:\script.ps1" %name%
