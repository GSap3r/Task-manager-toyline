# Task-manager-toyline
Simulador de tarefas para fábrica de brinquedos com TADs dinâmicos em C++. Usa filas de prioridade para distribuir tarefas entre setores e gera relatórios com estatísticas de execução.

# 🧸 Simulador - Fábrica de Brinquedos (TADs Dinâmicos)

Projeto da disciplina **Estruturas de Dados I**, que simula o gerenciamento de tarefas de produção em uma fábrica de brinquedos, utilizando **TADs com listas dinâmicas e fila de prioridades**.

## 📌 Objetivo

Implementar um sistema que gerencia tarefas com diferentes prioridades em setores produtivos, distribuindo tarefas entre operadores ao longo do tempo. Ao fim da simulação, um relatório apresenta estatísticas por setor.

## 🔧 Tecnologias

- Linguagem: **C++**
- Terminal gráfico: `conio2.h` e `windows.h` (Windows)
- Estrutura de dados:
  - TAD com descritor
  - Lista duplamente encadeada (tarefas)
  - Lista simplesmente encadeada (setores)

---

## ⚙️ Funcionalidades

### 🛠️ Simulação

- Adição e remoção de setores durante a execução
- Distribuição inteligente de tarefas para o setor com menos carga
- Fila de prioridade:
  - **Urgente** (alta prioridade)
  - **Normal** (prioridade média)
  - **Opcional** (baixa prioridade)
- Operadores processam as tarefas ao longo da simulação

### 📊 Relatórios Finais

- Tarefas concluídas por setor e por prioridade
- Tempo médio de espera por prioridade
- Tarefas não concluídas por setor

---

## 📁 Estrutura dos Arquivos

- `TrabalhoEDok.cpp`: código principal (menu, simulação e lógica do sistema)
- `TADTrabok.h`: TADs, estruturas e funções auxiliares
- `tarefas.txt`: arquivo de entrada com tarefas

### 📄 Formato de `tarefas.txt`

