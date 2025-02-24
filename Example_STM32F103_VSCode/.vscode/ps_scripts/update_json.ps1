# Получаем аргумент (имя целевого проекта)
$TARGET = $args[0]
$BUILD_DIR = $args[1]

#Write-Host "Читаем файл c_cpp_properties.json, заменяем строку с name" -ForegroundColor Magenta
Write-Host "replace name: $TARGET in file c_cpp_properties.json" -ForegroundColor Green
(Get-Content .vscode/c_cpp_properties.json) -replace '"name": ".*"', ('"name": "' + $TARGET + '"') | Set-Content .vscode/c_cpp_properties.json

# Читаем файл launch.json, заменяем строку с executable
Write-Host "replace executable: .$BUILD_DIR/$TARGET in file launch.json" -ForegroundColor Green
(Get-Content .vscode/launch.json) -replace '"executable": ".*"', ('"executable": "'+ $BUILD_DIR + '/' + $TARGET + '.elf' + '"') | Set-Content .vscode/launch.json

# Читаем файл tasks.json, заменяем строку с .elf и сохраняем изменения
Write-Host "replace args: $BUILD_DIR/$TARGET.elf in file tasks.json" -ForegroundColor Green
(Get-Content .vscode/tasks.json -Raw) -replace '([\"''])([^\"'']*\.elf)([\"''])', "`$1$BUILD_DIR/$TARGET.elf`$3" | Set-Content .vscode/tasks.json
# Читаем файл tasks.json, заменяем строку с .map и сохраняем изменения
Write-Host "replace args: $BUILD_DIR/$TARGET.elf in file tasks.json" -ForegroundColor Green
(Get-Content .vscode/tasks.json -Raw) -replace '([\"''])([^\"'']*\.map)([\"''])', "`$1$BUILD_DIR/$TARGET.map`$3" | Set-Content .vscode/tasks.json