#!/usr/bin/env python3
"""
Performance Analysis Script for Advanced Data Structures

This script analyzes the benchmark results and provides insights about:
- Execution time
- Number of rotations
- Key comparisons

Usage: python3 analyze_results.py
"""

import pandas as pd
import matplotlib.pyplot as plt


def plot_performance(csv_file):
    # Lê o CSV
    df = pd.read_csv(csv_file)

    # Mostra as colunas para garantir que estão ok
    print("Colunas no CSV:", df.columns.tolist())
    print(df.head())

    # Plotar gráfico de barras para algumas métricas
    # Separando operações distintas
    operations = df["Operation"].unique()

    fig, axs = plt.subplots(len(operations), 1, figsize=(10, 5 * len(operations)))
    if len(operations) == 1:
        axs = [axs]

    for i, op in enumerate(operations):
        subdf = df[df["Operation"] == op]
        x = (
            subdf["Operation"]
            + " ("
            + subdf["Execution_Time_ms"].round(2).astype(str)
            + "ms)"
        )

        axs[i].bar(
            subdf.index,
            subdf["Execution_Time_ms"],
            label="Tempo (ms)",
            color="tab:blue",
            alpha=0.7,
        )
        axs[i].set_title(f"Métricas para operação: {op}")
        axs[i].set_xticks(subdf.index)
        axs[i].set_xticklabels(x, rotation=45, ha="right")
        axs[i].set_ylabel("Tempo (ms)")

        # Plotar outras métricas no mesmo gráfico usando eixo y secundário
        ax2 = axs[i].twinx()
        ax2.plot(
            subdf.index,
            subdf["Rotations"],
            label="Rotações",
            color="tab:red",
            marker="o",
        )
        ax2.plot(
            subdf.index,
            subdf["Key_Comparisons"],
            label="Comparações",
            color="tab:green",
            marker="x",
        )
        ax2.set_ylabel("Contagem (Rotações, Comparações)")

        # Legenda combinada
        lines, labels = axs[i].get_legend_handles_labels()
        lines2, labels2 = ax2.get_legend_handles_labels()
        axs[i].legend(lines + lines2, labels + labels2, loc="upper left")

    plt.tight_layout()
    plt.savefig("performance_plot.png")


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Uso: python plot_performance.py <arquivo_csv>")
        sys.exit(1)

    csv_file = sys.argv[1]
    plot_performance(csv_file)
