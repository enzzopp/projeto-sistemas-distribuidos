# Sistema de Sincronização de Sensores IoT – Projeto em C

## Descrição

Este projeto simula um sistema distribuído de sensores IoT para monitoramento ambiental em um campo agrícola. Vários sensores independentes coletam dados fictícios (como temperatura, umidade, vento, pressão, UV, pH e localização), registram o horário local com milissegundos e enviam periodicamente essas informações para um centralizador via rede TCP/IP. O centralizador recebe, exibe e armazena os dados recebidos em um arquivo de log.

## Arquivos

- sensor.c: Programa do sensor que gera e envia dados para o centralizador.
- centralizador.c: Servidor centralizador que recebe, exibe e grava os dados em log.txt.
- rodar_sensores.sh: Script Bash que permite iniciar vários sensores em paralelo de forma automática.

## Como compilar

Compile os programas usando o GCC:

gcc -o centralizador centralizador.c -lpthread
gcc -o sensor sensor.c

## Como executar

1. Inicie o centralizador em um terminal:
./centralizador

2. Em outro terminal, execute o script para iniciar múltiplos sensores:
chmod +x rodar_sensores.sh
./rodar_sensores.sh

Ou, se preferir iniciar sensores manualmente:
./sensor 1 &
./sensor 2 &
./sensor 3 &
# ... e assim por diante

## Funcionamento

- Cada sensor gera dados ambientais simulados, monta uma mensagem com o horário local (com milissegundos), ID e localização, e envia para o centralizador em intervalos aleatórios.
- O centralizador recebe as conexões de vários sensores ao mesmo tempo (multithread), mostra no terminal e grava cada linha recebida no arquivo log.txt.

Exemplo de mensagem no log:
[2025-06-14 21:36:28.153] ID: 2 | Temp: 21.22°C | Umidade: 54.93% | Vento: 12.34km/h | Pressão: 1002.18hPa | UV: 4.21 | pH: 6.31 | Localização: (-45.00, 120.00)

## Como monitorar e encerrar sensores

- Para ver os sensores rodando:
ps aux | grep sensor

- Para encerrar todos os sensores:
    - Se estiver usando o script, basta pressionar qualquer tecla quando solicitado.
    - Ou, manualmente:
pkill sensor

## Observações

- O sistema usa apenas o horário local do sensor para registro dos dados.
- O arquivo de log será criado no mesmo diretório do centralizador.
- Para simular em rede, altere o valor da constante HOST em sensor.c para o IP do computador onde o centralizador está rodando.