#!/bin/bash

# Сделать систему (под линукс) для быстрой развертки кода на ардуино:
# 1. сделать репозиторий кода (просто файлы в директориях, название, версии...)
# 2. с кодом хранить его хеш (в отдельном файле, скажем md5sum)
# 3. написать утилиту (баш/python/perl/c++ скрипты, что вам ближе) которые будут
#     заливать выбранный код во все arduino, которые подключены к компу.
# с т.з. пользователя это может выглядеть так:
# + беру ноут
# + втыкаю в него usb hub на 10+ портов
# + в хаб втыкаю платы arduino (usb кабели)
# + запускаю написанный вами скрипт из консоли с нужными параметрами, типа
#      user@localhost $ nemezida --load uno3 --code follow_line.v3.11
# и на все присоединенные ардуинки заливается код follow_line.v3.11
# для консольной работы с ардуино есть утилита arduino_cli

PATH_TO_ARDUINO_CLI=$HOME/arduino #default for linux
export PATH="$PATH_TO_ARDUINO_CLI:$PATH"
PATH_TO_SKETCHES=$HOME/arduino/sketches
PATH_TO_BUILDS=$HOME/arduino/builds
SKETCH=$1

# При запуске скрипта первым парaметром передать имя заливаемого скетча
if [ -z "$1" ]; then
    echo 'Pass the name of the sketch as the first parameter\nExample: ./auto_upload.sh my_sketch'
    exit 1
fi
# Если есть файл с хеш суммой, то проверяем совпадение хеша и  информируем пользователя при негативном исходе
if [ -f $PATH_TO_SKETCHES/$SKETCH/md5sum ]; then
    if ! [[ $(md5sum $PATH_TO_SKETCHES/$SKETCH/$SKETCH.ino|awk '{print $1}') == \
    $(cat $PATH_TO_SKETCHES/$SKETCH/md5sum|awk '{print $1}') ]]; then
        echo 'Hash sum is not equal, continue? [y,n]'
        read input
        if ! [ "$input" == "y" ]; then
            echo "Process interruption..."
            exit 1
        fi
    fi
else
    md5sum $PATH_TO_SKETCHES/$SKETCH/$SKETCH.ino > $PATH_TO_SKETCHES/$SKETCH/md5sum
fi

BOARD="arduino:avr:uno" # default

# Массив всех портов, к которым подлючены  платы
PORT_ARRAY=$(arduino-cli board list | awk 'NR>1{print $1}')

# Создание директории, где будет хранится компилированный файл
if ! [ -d $PATH_TO_BUILDS/$SKETCH ]; then
    mkdir $PATH_TO_BUILDS/$SKETCH
fi

arduino-cli compile --build-path $PATH_TO_BUILDS/$SKETCH -b $BOARD $PATH_TO_SKETCHES/$SKETCH

for port in ${PORT_ARRAY[@]}
do
    arduino-cli upload -p $port -b $BOARD --input-dir $PATH_TO_BUILDS/$SKETCH
done