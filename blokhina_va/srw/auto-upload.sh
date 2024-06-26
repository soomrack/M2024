#!/bin/bash

PATH_TO_ARDUINO_CLI=$HOME/arduino
export PATH="$PATH_TO_ARDUINO_CLI:$PATH"
PATH_TO_SKETCHES=./sketches
PATH_TO_BUILDS=./builds

# При запуске скрипта первым парaметром передать имя заливаемого скетча
if [ -z "$1" ]; then
    echo -e "Pass the name of the sketch as the first parameter.\nExample: ./auto_upload.sh my_sketch"
    exit 1
fi

SKETCH=$1

if ! [ -f $PATH_TO_SKETCHES/$SKETCH/$SKETCH.ino ]; then
    echo "Sketch '$SKETCH' does not exist."
    exit 1
fi

# Проверяем совпадение хеш суммы
if [ -f $PATH_TO_SKETCHES/$SKETCH/md5sum ]; then
    if ! [[ $(md5sum $PATH_TO_SKETCHES/$SKETCH/$SKETCH.ino|awk '{print $1}') == \
    $(cat $PATH_TO_SKETCHES/$SKETCH/md5sum|awk '{print $1}') ]]; then
        echo "Hash sum are not equal. Update md5sum file in the folder where the sketch is located."
        exit 1
    fi
else
    echo "Create md5sum file in the folder where the sketch is located."
    exit 1
fi

BOARD="arduino:avr:uno" # default

# Массив всех портов, к которым подлючены  платы
PORT_ARRAY=$(arduino-cli board list | awk 'NR>1{print $1}')

if [ ${#PORT_ARRAY[@]} == 0 ]; then
    echo "No boards connected. No available ports."
    exit 1
fi
# Создание директории, где будет хранится компилированный файл
if ! [ -d $PATH_TO_BUILDS/$SKETCH ]; then
    mkdir $PATH_TO_BUILDS/$SKETCH
fi

arduino-cli compile --build-path $PATH_TO_BUILDS/$SKETCH -b $BOARD $PATH_TO_SKETCHES/$SKETCH

for port in ${PORT_ARRAY[@]}
    do
        echo "Deploying on port $port, continue? [y,n]"
        read input
        while ! [[ "$input" == "y" || "$input" == "n" ]]; do
            echo "Just input 'y' or 'n'"
            read input
        done
        if ! [ "$input" == "y" ]; then
            echo "Process interruption..."
            exit 1
        fi
        arduino-cli upload -p $port -b $BOARD --input-dir $PATH_TO_BUILDS/$SKETCH
done