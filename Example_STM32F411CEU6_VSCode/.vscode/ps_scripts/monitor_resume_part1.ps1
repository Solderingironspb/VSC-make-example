#   Monitor resume part 1
#   Автор: Волков Олег
#   Дата создания скрипта: 28.02.2025
#   GitHub: https://github.com/Solderingironspb
#   Группа Вконтакте: https://vk.com/solderingiron.stm32
#   YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
#   RuTube: https://rutube.ru/channel/36234184/
#   Яндекс Дзен: https://dzen.ru/id/622208eed2eb4c6d0cd16749

# Данный скрипт - это костыль, чтоб продолжить работу микроконтроллера после закрытия дебага.
# В этой части мы подключимся к микроконтроллеру, используя OpenOCD

$OPEN_OCD_BIN_PATH = "C:/ST/tools/OpenOCD-20240916-0.12.0/bin/openocd.exe"
$OPEN_OCD_INTERFACE_PATH = "C:/ST/tools/OpenOCD-20240916-0.12.0/share/openocd/scripts/interface/stlink.cfg"
$OPEN_OCD_TARGET_PATH = "C:/ST/tools/OpenOCD-20240916-0.12.0/share/openocd/scripts/target/stm32f4x.cfg"

Start-Process -FilePath $OPEN_OCD_BIN_PATH -ArgumentList "-f", "$OPEN_OCD_INTERFACE_PATH", "-f", "$OPEN_OCD_TARGET_PATH" -NoNewWindow
   






























