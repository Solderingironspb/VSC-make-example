#   Monitor resume part 2
#   Автор: Волков Олег
#   Дата создания скрипта: 28.02.2025
#   GitHub: https://github.com/Solderingironspb
#   Группа Вконтакте: https://vk.com/solderingiron.stm32
#   YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
#   RuTube: https://rutube.ru/channel/36234184/
#   Яндекс Дзен: https://dzen.ru/id/622208eed2eb4c6d0cd16749

# Данный скрипт - это костыль, чтоб продолжить работу микроконтроллера после закрытия дебага.
# В этой части мы, используя arm-none-eabi-gdb, подключимся к микроконтроллеру по порту 3333 и отправим команды:
# "monitor resume", "detach", "quit"

$GNU_TOOLCHAIN_GDB_PATH = "C:/ST/tools/gnu-tools-for-stm32.12.3/tools/bin/arm-none-eabi-gdb.exe"
$BUILD_DIR = "Debug"
$TARGET = "Clean_project"

& "$GNU_TOOLCHAIN_GDB_PATH" --batch -ex "file '$BUILD_DIR/$TARGET.elf' " -ex "target extended-remote :3333"  -ex "monitor resume" -ex "monitor shutdown" -ex "detach" -ex "quit"

# Проверка, запущен ли процесс OpenOCD
$openocdProcess = Get-Process -Name "openocd*" -ErrorAction SilentlyContinue

if ($openocdProcess) {
    # Если процесс найден, завершаем его
    Write-Host "OpenOCD is running. Terminating..." -ForegroundColor Red
    Stop-Process -InputObject $openocdProcess -Force
    Write-Host "OpenOCD terminated." -ForegroundColor Green
} else {
    # Если процесс не найден
    Write-Host "OpenOCD is not running." -ForegroundColor Green
}














