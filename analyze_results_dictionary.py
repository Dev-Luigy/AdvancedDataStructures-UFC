#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import sys
import numpy as np


def plot_all_metrics(csv_file):
    df = pd.read_csv(csv_file)

    # Use Data_Size instead of extracting from Operation
    df = df.sort_values("Data_Size")

    # Get unique structures
    structures = df["Structure"].unique()
    colors = ["blue", "green", "red", "orange"]
    markers = ["o", "s", "^", "d"]

    fig, axs = plt.subplots(3, 2, figsize=(14, 12))
    fig.suptitle(
        "Desempenho das Estruturas de Dados por Tamanho dos Dados", fontsize=16
    )

    # Subplot 1: Tempo de execução
    for i, structure in enumerate(structures):
        struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
        axs[0, 0].plot(
            struct_data["Data_Size"],
            struct_data["Execution_Time_ms"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=structure,
        )
    axs[0, 0].set_title("Tempo de Execução")
    axs[0, 0].set_xlabel("Tamanho dos Dados")
    axs[0, 0].set_ylabel("Tempo (ms)")
    axs[0, 0].legend()
    axs[0, 0].grid(True)

    # Subplot 2: Rotações (apenas para árvores)
    tree_structures = ["avl", "rb"]
    for i, structure in enumerate(tree_structures):
        if structure in structures:
            struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
            axs[0, 1].plot(
                struct_data["Data_Size"],
                struct_data["Rotations"],
                marker=markers[i % len(markers)],
                color=colors[i % len(colors)],
                label=structure,
            )
    axs[0, 1].set_title("Número de Rotações (Árvores)")
    axs[0, 1].set_xlabel("Tamanho dos Dados")
    axs[0, 1].set_ylabel("Rotações")
    axs[0, 1].legend()
    axs[0, 1].grid(True)

    # Subplot 3: Comparações de chave
    for i, structure in enumerate(structures):
        struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
        axs[1, 0].plot(
            struct_data["Data_Size"],
            struct_data["Key_Comparisons"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=structure,
        )
    axs[1, 0].set_title("Comparações de Chave")
    axs[1, 0].set_xlabel("Tamanho dos Dados")
    axs[1, 0].set_ylabel("Comparações")
    axs[1, 0].legend()
    axs[1, 0].grid(True)

    # Subplot 4: Profundidade de busca (usando Search_Depth que é igual a Key_Comparisons nos seus dados)
    for i, structure in enumerate(structures):
        struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
        axs[1, 1].plot(
            struct_data["Data_Size"],
            struct_data["Search_Depth"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=structure,
        )
    axs[1, 1].set_title("Profundidade de Busca")
    axs[1, 1].set_xlabel("Tamanho dos Dados")
    axs[1, 1].set_ylabel("Profundidade")
    axs[1, 1].legend()
    axs[1, 1].grid(True)

    # Subplot 5: Fixups de inserção (apenas para árvores)
    for i, structure in enumerate(tree_structures):
        if structure in structures:
            struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
            axs[2, 0].plot(
                struct_data["Data_Size"],
                struct_data["Insertion_Fixups"],
                marker=markers[i % len(markers)],
                color=colors[i % len(colors)],
                label=f"{structure} - Inserção",
            )
            # Deletion_Fixups são 0 nos seus dados, mas vamos incluir para completude
            axs[2, 0].plot(
                struct_data["Data_Size"],
                struct_data["Deletion_Fixups"],
                marker=markers[i % len(markers)],
                color=colors[i % len(colors)],
                linestyle="--",
                alpha=0.5,
                label=f"{structure} - Remoção",
            )
    axs[2, 0].set_title("Fixups (Árvores)")
    axs[2, 0].set_xlabel("Tamanho dos Dados")
    axs[2, 0].set_ylabel("Quantidade de Fixups")
    axs[2, 0].legend()
    axs[2, 0].grid(True)

    # Subplot 6: Mudanças de cor (apenas para árvore rubro-negra)
    if "rb" in structures:
        rb_data = df[df["Structure"] == "rb"].sort_values("Data_Size")
        axs[2, 1].plot(
            rb_data["Data_Size"],
            rb_data["Color_Changes"],
            marker="*",
            color="darkred",
            label="Red-Black Tree",
        )
    axs[2, 1].set_title("Mudanças de Cor (Red-Black)")
    axs[2, 1].set_xlabel("Tamanho dos Dados")
    axs[2, 1].set_ylabel("Color Changes")
    axs[2, 1].legend()
    axs[2, 1].grid(True)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    plt.savefig("performance_overview.png", dpi=300, bbox_inches="tight")
    print("✅ Gráfico salvo em performance_overview.png")

    # Print summary statistics
    print("\n" + "=" * 60)
    print("RESUMO ESTATÍSTICO")
    print("=" * 60)

    for structure in structures:
        struct_data = df[df["Structure"] == structure]
        print(f"\n{structure.upper()}:")
        print(f"  Tempo médio: {struct_data['Execution_Time_ms'].mean():.2f} ms")
        print(f"  Comparações médias: {struct_data['Key_Comparisons'].mean():.0f}")
        if structure in tree_structures:
            print(f"  Rotações médias: {struct_data['Rotations'].mean():.0f}")
            print(f"  Fixups médios: {struct_data['Insertion_Fixups'].mean():.0f}")
            if structure == "rb":
                print(
                    f"  Mudanças de cor médias: {struct_data['Color_Changes'].mean():.0f}"
                )


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python analyze_results.py <arquivo_csv>")
        sys.exit(1)
    plot_all_metrics(sys.argv[1])
