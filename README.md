# Sistema de Irrigação Automatizado para Cultura de Milho

Este projeto implementa um sistema de irrigação automatizado para a cultura de milho, utilizando um microcontrolador Arduino. O sistema monitora umidade do solo, pH, e a presença de fósforo e potássio, controlando uma bomba de irrigação com base nas necessidades específicas do milho.

## Objetivo
Automatizar a irrigação do milho, garantindo:
- Umidade do solo entre 40% e 70%.
- pH do solo na faixa de 5,5 a 7,0.
- Presença de fósforo e potássio (potássio considerado presente por padrão).
- Desligamento da irrigação se a umidade atingir 70%, independentemente de outras condições, para evitar encharcamento.

## Componentes do Circuito
- **Arduino Uno (ou compatível)**: Microcontrolador principal.
- **Sensor DHT22**: Mede a umidade do solo (simulada como umidade do ar para testes).
- **LDR (Resistor Dependente de Luz)**: Simula o sensor de pH, mapeando valores analógicos (0-4095) para a faixa de pH 0 a 14.
- **Botão (Fósforo)**: Simula a presença (pressionado, LOW) ou ausência (solto, HIGH) de fósforo.
- **Botão (Potássio)**: Configurado para indicar presença constante de potássio (`potassioOK = true`).
- **Relé**: Controla a bomba de irrigação (ou um LED para testes).
- **Resistores e Jumpers**: Para conexões e pull-ups.

### Esquema de Conexões
| Componente       | Pino Arduino | Descrição                              |
|------------------|--------------|----------------------------------------|
| DHT22 (Dados)    | 23           | Mede umidade do solo (simulada).       |
| Botão Fósforo    | 22           | Simula presença/ausência de fósforo.   |
| Botão Potássio   | 21           | Não usado (potássio sempre presente).  |
| LDR              | 34 (A0)      | Simula pH do solo (0 a 14).            |
| Relé (Sinal)     | 13           | Controla a bomba de irrigação.         |

## Lógica de Controle
O sistema segue estas regras:
1. **Leituras dos Sensores**:
   - **Umidade**: Lida pelo DHT22. Faixa ideal: 40% a 70%.
   - **pH**: Simulado pelo LDR, mapeado de 0-4095 para pH 0-14. Faixa ideal: 5,5 a 7,0.
   - **Fósforo**: Botão pressionado (LOW) indica presença; solto (HIGH) indica ausência.
   - **Potássio**: Configurado como sempre presente (`potassioOK = true`).

2. **Condições de Irrigação**:
   - A bomba é **desligada** se a umidade atingir ou ultrapassar 70%, evitando encharcamento, mesmo que haja deficiência de fósforo, potássio ou pH.
   - A bomba é **ativada** se pelo menos uma das condições abaixo for verdadeira:
     - Umidade < 40%.
     - pH fora da faixa (abaixo de 5,5 ou acima de 7,0).
     - Ausência de fósforo (botão solto).
     - (Potássio não afeta, pois está sempre presente).
   - Se todas as condições estiverem satisfeitas (umidade entre 40% e 70%, pH na faixa, fósforo presente), a bomba permanece **desligada**.

3. **Monitoramento**:
   - As leituras são exibidas no Monitor Serial (115200 baud) a cada 3 segundos, incluindo:
     - Estado de fósforo e potássio ("Sim" ou "Não").
     - Valor do pH e se está na faixa ideal.
     - Umidade do solo e se está na faixa ideal.
     - Estado da irrigação ("Ativa" ou "Inativa").

## Código
O código (`Irrigacao_Milho.ino`) está estruturado em:
- **Setup**: Inicializa o Arduino, configura pinos e o sensor DHT22.
- **Loop**:
   - Lê sensores (umidade, LDR, botão de fósforo).
   - Mapeia o valor do LDR para pH (0 a 14).
   - Aplica a lógica de controle para ativar/desativar o relé.
   - Exibe leituras no Monitor Serial.

## Como Usar
1. **Montagem**:
   - Conecte os componentes conforme o esquema de conexões.
   - Use resistores pull-up nos botões, se necessário.
2. **Upload do Código**:
   - Carregue o código no Arduino usando a IDE Arduino.
3. **Testes**:
   - Abra o Monitor Serial (115200 baud) para visualizar as leituras.
   - Simule condições variando a umidade (DHT22), pH (cobrir/expor o LDR) e fósforo (pressionar/soltar o botão).
4. **Ajustes**:
   - Modifique os pinos ou a lógica no código, se necessário.
   - Para um sensor de pH real, substitua o LDR e ajuste o mapeamento.

## Limitações
- O pH é simulado pelo LDR, o que não reflete condições reais do solo. Um sensor de pH dedicado seria ideal.
- A umidade do solo é simulada pelo DHT22 (umidade do ar). Use um sensor de umidade do solo para maior precisão.
- O potássio é fixo como presente, mas pode ser alterado para leitura dinâmica do botão, se necessário.

## Melhorias Futuras
- Integrar sensores reais de pH e umidade do solo.
- Adicionar um display LCD para leituras locais.
- Implementar controle remoto via Wi-Fi (com ESP32, por exemplo).
- Incluir temporização para irrigação em horários específicos.

## Licença
Este projeto é de código aberto e pode ser modificado ou distribuído livremente.
