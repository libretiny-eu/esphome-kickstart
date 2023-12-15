#!/bin/bash

set -e
mkdir -p build/
cd yaml/
configs=("bk7231t" "bk7231n" "rtl8710bn" "esp8266" "esp32")
for config in ${configs[@]}; do
    
    esphome compile kickstart-${config}.yaml
    
    if [ $config = "esp8266" ] || [ $config = "esp32" ]; then
        cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/firmware.bin ../build/kickstart-${config}-$1.bin
    else
        cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/firmware.uf2 ../build/kickstart-${config}-$1.uf2
        cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/*.ota.rbl ../build/ || true
        cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/*.ota.ug.bin ../build/ || true
    fi
done

cp .esphome/build/kickstart-bk7231t/.pioenvs/kickstart-bk7231t/*.ota.rbl ../build/OpenBK7231T_OTA_upgrade_to_esphome_$1.rbl || true
cp .esphome/build/kickstart-bk7231n/.pioenvs/kickstart-bk7231n/*.ota.rbl ../build/OpenBK7231N_OTA_upgrade_to_esphome_$1.rbl || true
