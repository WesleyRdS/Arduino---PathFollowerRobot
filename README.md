# PathFollowerRobot

Este repositório contém o código para um robô autônomo projetado para navegar em uma grade de células pré-determinada, seguindo um trajeto definido. O robô utiliza motores DC controlados por ponte H para realizar movimentos entre as células, ajustando sua posição e orientação com base nas coordenadas fornecidas.

## Principais Funcionalidades  
- **Navegação baseada em coordenadas**: O robô utiliza uma lista de posições `(y, x)` para seguir um trajeto predeterminado.  
- **Controle de movimento**: Movimentos lineares e rotações são calculados com base no diâmetro das rodas, distância entre células e a orientação atual.  
- **Ajuste de orientação**: O robô ajusta sua direção (90°, 180°, 270° ou 0°) conforme necessário para seguir o trajeto.  
- **Controle de motores**: Implementação simples para controlar dois motores DC utilizando uma ponte H.


## Problemas  
- **Falta de precisão nas rotações**:  
  - O kit básico de Arduino, embora inclua um "encoder", não possui o emissor e receptor necessários para utilizá-lo. Isso impede a medição precisa do número de rotações das rodas individualmente.  
  - O sistema depende de aproximações para estimar movimentos.  

- **Movimentação não linear**:  
  - A roda castor móvel fornecida no kit causa uma oscilação na direção do movimento do robô, resultando em trajetórias que não são perfeitamente retas.
 

## Como Funciona  
1. **Definição do Trajeto**: O trajeto é fornecido como uma string no formato de lista de tuplas `[(y1, x1), (y2, x2), ...]`.  
2. **Posição Inicial**: A posição inicial e a orientação do robô são definidas.  
3. **Execução do Trajeto**: O robô processa as mudanças de coordenadas para se mover de célula em célula, ajustando sua orientação conforme necessário.  
4. **Movimentos Precisos**: Funções específicas controlam os motores para movimentos lineares e rotações suaves.  

## Configuração  
- **Parâmetros do robô**:  
  - Diâmetro do robô: `14.3 cm`  
  - Raio da roda: `4.2 cm`  
  - Distância entre células: `40 cm`  
  - Velocidade de rotação: `2.3 rotações por segundo (RPS)`  

- **Pinos do motor**: Configurados no código para uso com Arduino.  
- **Bibliotecas necessárias**: Nenhuma biblioteca externa é usada, apenas funções nativas do Arduino.  

## Arquitetura do Código  
- **Classe `DCMotor`**: Simplifica o controle dos motores DC (frente, ré e parar).  
- **Funções de Movimento**: Gerenciam os deslocamentos lineares e rotações.  
- **Funções de Trajetória**: Processam a lista de coordenadas e calculam os movimentos necessários.  

## Exemplo de Trajeto  
```cpp
String path_data = "[(6, 1), (7, 1), (8, 1), (9, 1), (10, 1)]";
String initial_position = "(6, 0)";
int initial_orientation = 0;
runTrajectory(path_data, initial_position, initial_orientation);


