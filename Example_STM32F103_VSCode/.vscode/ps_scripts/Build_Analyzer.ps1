#   Build Analyzer
#   Автор: Волков Олег
#   Дата создания скрипта: 26.02.2025
#   GitHub: https://github.com/Solderingironspb
#   Группа Вконтакте: https://vk.com/solderingiron.stm32
#   YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
#   RuTube: https://rutube.ru/channel/36234184/
#   Яндекс Дзен: https://dzen.ru/id/622208eed2eb4c6d0cd16749

#Укажите путь до arm-none-eabi-size.exe
$GCC_PATH = "C:\ST\tools\gcc-arm-none-eabi-10.3-2021.10\bin\arm-none-eabi-size.exe"

# Путь к .elf файлу
$elfFile = $args[0]

# Путь к .map файлу
$mapFile = $args[1]

# Размер памяти RAM (KB)
$ram_size_ld_file = $args[2]

# Размер памяти FLASH (KB)
$flash_size_ld_file = $args[3]

# Memory Details (true/false)
$memory_details = $args[4]


# Проверка, существует ли файл
if (-Not (Test-Path $elfFile)) {
    Write-Host "File $elfFile not found." -ForegroundColor Red
    exit
}

# Проверка, существует ли файл
if (-Not (Test-Path $mapFile)) {
    Write-Host "File $mapFile not found." -ForegroundColor Red
    exit
}


# Чтение содержимого .map файла
$mapContent = Get-Content -Path $mapFile

# Поиск строк с информацией о памяти
$memoryConfig = $mapContent | Select-String -Pattern '^(FLASH|RAM)\s+0x[0-9A-Fa-f]+\s+0x[0-9A-Fa-f]+\s+[a-z]+'

# Создание массива для хранения данных
$memoryData = @()

# Обработка найденных строк
foreach ($line in $memoryConfig) {
    # Разделение строки на части
    $parts = $line -split '\s+'

    # Извлечение значений
    $name = $parts[0]
    $origin = $parts[1]
    $length = $parts[2]
    $attributes = $parts[3]

    # Сохранение значений в хэш-таблицу
    $memoryEntry = @{
        Name = $name
        Origin = $origin
        Length = $length
        Attributes = $attributes
    }

    # Добавление хэш-таблицы в массив
    $memoryData += $memoryEntry
}

# Сохранение данных в переменные
$FLASH = $memoryData | Where-Object { $_.Name -eq "FLASH" }
$RAM = $memoryData | Where-Object { $_.Name -eq "RAM" }

$RAM_Start_address = [int]$RAM.Origin
$RAM_End_address = [int]$RAM.Origin + [int]$RAM.Length 

$Flash_Start_address = [int]$FLASH.Origin
$flash_End_address = [int]$FLASH.Origin + [int]$FLASH.Length 


if (($memory_details -eq "true") -or ($memory_details -eq "True") -or ($memory_details -eq 1)) {
Write-Host "Memory Details:" -ForegroundColor Green
& $GCC_PATH -A $elfFile
}


# Запуск arm-none-eabi-size и получение вывода
$sizeOutput = & $GCC_PATH -A $elfFile

# Извлечение значений
$isr_vectorLine = $sizeOutput | Select-String -Pattern '\.isr_vector'
$textLine = $sizeOutput | Select-String -Pattern '\.text'
$rodataLine = $sizeOutput | Select-String -Pattern '\.rodata'
$init_arrayLine = $sizeOutput | Select-String -Pattern '\.init_array'
$fini_arrayLine = $sizeOutput | Select-String -Pattern '\.fini_array'
$dataLine = $sizeOutput | Select-String -Pattern '\.data'
$bssLine = $sizeOutput | Select-String -Pattern '\.bss'
$_user_heap_stackLine = $sizeOutput | Select-String -Pattern '\._user_heap_stack'

# Проверка, найдены ли строки
if ($isr_vectorLine -and $textLine -and $dataLine -and $rodataLine -and $init_arrayLine -and $fini_arrayLine -and $dataLine -and $bssLine -and $_user_heap_stackLine) {
    # Разделение строк на части и извлечение значений
    $isr_vector = ($isr_vectorLine -split '\s+')[1]
    $text = ($textLine -split '\s+')[1]
    $rodata = ($rodataLine -split '\s+')[1]
    $init_array = ($init_arrayLine -split '\s+')[1]
    $fini_array = ($fini_arrayLine -split '\s+')[1]
    $data = ($dataLine -split '\s+')[1]
    $bss = ($bssLine -split '\s+')[1]
    $_user_heap_stack = ($_user_heap_stackLine -split '\s+')[1]

    # Преобразование в числа
    $isr_vector = [int]$isr_vector
    $text = [int]$text
    $rodata = [int]$rodata
    $init_array = [int]$init_array
    $fini_array = [int]$fini_array
    $data = [int]$data
    $bss = [int]$bss
    $_user_heap_stack = [int]$_user_heap_stack

    # Расчет FLASH и RAM
    [int]$ramUsed =  $data + $bss +$_user_heap_stack
    [int]$flashUsed = $isr_vector + $rodata + $init_array + $fini_array+ $text + $data
    

    # Размеры памяти (замените на свои значения)
    $ramSize = [int]$ram_size_ld_file * 1024    
    $flashSize = [int]$flash_size_ld_file * 1024  


    $ramFree = $ramSize - $ramUsed
    $flashFree = $flashSize - $flashUsed    

    # Расчет процентов
    $flashPercent = [math]::Round(($flashUsed / $flashSize) * 100, 2)
    $ramPercent = [math]::Round(($ramUsed / $ramSize) * 100, 2)

    # Вывод
    Write-Host "Memory Regions:" -NoNewline -ForegroundColor Green
# Создание объектов для таблицы
$data = @(
    [PSCustomObject]@{
        "Region    " = "RAM:" 
        "Start address    " = "0x$("{0:X8}" -f $RAM_Start_address)"
        "End address    " = "0x$("{0:X8}" -f $RAM_End_address)"
        "Size      "   = "$([math]::Round($ramSize / 1024)) KB"
        "Free      " = "$([math]::Round($ramFree / 1024, 2)) KB"
        "Used      "     = "$([math]::Round($ramUsed / 1024, 2)) KB"
        "Usage (%)"    = "$ramPercent%"
    },
    [PSCustomObject]@{
        "Region    " = "FLASH:"
        "Start address    " = "0x$("{0:X8}" -f $FLASH_Start_address)"
        "End address    " = "0x$("{0:X8}" -f $FLASH_End_address)"
        "Size      "   = "$([math]::Round($flashSize / 1024)) KB"
        "Free      " = "$([math]::Round($flashFree / 1024, 2)) KB"
        "Used      "     = "$([math]::Round($flashUsed / 1024, 2)) KB"
        "Usage (%)"    = "$flashPercent%"
    }
)

# Вывод таблицы
$data | Format-Table -AutoSize 
} else {
    Write-Host "Ошибка: Не удалось извлечь данные из вывода arm-none-eabi-size." -ForegroundColor Red
}