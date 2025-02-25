#   Makefile update json
#   Автор: Волков Олег
#   Дата создания скрипта: 26.02.2025
#   GitHub: https://github.com/Solderingironspb
#   Группа Вконтакте: https://vk.com/solderingiron.stm32
#   YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
#   RuTube: https://rutube.ru/channel/36234184/
#   Яндекс Дзен: https://dzen.ru/id/622208eed2eb4c6d0cd16749

# Получаем аргумент (имя целевого проекта)
$TARGET = $args[0]
$BUILD_DIR = $args[1]

#Write-Host "Читаем файл c_cpp_properties.json, заменяем строку с name" -ForegroundColor Magenta
# Читаем текущее значение поля "name" из файла
$fileContent = Get-Content .vscode/c_cpp_properties.json -Raw -Encoding utf8
$oldName = ($fileContent | Select-String -Pattern '"name": "(.*?)"' -AllMatches).Matches.Groups[1].Value
Write-Host "file c_cpp_properties.json: " -ForegroundColor DarkYellow -NoNewline  
Write-Host "string replacement 'name: $oldname' -> 'name: $TARGET' " -ForegroundColor Green
(Get-Content .vscode/c_cpp_properties.json) -replace '"name": ".*"', ('"name": "' + $TARGET + '"') | Set-Content .vscode/c_cpp_properties.json


# Читаем файл launch.json, заменяем строку с executable
$fileContent = Get-Content .vscode/launch.json -Raw -Encoding utf8
$oldName = ($fileContent | Select-String -Pattern '"executable": "(.*?)"' -AllMatches).Matches.Groups[1].Value
Write-Host "file launch.json: " -ForegroundColor DarkYellow -NoNewline  
Write-Host "string replacement 'executable: .$oldname' -> 'executable: .$BUILD_DIR/$TARGET.elf' " -ForegroundColor Green
(Get-Content .vscode/launch.json) -replace '"executable": ".*"', ('"executable": "'+ $BUILD_DIR + '/' + $TARGET + '.elf' + '"') | Set-Content .vscode/launch.json

# Читаем файл tasks.json, заменяем строку с .elf и сохраняем изменения
Write-Host "file tasks.json: " -ForegroundColor DarkYellow -NoNewline
Write-Host "string replacement 'args: $BUILD_DIR/$TARGET.elf' " -ForegroundColor Green
(Get-Content .vscode/tasks.json -Raw -Encoding utf8) -replace '([\"''])([^\"'']*\.elf)([\"''])', "`$1$BUILD_DIR/$TARGET.elf`$3" | Out-File .vscode/tasks.json -Encoding utf8


# Читаем файл tasks.json, заменяем строку с .map и сохраняем изменения
Write-Host "file tasks.json: " -ForegroundColor DarkYellow -NoNewline
Write-Host "string replacement 'args: $BUILD_DIR/$TARGET.map' " -ForegroundColor Green
(Get-Content .vscode/tasks.json -Raw -Encoding utf8) -replace '([\"''])([^\"'']*\.map)([\"''])', "`$1$BUILD_DIR/$TARGET.map`$3" | Out-File .vscode/tasks.json -Encoding utf8