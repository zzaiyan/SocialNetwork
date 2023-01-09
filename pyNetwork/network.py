# -*- coding:utf-8 -*-

import matplotlib.pyplot as plt
import random
import networkx
import networkx as nx
import igraph as motif
import self as self
from matplotlib import collections
from matplotlib.lines import Line2D
from networkx.algorithms import community
import numpy
from networkx.drawing.nx_agraph import graphviz_layout
import pandas as pd
import itertools

plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号
plt.rcParams['figure.figsize'] = (8, 8)

G = nx.read_weighted_edgelist('../SocialNetwork/in.csv', delimiter=',',
                              create_using=nx.MultiGraph(),encoding='gbk')

# 度中心度
degree_centrality = nx.centrality.degree_centrality(G)
# 8个度中心性最高的节点及其度中心性（重要人物分析）
Person_cen = sorted(degree_centrality.items(), key=lambda item: item[1], reverse=True)[:8]
Person_cen = [Person_cen[0][0], Person_cen[1][0], Person_cen[2][0], Person_cen[3][0], Person_cen[4][0],
              Person_cen[5][0], Person_cen[6][0], Person_cen[7][0]]

# 影响力分析


# 边的分类
efather = [(u, v) for (u, v, d) in G.edges(data=True) if d['weight'] == 1]  # 子女
ewife = [(u, v) for (u, v, d) in G.edges(data=True) if d['weight'] == 2]  # 妻子
eserva = [(u, v) for (u, v, d) in G.edges(data=True) if d['weight'] == 3]  # 丫鬟
econcu = [(u, v) for (u, v, d) in G.edges(data=True) if d['weight'] == 4]  # 妾室


def team():
    # 读取txt文件
    # with open("C:/Users/1/Desktop/第二题/sign.txt", encoding='utf-8') as file:
    #     content = file.read()
    #     print(content.rstrip())  ##rstrip()删除字符串末尾的空行

    # content = "BN算法"
    k = 0

    # 划分社区（小团体）
    if k != 0:
        # BN算法
        comp = community.girvan_newman(G)
        # k = 13  # 想要13个社区
        limited = itertools.takewhile(lambda c: len(c) <= k, comp)  # 层次感迭代器
        for communities in limited:
            b = list(sorted(c) for c in communities)
        return b
    else:
        # louvain算法
        result = community.louvain_communities(G)
        return result


# 结构相似度分析
# 生成诱导子图
# num = 0   # 计数
# target = nx.MultiGraph()
# target.add_edge('贾代化','贾敬')
# target.add_edge('贾敬','贾珍')
# target.add_edge('贾敬','贾珍')
# target.add_edge('贾代化','贾敷')
# target.add_edge('贾演','贾代化')
# target.add_edge('贾珍','贾蓉')
# target.add_edge('贾珍','贾蔷')
# target.add_edge('贾珍','尤氏')
#
# for sub_nodes_pre in itertools.combinations(G.nodes(),len(target.nodes())):
#     subg_pre = G.subgraph(sub_nodes_pre)
#     i = set(subg_pre).intersection(target)
#     res = round(len(i) / (len(target) + len(subg_pre) - len(i)), 3)   # Jaccard distance
#     if nx.is_connected(subg_pre) and nx.is_isomorphic(target, subg_pre):
#         print(res)
#         print(target.edges())
#         print(subg_pre.edges())
#     else:
#         print(num)
#         num += 1
#
# 种子
seed = int(random.random()*3000)
# seed = 2426
pos = nx.spring_layout(G, seed=seed, scale=2)
#
# 获取节点的度数
nx.draw(G, pos, edgelist=[],node_size=150, node_color='lightblue', style='solid')
nx.draw(G, pos, nodelist=Person_cen, edgelist=[], node_size=110, node_color='red', style='solid')
nx.draw(G, pos, nodelist=[], edgelist=efather, width=1, alpha=0.8, edge_color='b', style='solid')
nx.draw(G, pos, nodelist=[], edgelist=ewife, width=1, alpha=0.8, edge_color='g', style='solid')
nx.draw(G, pos, nodelist=[], edgelist=eserva, width=1, alpha=0.8, edge_color='y', style='solid')
nx.draw(G, pos, nodelist=[], edgelist=econcu, width=1, alpha=0.8, edge_color='c', style='solid')
# 画结点
Person = team()
color = ['lightblue', 'lightpink', 'lightyellow', 'lightgreen', 'Cyan', 'LavenderBlush', 'Lavender', 'DarkViolet',
         'SeaGreen',
         'SlateGray', 'Lime', 'Gold', 'Orange']
for i in range(len(Person)):
    print(i)
    nx.draw(G, pos, nodelist=Person[i], edgelist=[], node_size=120, node_color=color[i])
# 标签
nx.draw_networkx_labels(G, pos, font_size=8, font_family='sans-serif')

# # 增加图例
# legend_elements = [
#     Line2D([0], [0], marker='o', color='red', label='重要人物', lw=0,
#            markerfacecolor='white', markersize=10),
#     # Line2D([], [], color='b', label='子女'),
#     # Line2D([], [], color='g', label='夫妻'),
#     # Line2D([], [], color='y', label='主仆'),
#     # Line2D([], [], color='c', label='小妾'),
# ]
# plt.legend(handles=legend_elements, loc='lower left')

print('seed =', seed)

# 显示图形
plt.show()
