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
import numpy as np


def load_data():
    """Load the performance results from CSV."""
    try:
        df = pd.read_csv("freq_run_results.csv")
        return df
    except FileNotFoundError:
        print("Error: freq_run_results.csv not found. Please run the benchmark first.")
        return None


def analyze_tree_rotations(df):
    """Analyze rotation behavior of tree structures (avl, rb)."""
    print("\n" + "=" * 60)
    print("ROTATION ANALYSIS (Tree Structures Only)")
    print("=" * 60)

    tree_structures = ["avl", "rb"]
    tree_data = df[df["Structure"].isin(tree_structures)]

    if tree_data.empty:
        print("No tree structure data found.")
        return

    rotation_summary = (
        tree_data.groupby(["Structure"])["Rotations"]
        .agg(["mean", "sum", "count"])
        .round(2)
    )

    print("\nRotations Summary:")
    print(rotation_summary)

    avl_rotations = tree_data[tree_data["Structure"] == "avl"]["Rotations"].sum()
    rb_rotations = tree_data[tree_data["Structure"] == "rb"]["Rotations"].sum()

    print(f"\nTotal Rotations Comparison:")
    print(f"avl: {avl_rotations}")
    print(f"rb: {rb_rotations}")
    diff = avl_rotations - rb_rotations
    print(f"Difference: {abs(diff)} ({'avl' if diff > 0 else 'rb'} has more)")


def analyze_fixup_operations(df):
    """Analyze fixup operations for tree structures (avl, rb)."""
    print("\n" + "=" * 60)
    print("FIXUP OPERATIONS ANALYSIS (Tree Structures Only)")
    print("=" * 60)

    tree_structures = ["avl", "rb"]
    tree_data = df[df["Structure"].isin(tree_structures)]

    if tree_data.empty:
        print("No tree structure data found.")
        return

    print("\n=== INSERTION FIXUPS ===")
    insertion_fixups = (
        tree_data.groupby(["Structure"])["Insertion_Fixups"]
        .agg(["mean", "sum"])
        .round(2)
    )
    print(insertion_fixups)

    print("\n=== DELETION FIXUPS ===")
    deletion_fixups = (
        tree_data.groupby(["Structure"])["Deletion_Fixups"]
        .agg(["mean", "sum"])
        .round(2)
    )
    print(deletion_fixups)

    print("\n=== COLOR CHANGES (rb Only) ===")
    rb_data = tree_data[tree_data["Structure"] == "rb"]
    if not rb_data.empty:
        color_changes = rb_data["Color_Changes"].agg(["mean", "sum"]).round(2)
        print(color_changes)
    else:
        print("No rb tree data found.")

    print("\n=== FIXUP EFFICIENCY COMPARISON ===")
    for structure in tree_data["Structure"].unique():
        struct_data = tree_data[tree_data["Structure"] == structure]
        if not struct_data.empty:
            avg_insertion_fixups = struct_data["Insertion_Fixups"].mean()
            avg_rotations = struct_data["Rotations"].mean()
            print(f"{structure}:")
            print(f"  Average Insertion Fixups: {avg_insertion_fixups:.2f}")
            print(f"  Average Rotations: {avg_rotations:.2f}")
            if avg_insertion_fixups > 0:
                print(
                    f"  Fixup/Rotation Ratio: {avg_rotations/avg_insertion_fixups:.2f}"
                )


def analyze_key_comparisons(df):
    """Analyze key comparison patterns across all structures."""
    print("\n" + "=" * 60)
    print("KEY COMPARISON ANALYSIS")
    print("=" * 60)

    comparison_summary = (
        df.groupby(["Structure"])["Key_Comparisons"].agg(["mean", "sum"]).round(2)
    )

    print("\nKey Comparisons Summary:")
    print(comparison_summary)


