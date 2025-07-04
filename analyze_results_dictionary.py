#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import sys
import re


def plot_all_metrics(csv_file):
    df = pd.read_csv(csv_file)

    # Extrai o número de palavras da coluna Operation (ex: "freq_1000" -> 1000)
    df["Words"] = df["Operation"].apply(lambda op: int(re.search(r"\d+", op).group()))
    df = df.sort_values("Words")

    x = df["Words"]

    fig, axs = plt.subplots(3, 2, figsize=(14, 12))
    fig.suptitle("Desempenho da Estrutura por Número de Palavras", fontsize=16)

    # Subplot 1: Tempo de execução
    axs[0, 0].plot(x, df["Execution_Time_ms"], marker="o", color="blue")
    axs[0, 0].set_title("Tempo de Execução")
    axs[0, 0].set_xlabel("Número de Palavras")
    axs[0, 0].set_ylabel("Tempo (ms)")
    axs[0, 0].grid(True)

    # Subplot 2: Rotações
    axs[0, 1].plot(x, df["Rotations"], marker="s", color="green")
    axs[0, 1].set_title("Número de Rotações")
    axs[0, 1].set_xlabel("Número de Palavras")
    axs[0, 1].set_ylabel("Rotações")
    axs[0, 1].grid(True)

    # Subplot 3: Comparações de chave
    axs[1, 0].plot(x, df["Key_Comparisons"], marker="x", color="purple")
    axs[1, 0].set_title("Comparações de Chave")
    axs[1, 0].set_xlabel("Número de Palavras")
    axs[1, 0].set_ylabel("Comparações")
    axs[1, 0].grid(True)

    # Subplot 4: Profundidade de busca
    axs[1, 1].plot(x, df["Search_Depth"], marker="d", color="red")
    axs[1, 1].set_title("Profundidade de Busca")
    axs[1, 1].set_xlabel("Número de Palavras")
    axs[1, 1].set_ylabel("Profundidade")
    axs[1, 1].grid(True)

    # Subplot 5: Fixups de inserção e deleção
    axs[2, 0].plot(
        x, df["Insertion_Fixups"], marker="^", color="orange", label="Inserção"
    )
    axs[2, 0].plot(x, df["Deletion_Fixups"], marker="v", color="brown", label="Remoção")
    axs[2, 0].set_title("Fixups")
    axs[2, 0].set_xlabel("Número de Palavras")
    axs[2, 0].set_ylabel("Quantidade")
    axs[2, 0].legend()
    axs[2, 0].grid(True)

    # Subplot 6: Mudanças de cor (para árvores rubro-negras)
    axs[2, 1].plot(x, df["Color_Changes"], marker="*", color="black")
    axs[2, 1].set_title("Mudanças de Cor (Red-Black)")
    axs[2, 1].set_xlabel("Número de Palavras")
    axs[2, 1].set_ylabel("Color Changes")
    axs[2, 1].grid(True)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig("performance_overview.png", dpi=300)
    print("✅ Gráfico salvo em performance_overview.png")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python analyze_results.py <arquivo_csv>")
        sys.exit(1)

    plot_all_metrics(sys.argv[1])
