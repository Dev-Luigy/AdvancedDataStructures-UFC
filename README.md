# 📊 Estruturas de Dados Avançadas – Contador de Frequência e Benchmark

Este projeto implementa e compara diferentes estruturas de dados em C++ para realizar:

- Contagem de frequência de palavras com suporte a Unicode (ICU)
- Benchmarks de tempo de inserção/remoção/consulta
- Visualização de resultados com Python

---

## 🧩 Estruturas suportadas

- `AVLTree`
- `RedBlack`
- `OpenHashMap`
- `ExternHashMap`

Todas seguem uma interface comum e são integradas por uma factory.

---

## ⚙️ Compilação

Use o Makefile para compilar os dois alvos:

```bash
make
```

Isso irá gerar dois binários:

- `main` — executa o programa de contagem de palavras
- `benchmark` — executa testes de desempenho e gera o arquivo `performance_results.csv`

---

## ▶️ Execução

### 🔠 Rodar o contador de frequência (`main`)

```bash
./main <estrutura> <arquivo_texto>
```

#### 📌 Exemplos:

```bash
./main avl texto.txt
./main rb texto.txt
./main open texto.txt
./main extern texto.txt
```

O programa irá:

- Ler o arquivo `texto.txt`
- Normalizar as palavras com ICU (acentuação, maiúsculas etc.)
- Contar frequência usando a estrutura especificada
- Mostrar todas as palavras com suas contagens ordenadas pela lógica da estrutura

---

### ⏱️ Rodar o benchmark

```bash
make run-benchmark
```

Isso irá gerar um arquivo:

```
performance_results.csv
```

Contendo os tempos de execução para inserções, remoções e consultas.

---

## 📊 Visualização com Python

### 1. Criar ambiente virtual:

```bash
python3 -m venv .venv
```

### 2. Ativar ambiente:

#### Linux/macOS Bash:

```bash
source .venv/bin/activate
```

#### Fish shell:

```bash
source .venv/bin/activate.fish
```

### 3. Instalar dependências:

```bash
pip install -r requirements.txt
```

> O `requirements.txt` deve conter:
>
> ```
> pandas
> matplotlib
> ```

### 4. Rodar análise de resultados:

```bash
.venv/bin/python analyze_results.py
```

Ou com ambiente ativado:

```bash
python analyze_results.py
```

---

## 🧹 Limpeza

Para remover os binários e o CSV de resultados:

```bash
make clean
```

---

## 📁 Estrutura do Projeto

```
.
├── main.cpp
├── benchmark.cpp
├── analyze_results.py
├── factory/
│   └── makeStructury.{hpp,cpp}
├── structures/
│   ├── Trees/
│   └── Data/
├── interfaces/
├── texto.txt  (exemplo de entrada)
├── performance_results.csv
├── .venv/
├── Makefile
└── README.md
```

---

## 🛠️ Requisitos

### C++

- `g++` com suporte a C++17
- `pkg-config`
- `libicu-dev` (ICU para Unicode)

Instalar no Ubuntu:

```bash
sudo apt install g++ pkg-config libicu-dev
```

### Python

- Python 3.7+
- `venv`, `pip`, `pandas`, `matplotlib`

---