def analyze_execution_time(df):
    """Analyze execution time patterns."""
    print("\n" + "=" * 60)
    print("EXECUTION TIME ANALYSIS")
    print("=" * 60)

    time_summary = (
        df.groupby(["Structure"])["Execution_Time_ms"]
        .agg(["mean", "min", "max"])
        .round(3)
    )

    print("\nExecution Time Summary (ms):")
    print(time_summary)

    print(f"\nPerformance by Data Size:")
    for size in sorted(df["Data_Size"].unique()):
        size_data = df[df["Data_Size"] == size]
        print(f"  Data Size {size}:")
        for structure in size_data["Structure"].unique():
            struct_data = size_data[size_data["Structure"] == structure]
            avg_time = struct_data["Execution_Time_ms"].mean()
            print(f"    {structure}: {avg_time:.3f} ms")


def performance_recommendations(df):
    """Provide performance recommendations based on analysis."""
    print("\n" + "=" * 60)
    print("PERFORMANCE RECOMMENDATIONS")
    print("=" * 60)

    best_time = df.loc[df["Execution_Time_ms"].idxmin()]
    best_comparisons = df.loc[df["Key_Comparisons"].idxmin()]

    print(
        f"\nBest for Speed: {best_time['Structure']} ({best_time['Execution_Time_ms']:.3f} ms)"
    )
    print(
        f"Best for Key Comparisons: {best_comparisons['Structure']} ({best_comparisons['Key_Comparisons']} comparisons)"
    )

    print("\nGeneral Insights:")
    print("• Hash tables (open, extern) have fastest insertions")
    print("• Open addressing has fewest key comparisons")
    print("• Tree structures (avl, rb) provide balanced performance")
    print("• AVL trees require more rotations but maintain stricter balance")
    print("• RedBlack trees require fewer rotations but allow more relaxed balance")


