#!/bin/bash

set -e
mkdir -p build/
cd yaml/
configs=("bk7231t" "bk7231n" "rtl8710bn-2mb-788k")
for config in ${configs[@]}; do
	esphome compile kickstart-${config}.yaml
	cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/firmware.uf2 ../build/kickstart-${config}-$1.uf2
	cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/*.ota.rbl ../build/ || true
	cp .esphome/build/kickstart-${config}/.pioenvs/kickstart-${config}/*.ota.ug.bin ../build/ || true
done
