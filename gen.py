import numpy as np
import networkx as nx

V = 5 # number of nodes
D = 2 # dimensionality

positions = np.random.randint(40, size=(V, V))
differences = positions[:, None, :] - positions[None, :, :]
distances = np.round(np.sqrt(np.sum(differences**2, axis=-1))) # euclidean

# create a weighted, directed graph in networkx
graph = nx.from_numpy_matrix(distances, create_using=nx.DiGraph())

A = nx.adjacency_matrix(graph)
print(A.todense())