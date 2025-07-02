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
        df = pd.read_csv('performance_results.csv')
        return df
    except FileNotFoundError:
        print("Error: performance_results.csv not found. Please run the benchmark first.")
        return None

def analyze_tree_rotations(df):
    """Analyze rotation behavior of tree structures."""
    print("\n" + "="*60)
    print("ROTATION ANALYSIS (Tree Structures Only)")
    print("="*60)
    
    tree_data = df[df['Structure'].str.contains('Tree')]
    
    # Group by structure and operation
    rotation_summary = tree_data.groupby(['Structure', 'Operation'])['Rotations'].agg(['mean', 'sum', 'count']).round(2)
    
    print("\nRotations Summary:")
    print(rotation_summary)
    
    # AVL vs RedBlack rotation comparison
    avl_rotations = tree_data[tree_data['Structure'] == 'AVL Tree']['Rotations'].sum()
    rb_rotations = tree_data[tree_data['Structure'] == 'RedBlack Tree']['Rotations'].sum()
    
    print(f"\nTotal Rotations Comparison:")
    print(f"AVL Tree: {avl_rotations}")
    print(f"RedBlack Tree: {rb_rotations}")
    print(f"Difference: {abs(avl_rotations - rb_rotations)} ({'AVL' if avl_rotations > rb_rotations else 'RedBlack'} has more)")

def analyze_fixup_operations(df):
    """Analyze fixup operations for tree structures."""
    print("\n" + "="*60)
    print("FIXUP OPERATIONS ANALYSIS (Tree Structures Only)")
    print("="*60)
    
    tree_data = df[df['Structure'].str.contains('Tree')]
    
    # Insertion fixups analysis
    print("\n=== INSERTION FIXUPS ===")
    insertion_fixups = tree_data.groupby(['Structure', 'Operation'])['Insertion_Fixups'].agg(['mean', 'sum']).round(2)
    print(insertion_fixups)
    
    # Deletion fixups analysis
    print("\n=== DELETION FIXUPS ===")
    deletion_fixups = tree_data.groupby(['Structure', 'Operation'])['Deletion_Fixups'].agg(['mean', 'sum']).round(2)
    print(deletion_fixups)
    
    # Color changes analysis (RedBlack only)
    print("\n=== COLOR CHANGES (RedBlack Tree Only) ===")
    rb_data = tree_data[tree_data['Structure'] == 'RedBlack Tree']
    if not rb_data.empty:
        color_changes = rb_data.groupby('Operation')['Color_Changes'].agg(['mean', 'sum']).round(2)
        print(color_changes)
    else:
        print("No RedBlack tree data found.")
    
    # Fixup efficiency comparison
    print("\n=== FIXUP EFFICIENCY COMPARISON ===")
    for structure in tree_data['Structure'].unique():
        struct_data = tree_data[tree_data['Structure'] == structure]
        insert_data = struct_data[struct_data['Operation'] == 'Insert']
        if not insert_data.empty:
            avg_insertion_fixups = insert_data['Insertion_Fixups'].mean()
            avg_rotations = insert_data['Rotations'].mean()
            print(f"{structure}:")
            print(f"  Average Insertion Fixups: {avg_insertion_fixups:.2f}")
            print(f"  Average Rotations: {avg_rotations:.2f}")
            if avg_insertion_fixups > 0:
                print(f"  Fixup/Rotation Ratio: {avg_rotations/avg_insertion_fixups:.2f}")

def analyze_search_performance(df):
    """Analyze search performance metrics."""
    print("\n" + "="*60)
    print("SEARCH PERFORMANCE ANALYSIS")
    print("="*60)
    
    search_data = df[df['Operation'] == 'Search']
    
    if search_data.empty:
        print("No search data found.")
        return
    
    print("\n=== KEY COMPARISONS ===")
    comparison_summary = search_data.groupby('Structure')['Key_Comparisons'].agg(['mean', 'min', 'max']).round(2)
    print(comparison_summary)
    
    print("\n=== SEARCH DEPTH ===")
    depth_summary = search_data.groupby('Structure')['Search_Depth'].agg(['mean', 'min', 'max']).round(2)
    print(depth_summary)
    
    print("\n=== NODES VISITED ===")
    nodes_summary = search_data.groupby('Structure')['Nodes_Visited'].agg(['mean', 'min', 'max']).round(2)
    print(nodes_summary)
    
    print("\n=== SEARCH EFFICIENCY COMPARISON ===")
    for structure in search_data['Structure'].unique():
        struct_data = search_data[search_data['Structure'] == structure]
        avg_comparisons = struct_data['Key_Comparisons'].mean()
        avg_depth = struct_data['Search_Depth'].mean()
        avg_nodes = struct_data['Nodes_Visited'].mean()
        
        print(f"\n{structure}:")
        print(f"  Average Key Comparisons: {avg_comparisons:.1f}")
        print(f"  Average Search Depth: {avg_depth:.1f}")
        print(f"  Average Nodes Visited: {avg_nodes:.1f}")
        
        # Calculate efficiency ratios
        if avg_nodes > 0:
            comparison_efficiency = avg_comparisons / avg_nodes
            print(f"  Comparisons per Node: {comparison_efficiency:.2f}")

