# Sistema de Controle de Aula — STM32

Manual de uso do sistema de controle de presença para laboratório, desenvolvido com microcontrolador STM32 e display ST7735.

---

## Mapeamento dos Botões

O sistema utiliza 4 botões dispostos em formato direcional:

```
        [ BOTÃO 12 ]
             ▲
             |
[ BOTÃO 9 ] ◄─── ► [ BOTÃO 11 ]
             |
             ▼
        [ BOTÃO 10 ]
```

| Botão | Posição | Função geral |
|-------|---------|--------------|
| Botão 9  | Esquerda | Confirmar / Entrar |
| Botão 10 | Baixo    | Incrementar / Digitar / Registrar saída |
| Botão 11 | Direita  | Decrementar / Registrar retorno de aluno |
| Botão 12 | Cima     | — |

---

## Fluxo do Sistema

```
Inicialização
     │
     ▼
Tela de início → Pressione Botão 9
     │
     ▼
Digite a senha (6 dígitos via Botão 10)
     │
     ▼
Configurar número máximo de alunos
     │
     ▼
Menu principal (controle da aula)
     │
     ▼
Relatório final (2 minutos)
     │
     ▼
Sistema reinicia automaticamente
```

---

## Passo a Passo

### 1. Inicialização

Ao ligar o sistema, o display exibirá:

```
Pressione o botao para o
comeco da aula
```

Pressione o **Botão 9 (esquerda)** para iniciar.

---

### 2. Autenticação por Senha

O sistema exibirá:

```
Bem vindo, Digite a sua senha:
```

Uma senha de **6 dígitos** é gerada automaticamente e deve ser visualizada pelo professor via debugger no STM32CubeIDE. Siga os passos:

1. Conecte o STM32 ao computador via cabo ST-Link
2. No STM32CubeIDE, clique no ícone de **bug** (🐛) para iniciar o modo debug
3. Coloque um **breakpoint** clicando na barra lateral na linha da geração da senha:
```c
senha = (rand() % 90000) + 100000;  // ← clique aqui para o breakpoint
```
4. Pressione **F8** para rodar até o breakpoint
5. Na aba **Variables** (canto inferior esquerdo), localize a variável `senha` — o valor exibido é a senha gerada
6. Anote a senha e pressione **F8** novamente para continuar a execução normal

**Como digitar cada dígito:**

Pressione o **Botão 10 (baixo)** repetidamente para selecionar o número desejado:

| Número de cliques | Dígito |
|:-----------------:|:------:|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| 4 | 4 |
| 5 | 5 |
| 6 | 6 |
| 7 | 7 |
| 8 | 8 |
| 9 | 9 |
| 10 | 0 |

Aguarde **1 segundo** sem pressionar para confirmar o dígito. Repita para os 6 dígitos.

**Resultado:**
- Senha correta → `ACESSO LIBERADO` (verde)
- Senha errada → `SENHA INCORRETA, TENTE NOVAMENTE` (vermelho) — tente novamente desde o primeiro dígito
- 3 erros consecutivos → sistema bloqueia por **30 segundos** antes de permitir nova tentativa

---

### 3. Configuração de Turma

```
Digite a quantidade de alunos
Alunos: 0
```

| Botão | Ação |
|-------|------|
| Botão 10 (baixo) | Incrementar quantidade |
| Botão 11 (direita) | Decrementar quantidade |
| Botão 9 (esquerda) | Confirmar |

> Não é possível confirmar com 0 alunos.

---

### 4. Menu Principal — Controle da Aula

O display exibirá o estado atual da sala:

```
Max de alunos:    XX
Alunos na sala:   XX
Alunos fora:      XX

[========          ] ← barra de ocupação
```

A barra de ocupação é exibida em verde e cresce proporcionalmente conforme os alunos entram na sala, com borda branca delimitando o espaço total disponível.

**Ações disponíveis:**

| Botão | Ação |
|-------|------|
| Botão 9 (esquerda) | Abrir tela de entrada de alunos |
| Botão 10 (baixo) | Registrar saída (banheiro/água) |
| Botão 11 (direita) | Registrar retorno do aluno |
| Botão 11 + Botão 12 simultaneamente | Encerrar aula e exibir relatório |