def create_visualizations(df):
    """Create performance visualizations with all available metrics."""
    try:
        fig, axes = plt.subplots(4, 2, figsize=(15, 22))

        tree_structures = ["avl", "rb"]
        tree_data = df[df["Structure"].isin(tree_structures)]

        # 1. Execution Time by Structure
        time_by_structure = (
            df.groupby("Structure")["Execution_Time_ms"].mean().sort_values()
        )
        time_by_structure.plot(kind="bar", ax=axes[0, 0], color="skyblue")
        axes[0, 0].set_title("Average Execution Time by Structure")
        axes[0, 0].set_ylabel("Time (ms)")
        axes[0, 0].tick_params(axis="x", rotation=45)

        # 2. Execution Time Scalability
        for structure in df["Structure"].unique():
            struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
            axes[0, 1].plot(
                struct_data["Data_Size"],
                struct_data["Execution_Time_ms"],
                marker="o",
                label=structure,
            )
        axes[0, 1].set_title("Execution Time Scalability")
        axes[0, 1].set_xlabel("Data Size")
        axes[0, 1].set_ylabel("Time (ms)")
        axes[0, 1].legend()
        axes[0, 1].grid(True)

        # 3. Key Comparisons by Data Size
        for structure in df["Structure"].unique():
            struct_data = df[df["Structure"] == structure].sort_values("Data_Size")
            axes[1, 0].plot(
                struct_data["Data_Size"],
                struct_data["Key_Comparisons"],
                marker="s",
                label=structure,
            )
        axes[1, 0].set_title("Key Comparisons by Data Size")
        axes[1, 0].set_xlabel("Data Size")
        axes[1, 0].set_ylabel("Key Comparisons")
        axes[1, 0].legend()
        axes[1, 0].grid(True)

        # 4. Rotations by Data Size (Tree structures only)
        if not tree_data.empty:
            for structure in tree_data["Structure"].unique():
                struct_data = tree_data[
                    tree_data["Structure"] == structure
                ].sort_values("Data_Size")
                axes[1, 1].plot(
                    struct_data["Data_Size"],
                    struct_data["Rotations"],
                    marker="^",
                    label=structure,
                )
            axes[1, 1].set_title("Rotations by Data Size (Tree Structures)")
            axes[1, 1].set_xlabel("Data Size")
            axes[1, 1].set_ylabel("Rotations")
            axes[1, 1].legend()
            axes[1, 1].grid(True)
        else:
            axes[1, 1].text(
                0.5,
                0.5,
                "No Tree Data",
                ha="center",
                va="center",
                transform=axes[1, 1].transAxes,
            )

        # 5. Total Key Comparisons by Structure
        total_comparisons = (
            df.groupby("Structure")["Key_Comparisons"].sum().sort_values()
        )
        total_comparisons.plot(kind="barh", ax=axes[2, 0], color="lightgreen")
        axes[2, 0].set_title("Total Key Comparisons by Structure")
        axes[2, 0].set_xlabel("Total Key Comparisons")

        # 6. Insertion Fixups by Data Size (Tree structures only)
        if not tree_data.empty:
            for structure in tree_data["Structure"].unique():
                struct_data = tree_data[
                    tree_data["Structure"] == structure
                ].sort_values("Data_Size")
                axes[2, 1].plot(
                    struct_data["Data_Size"],
                    struct_data["Insertion_Fixups"],
                    marker="d",
                    label=structure,
                )
            axes[2, 1].set_title("Insertion Fixups by Data Size")
            axes[2, 1].set_xlabel("Data Size")
            axes[2, 1].set_ylabel("Insertion Fixups")
            axes[2, 1].legend()
            axes[2, 1].grid(True)
        else:
            axes[2, 1].text(
                0.5,
                0.5,
                "No Tree Data",
                ha="center",
                va="center",
                transform=axes[2, 1].transAxes,
            )

        # 7. Color Changes by Data Size (rb only)
        rb_data = df[df["Structure"] == "rb"]
        if not rb_data.empty:
            rb_sorted = rb_data.sort_values("Data_Size")
            axes[3, 0].plot(
                rb_sorted["Data_Size"],
                rb_sorted["Color_Changes"],
                marker="o",
                color="red",
                label="rb",
            )
            axes[3, 0].set_title("Color Changes by Data Size (Red-Black Tree)")
            axes[3, 0].set_xlabel("Data Size")
            axes[3, 0].set_ylabel("Color Changes")
            axes[3, 0].grid(True)
        else:
            axes[3, 0].text(
                0.5,
                0.5,
                "No rb Data",
                ha="center",
                va="center",
                transform=axes[3, 0].transAxes,
            )

        # 8. Efficiency Comparison: Time vs Key Comparisons
        for structure in df["Structure"].unique():
            struct_data = df[df["Structure"] == structure]
            axes[3, 1].scatter(
                struct_data["Key_Comparisons"],
                struct_data["Execution_Time_ms"],
                label=structure,
                alpha=0.7,
                s=struct_data["Data_Size"] / 10000,
            )
        axes[3, 1].set_title("Execution Time vs Key Comparisons")
        axes[3, 1].set_xlabel("Key Comparisons")
        axes[3, 1].set_ylabel("Execution Time (ms)")
        axes[3, 1].legend()
        axes[3, 1].grid(True)

        plt.tight_layout()
        plt.savefig("performance_analysis.png", dpi=300, bbox_inches="tight")
        print(f"\n✅ Visualization saved as 'performance_analysis.png'")

    except ImportError:
        print("Matplotlib not available. Install with: pip install matplotlib")


def main():
    print("Advanced Data Structures Performance Analysis")
    print("=" * 50)

    df = load_data()
    if df is None:
        return

    print(f"\nLoaded {len(df)} performance measurements")
    print(f"Structures tested: {', '.join(df['Structure'].unique())}")
    print(f"Operations tested: {', '.join(df['Operation'].unique())}")
    print(f"Data sizes tested: {', '.join(map(str, sorted(df['Data_Size'].unique())))}")

    analyze_execution_time(df)
    analyze_tree_rotations(df)
    analyze_fixup_operations(df)
    analyze_key_comparisons(df)
    performance_recommendations(df)
    create_visualizations(df)

    print("\nAnalysis complete!")


if __name__ == "__main__":
    main()
