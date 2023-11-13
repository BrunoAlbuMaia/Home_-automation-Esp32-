
# Sistema de Automação Residencial com ESP32

O projeto de Automação Residencial com ESP32 propõe uma solução inteligente e acessível para o controle e monitoramento de diversos aspectos em um kitnet composto por cinco casas. Este sistema utiliza a potência do ESP32, uma placa microcontroladora versátil, para integrar sensores e atuadores(Relê), proporcionando aos moradores a capacidade de gerenciar a iluminação do corredor, ativar ou desativar um sensor de movimento, e monitorar o nível da caixa d'água por meio de um sensor ultrassônico.

# Componentes-Chave

    ESP32:
        A placa microcontroladora ESP32 serve como o cérebro central do     sistema, executando o código necessário para controlar e monitorar os dispositivos.

![App Screenshot](https://raw.githubusercontent.com/BrunoAlbuMaia/Home_-automation-Esp32-/Master/img/ESP32-C3-DEVKITM-1.jpg)


    Sensor PIR (Detector) de Movimento:
        Sensor PIR (Detector) de Movimento são empregados para a ativação automática da iluminação do corredor, garantindo segurança e economia de energia.

![Sensor PIR (Detector) de Movimento](https://github.com/BrunoAlbuMaia/Home_-automation-Esp32-/blob/Master/img/Sensor%20de%20Movimento.png?raw=true)
    
    Sensor Ultrassonico:
        O sensor ultrassônico é utilizado para medir o nível da caixa d'água, oferecendo uma visão precisa e em tempo real da quantidade hídrico.

![App Screenshot](https://github.com/BrunoAlbuMaia/Home_-automation-Esp32-/blob/Master/img/Sensor%20Ultrassonico.png?raw=true)

    Relê 5V:
        Relés controlam o estado das luzes do corredor, permitindo que sejam ligadas ou desligadas remotamente é importante comprar um relê que tenha um transistor J3Y.

![App Screenshot](https://github.com/BrunoAlbuMaia/Home_-automation-Esp32-/blob/Master/img/Rel%C3%AA%205V.png?raw=true)


## Esquema de Ligação Prático

A imagem abaixo detalha as ligações físicas dos componentes no projeto:

![Ligações dos Componentes](https://github.com/BrunoAlbuMaia/Home_-automation-Esp32-/blob/Master/img/IOT2D.png?raw=true)

**Certifique-se de seguir cuidadosamente estas orientações ao conectar os componentes:**

- **Sensor PIR (Detector) de Movimento:**
  - Conecte o PINO D do Sensor PIR à porta D2 do ESP32.
  - Conecte os pinos VCC e GND do sensor aos pinos 5V e GND do ESP32, respectivamente.

- **Relê:**
  - Conecte os PINOS VCC e GND do Relê aos pinos 5V e GND do ESP32.
  - Conecte o pino D15 do ESP32 ao pino de controle do Relê.
  - No lado de saída do Relê, conecte um fio da lâmpada ao Com (C) e o fio da tomada 220V ao Normalmente Conectado (NC).

- **Sensor Ultrassônico:**
  - Conecte os PINOS VCC e GND do Sensor Ultrassônico aos pinos 5V e GND do ESP32, respectivamente.
  - Conecte os PINOS TRIG e ECHO às portas D13 e D12 do ESP32, respectivamente.

# Software

O software do Sistema de Automação Residencial com ESP32 é crucial para o funcionamento eficiente do sistema. Aqui estão as principais informações sobre a programação e controle:

## Código Fonte

O código fonte para o ESP32 está disponível no arquivo [sketch.ino](https://github.com/BrunoAlbuMaia/Home_-automation-Esp32-/blob/Master/sketch.ino)
 deste repositório. Certifique-se de seguir as etapas abaixo para carregar o código na placa:

## Configuração do Ambiente de Desenvolvimento (IDE Arduino)
Caso você ainda nao tenha o Esp32 configurado na IDE do Arduino, siga os passos deste [LINK](https://www.crescerengenharia.com/post/instalando-esp32-arduino).

Se você já tem o ESP32 configurado na sua IDE do Arduino, siga estas etapas simples para carregar o código fonte:

1. Abra o arquivo `sketch.ino` na IDE do Arduino.

2. Localize a seguinte linha de código:

   ```cpp
   const char* baseUrl = "http://seu_endereco_da_api_aqui/";
Nessa linha acima, você irá colocar o endereço da API Criada, Se você ainda não possui uma API configurada, siga estas etapas passadas nesse repositório
[Link API](https://github.com/BrunoAlbuMaia/APIHome_automation)

3. Verifique se as configurações do modelo de placa estão corretas. Selecione "ESP32 Dev Module" em `Tools` -> `Board`.

4. Certifique-se de que a porta COM correta está selecionada em `Tools` -> `Port`.

5. Clique no botão "Upload" para carregar o código no ESP32.

Pronto! Seu código agora está no ESP32 e pronto para ser executado.


