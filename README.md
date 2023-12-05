# Porta Precisa

A Porta Precisa é uma tranca eletrônica instalada na Sala Precisa que utiliza tecnologia de cartões RFID e que mantém um registro de usuários.

## Requerimentos

Este projeto usa as seguintes peças:

- Arduino UNO programado em C/C++;
- módulo RFID – RC522;
- módulo para inclusão de cartão microSD;
- módulo de relê;
- tela LCD 16x2 com o Modulo I2C;
- solenóide;
- botão para acionamento, como alternativa à leitura RFID;
- fontes de 5v e 12v, para alimentar, respectivamente: Arduino e módulos; e a solenóide.
- duas caixas confeccionadas mediante impressão 3d, que comportarão todos os outros itens.

## Instalação

É necessário instalar a tranca usando uma porta e duas tomadas. Pode ser necessário furar a porta.

## Configuração

1. É necessário preencher o documento que está no cartão microSD, denominado data.txt, com os nomes e as chaves de todos os usuários.
1. Após preencher esse documento por qualquer meio, insira o cartão microSD no módulo de SD no local indicado na caixa.
1. O arquivo com logs estará no outro .txt dentro do microSD.

## Bibliotecas

- SD.h, SPI.h: Para interface com o cartão SD.
- MFRC522.h: Para funcionalidades do leitor RFID.
- Wire.h: Para comunicação I2C.
- LiquidCrystal_I2C.h: Para controlar o display LCD.
- time.h: Para manipulação de funções relacionadas a tempo.

## Definições e Variáveis Globais

- SS_PIN, RST_PIN, SDCARD_CS_PIN: Definem os pinos para comunicação SPI com o leitor RFID e o cartão SD.
- col, lin, ende: Definem as dimensões e endereço para o display LCD.
- LiquidCrystal_I2C lcd: Inicializa o objeto do display LCD.
- RelePin: Pino para controle da fechadura da porta.
- File myFile, dataFile: Objetos de arquivo para manipulação de dados do cartão SD.
- MFRC522: Inicializa o objeto do leitor RFID.
- Diversas variáveis de bytes e strings para manipulação de dados RFID e status.

## Funções

- resetVariables(): Reseta dados do usuário e variáveis de status.
- mensageminicial(): Exibe a mensagem inicial no LCD.
- liberar(String name): Concede acesso, ativa o relé, registra o evento e mostra mensagem de boas-vindas.
- invalida(String key): Exibe mensagem de acesso negado e registra o evento.
- registro(String name, String Key): Registra o evento no arquivo de log.
- setup(): Inicializa a comunicação serial, SPI, leitor RFID, cartão SD, display LCD e define o estado inicial do relé.
- loop(): Loop principal para verificar continuamente a presença do cartão RFID, ler dados do cartão, validar usuários e acionar ações de controle de acesso.

## Lógica Principal

- O programa inicializa os componentes de hardware necessários no setup().
- No loop(), ele aguarda um cartão RFID, lê seus dados e verifica contra os dados do usuário armazenados no cartão SD.
- Se um usuário válido for detectado, ele aciona o relé, registra o evento e mostra uma mensagem de boas-vindas.
- Se um cartão inválido for apresentado, ele registra a tentativa e mostra uma mensagem de acesso negado.

## Pessoal Autorizado

Estão autorizado a manipular Porta Precisa a equipe de mantenedores, os Centros Acadêmicos de Engenharia da Computação e Telecomunicações (CAECOMP e DAETEL), a Faculdade Computação e Telecomunicações, e o Instituto de Tecnologia da Universidade Federal do Pará.

## Mantenedores

- Nillson Silva - nillson.santos@itec.ufpa.br
- R. Gabriel Silva - roberto.menezes.silva@itec.ufpa.br
- Ramiro Silva - ramiro.silva@itec.ufpa.br
- Luan Freitas - luan.freitas@itec.ufpa.br