> Máximo de **3 alunos** fora da sala simultaneamente — valor configurável no código.  
> Não é possível registrar saída se não houver alunos na sala.  
> Não é possível registrar retorno se não houver alunos fora.

---

### 5. Tela de Entrada de Alunos

```
Aperte o botao 9 para entrar
Bem Vindo, Entrou: XXXX  ← matrícula
[========          ]
```

| Botão | Ação |
|-------|------|
| Botão 9 (esquerda) | Registrar entrada de um aluno |
| Botão 10 (baixo) | Voltar ao menu principal |

Cada aluno recebe uma matrícula sequencial a partir de **1000**. O timer da aula começa a contar a partir do momento em que a tela de entrada é aberta pela primeira vez.

> Se a sala estiver cheia, o sistema exibe `SalaCheia` em vermelho e bloqueia novas entradas.

---

### 6. Relatório Final

Pressione **Botão 11 + Botão 12 simultaneamente** no menu principal para encerrar a aula e exibir o relatório:

```
RelatorioFinal
Total de alunos presentes: XX
Total de saidas:           XX
Total de retornos:         XX
Tempo da aula:          MM:SS
```

O relatório fica exibido por **2 minutos** automaticamente. Após esse tempo, o sistema se reinicia completamente, voltando à tela inicial pronto para uma nova aula.

---

## Vídeo de Apresentação

[Assistir no YouTube](https://youtu.be/B5gMoBTCBUY)

---

## Decisões de Implementação

### Estrutura do código
O código foi organizado em funções separadas para cada etapa do fluxo (`inicio`, `digitarsenha`, `configurarAlunos`, `menu`, `entrada`, `relatorio`), facilitando a leitura e manutenção.

### Sistema de senha
A senha de 6 dígitos é gerada aleatoriamente com `rand()` usando `HAL_GetTick()` como semente, garantindo que seja diferente a cada execução. A senha fica visível apenas via inspeção de variável no debugger, sem ser exibida no display, conforme especificado no projeto.

### Bloqueio por tentativas
Após 3 erros consecutivos na digitação da senha, o sistema exibe uma mensagem de bloqueio e aguarda 30 segundos antes de permitir novas tentativas, protegendo o sistema contra acesso não autorizado.

### Debounce dos botões
Para evitar leituras duplicadas, após cada acionamento de botão foi aplicado `HAL_Delay(200)` seguido de espera pelo soltar do botão (`while(BotaoLe == 0)`), garantindo uma leitura por pressão.

### Controle de saída
O limite de 3 alunos fora foi implementado via variável local `alunosFora` com verificação antes de permitir nova saída. O retorno só é permitido se houver alunos fora, evitando valores negativos.

### Barra de ocupação
A barra de progresso foi implementada com `ST7735_FillRectangle` em cor verde, com borda branca ao redor para delimitar visualmente o espaço total disponível. A barra cresce proporcionalmente conforme os alunos entram, dando feedback visual imediato ao professor.

### Tempo de aula
O tempo de aula começa a ser contado no momento em que a tela de entrada de alunos é aberta pela primeira vez (via `inicioAula = HAL_GetTick()` dentro de `menu()`). O relatório calcula a diferença entre esse valor e o tick atual, exibindo o resultado no formato `MM:SS`.

### Matrícula via "leitor facial"
O enunciado pedia para simular um leitor facial que envia a matrícula do aluno. Interpretamos isso como uma variável interna incrementada automaticamente a cada entrada, começando em 1000, simulando o recebimento de uma matrícula externa.

### Encerramento e reinício
Ao pressionar Botão 11 + Botão 12 simultaneamente, o sistema exibe o relatório final por 2 minutos e reinicia automaticamente, zerando todas as variáveis para uma nova aula, sem necessidade de desligar o equipamento.

---

## Integrantes do Grupo

- Layon Rubens Motta de Freitas
- Marcelo Vitor da Silva Pereira

---