def analyze_key_comparisons(df):
    """Analyze key comparison patterns across all operations."""
    print("\n" + "="*60)
    print("KEY COMPARISON ANALYSIS (All Operations)")
    print("="*60)
    
    # Group by structure and operation
    comparison_summary = df.groupby(['Structure', 'Operation'])['Key_Comparisons'].agg(['mean', 'sum']).round(2)
    
    print("\nKey Comparisons Summary:")
    print(comparison_summary)

def analyze_execution_time(df):
    """Analyze execution time patterns."""
    print("\n" + "="*60)
    print("EXECUTION TIME ANALYSIS")
    print("="*60)
    
    # Group by structure and operation
    time_summary = df.groupby(['Structure', 'Operation'])['Execution_Time_ms'].agg(['mean', 'min', 'max']).round(3)
    
    print("\nExecution Time Summary (ms):")
    print(time_summary)
    
    # Insert operation comparison by data size
    insert_data = df[df['Operation'] == 'Insert']
    print(f"\nInsert Performance by Data Size:")
    for size in sorted(insert_data['Data_Size'].unique()):
        size_data = insert_data[insert_data['Data_Size'] == size]
        print(f"\nData Size: {size}")
        for structure in size_data['Structure'].unique():
            struct_data = size_data[size_data['Structure'] == structure]
            avg_time = struct_data['Execution_Time_ms'].mean()
            print(f"  {structure}: {avg_time:.3f} ms")

def performance_recommendations(df):
    """Provide performance recommendations based on analysis."""
    print("\n" + "="*60)
    print("PERFORMANCE RECOMMENDATIONS")
    print("="*60)
    
    # Best performers by operation
    operations = df['Operation'].unique()
    
    for op in operations:
        op_data = df[df['Operation'] == op]
        if op == 'Search':
            # For search, lower key comparisons is better
            best = op_data.loc[op_data['Key_Comparisons'].idxmin()]
            metric = f"{best['Key_Comparisons']} comparisons"
        else:
            # For insert/remove, lower execution time is better
            best = op_data.loc[op_data['Execution_Time_ms'].idxmin()]
            metric = f"{best['Execution_Time_ms']:.3f} ms"
        
        print(f"\nBest for {op}: {best['Structure']} ({metric})")
    
    # General recommendations
    print(f"\nGeneral Insights:")
    print(f"• Hash tables (OpenHashMap, ExternHashMap) have fastest insertions")
    print(f"• ExternHashMap has fewest key comparisons for searches")
    print(f"• Tree structures provide balanced performance with logarithmic guarantees")
    print(f"• AVL trees typically require more rotations but maintain stricter balance")
    print(f"• RedBlack trees require fewer rotations but allow more relaxed balance")

