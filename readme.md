<table align="center">
  <tr>
    <td><img src="image.png" width="100"></td>
    <td align="center">
      <b>Embarca Tech - CEPEDI</b><br>
      Capacitação Inicial em Sistemas Embarcados<br>
      U4 - Microcontroladores <br>
      <b>Discente - Rodrigo Damasceno Sampaio (Grupo 9)</b>
    </td>
    <td><img src="image2.png" width="100"></td>
  </tr>
</table>


## 🌟 Projeto: Controle de Matriz RGB e Display OLED
Este projeto integra uma matriz de LEDs RGB (5x5) e um display OLED SSD1306 na placa Raspberry Pi Pico , utilizando a BitDogLab . O sistema é controlado por botões físicos e comunicação serial via USB , permitindo:

Exibir números (0-9) na matriz RGB ao receber caracteres via serial.
Atualizar o display OLED com o estado dos LEDs (verde e azul) e o último caractere recebido.
Controlar os LEDs RGB e resetar o dispositivo usando interrupções de botão.

## 🎯 Objetivos

✅ Implementar controle de matriz RGB usando PIO (Programmable I/O).

✅ Integrar um display OLED SSD1306 para exibir informações dinâmicas.

✅ Utilizar interrupções (IRQ) para detectar eventos de botão sem polling.

✅ Receber e processar dados via comunicação serial para interação em tempo real.

## 📚 Descrição do Projeto
O projeto utiliza a BitDogLab (RP2040) para integrar múltiplos componentes:

Matriz RGB : Exibe números (0-9) ou apaga-se quando recebe caracteres não numéricos.
Display OLED : Mostra o estado dos LEDs (verde e azul) e o último caractere recebido via serial.
Botões :
Botão A : Alterna o estado do LED verde.
Botão B : Alterna o estado do LED azul.
Joystick Button : Reseta o dispositivo.
Comunicação Serial : Permite enviar caracteres via terminal para interagir com o sistema.

#### 📌 O loop principal continua rodando normalmente , enquanto as interrupções e temporizações garantem o funcionamento sem bloqueios.

| 🛠 Componentes e Conexões | GPIO  |
|--------------------------|------|
| 🔴 LED Vermelho         | 13   |
| 🔵 LED Azul            | 12   |
| 🟢 LED Verde           | 11   |
| 🔘 Botão A            | 5    |
| 🔘 Botão B            | 6    |
| 🔘 Joystick Button    | 22   |
| 🖥 Display OLED (SDA)  | 14   |
| 🖥 Display OLED (SCL)  | 15   |
| 🎨 Matriz RGB         | 7    |


## 🚀 Como Rodar o Projeto

### 📌 **1️⃣ Clonar o Repositório**
```bash
git clone https://github.com/Rodrigodsgit/U4-display-ssd1306.git
cd semaforo
```

### 📌 **2️⃣ Configurar o Ambiente**
Instale a extensão da Raspberry Pi Pico Project no VsCode  

### 📌 3️⃣ Compilar o Código
Através da extensão, use o acesso rapido e clique em "Compile Project"

### 📌 4️⃣ Rode o Código
Através da extensão, use o acesso rapido e clique em "Run Project (USB)"

## 🚀 Video Demonstração

<iframe width="560" height="315" src="https://youtube.com/shorts/vCkb-oRNddI?feature=share" frameborder="0" allowfullscreen></iframe>

[🎥 Assista ao vídeo](https://youtube.com/shorts/vCkb-oRNddI?feature=share)

