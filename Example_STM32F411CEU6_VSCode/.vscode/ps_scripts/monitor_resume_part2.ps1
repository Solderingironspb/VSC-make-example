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

$GNU_TOOLCHAIN_GDB_PATH = "C:/ST/tools/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gdb.exe"

& "$GNU_TOOLCHAIN_GDB_PATH" --batch -ex "target extended-remote :3333"  -ex "monitor resume" -ex "detach" -ex "quit"
   