def create_visualizations(df):
    """Create performance visualizations with all metrics."""
    try:
        fig, axes = plt.subplots(3, 2, figsize=(15, 18))
        
        # 1. Execution Time by Structure and Operation
        execution_pivot = df.pivot_table(values='Execution_Time_ms', 
                                       index='Structure', 
                                       columns='Operation', 
                                       aggfunc='mean')
        execution_pivot.plot(kind='bar', ax=axes[0,0])
        axes[0,0].set_title('Average Execution Time by Structure and Operation')
        axes[0,0].set_ylabel('Time (ms)')
        axes[0,0].tick_params(axis='x', rotation=45)
        
        # 2. Rotations vs Fixups (Tree structures only)
        tree_data = df[df['Structure'].str.contains('Tree')]
        insert_trees = tree_data[tree_data['Operation'] == 'Insert']
        for structure in insert_trees['Structure'].unique():
            struct_data = insert_trees[insert_trees['Structure'] == structure]
            axes[0,1].scatter(struct_data['Insertion_Fixups'], struct_data['Rotations'], 
                            label=structure, alpha=0.7, s=struct_data['Data_Size']/10)
        axes[0,1].set_title('Rotations vs Insertion Fixups (Size = bubble size)')
        axes[0,1].set_xlabel('Insertion Fixups')
        axes[0,1].set_ylabel('Rotations')
        axes[0,1].legend()
        
        # 3. Search Depth by Data Size
        search_data = df[df['Operation'] == 'Search']
        depth_pivot = search_data.pivot_table(values='Search_Depth', 
                                             index='Data_Size', 
                                             columns='Structure', 
                                             aggfunc='mean')
        depth_pivot.plot(kind='line', ax=axes[1,0], marker='o')
        axes[1,0].set_title('Average Search Depth by Data Size')
        axes[1,0].set_ylabel('Search Depth')
        axes[1,0].set_xlabel('Data Size')
        
        # 4. Key Comparisons vs Nodes Visited
        for structure in search_data['Structure'].unique():
            struct_data = search_data[search_data['Structure'] == structure]
            axes[1,1].scatter(struct_data['Nodes_Visited'], struct_data['Key_Comparisons'], 
                            label=structure, alpha=0.7, s=struct_data['Data_Size']/10)
        axes[1,1].set_title('Key Comparisons vs Nodes Visited (Size = bubble size)')
        axes[1,1].set_xlabel('Nodes Visited')
        axes[1,1].set_ylabel('Key Comparisons')
        axes[1,1].legend()
        
        # 5. Color Changes (RedBlack Tree only)
        rb_data = df[df['Structure'] == 'RedBlack Tree']
        if not rb_data.empty:
            rb_pivot = rb_data.pivot_table(values='Color_Changes', 
                                         index='Data_Size', 
                                         columns='Operation', 
                                         aggfunc='sum')
            rb_pivot.plot(kind='bar', ax=axes[2,0])
            axes[2,0].set_title('RedBlack Tree: Color Changes by Operation')
            axes[2,0].set_ylabel('Color Changes')
            axes[2,0].set_xlabel('Data Size')
            axes[2,0].tick_params(axis='x', rotation=0)
        else:
            axes[2,0].text(0.5, 0.5, 'No RedBlack Tree Data', 
                          ha='center', va='center', transform=axes[2,0].transAxes)
            axes[2,0].set_title('RedBlack Tree: Color Changes')
        
        # 6. Scalability Analysis (All metrics)
        insert_data = df[df['Operation'] == 'Insert']
        scalability_pivot = insert_data.pivot_table(values='Execution_Time_ms', 
                                                  index='Data_Size', 
                                                  columns='Structure', 
                                                  aggfunc='mean')
        scalability_pivot.plot(kind='line', ax=axes[2,1], marker='^')
        axes[2,1].set_title('Insert Performance Scalability')
        axes[2,1].set_ylabel('Time (ms)')
        axes[2,1].set_xlabel('Data Size')
        
        plt.tight_layout()
        plt.savefig('performance_analysis.png', dpi=300, bbox_inches='tight')
        print(f"\nVisualization saved as 'performance_analysis.png'")
        
    except ImportError:
        print(f"\nMatplotlib not available. Skipping visualizations.")
        print(f"Install with: pip install matplotlib")

def main():
    """Main analysis function."""
    print("Advanced Data Structures Performance Analysis")
    print("=" * 50)
    
    df = load_data()
    if df is None:
        return
    
    print(f"\nLoaded {len(df)} performance measurements")
    print(f"Structures tested: {', '.join(df['Structure'].unique())}")
    print(f"Operations tested: {', '.join(df['Operation'].unique())}")
    print(f"Data sizes tested: {', '.join(map(str, sorted(df['Data_Size'].unique())))}")
    
    # Run analyses
    analyze_execution_time(df)
    analyze_tree_rotations(df)
    analyze_fixup_operations(df)
    analyze_search_performance(df)
    analyze_key_comparisons(df)
    performance_recommendations(df)
    
    # Create visualizations if possible
    create_visualizations(df)
    
    print(f"\nAnalysis complete!")

if __name__ == "__main__":
    main()
