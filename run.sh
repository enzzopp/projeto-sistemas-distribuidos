#!/bin/bash

SENSOR_EXEC="./sensor"

read -p "Quantos sensores você quer criar? " N

if ! [[ "$N" =~ ^[0-9]+$ ]]; then
  echo "Digite um número inteiro válido."
  exit 1
fi

for ((i=1; i<=N; i++)); do
  $SENSOR_EXEC $i &
  echo "Sensor $i iniciado."
done

echo "$N sensores em execução. Para ver os processos: 'ps aux | grep sensor'"
read -n 1 -s -r -p "Pressione qualquer tecla para encerrar..."
# Encerra todos os sensores
kill $(pgrep -f $SENSOR_EXEC)
