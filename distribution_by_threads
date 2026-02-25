import numpy as np
import matplotlib.pyplot as plt

def distribution(n, processors):
    total = 2 ** n
    block = total // processors

    # dist[p][k]
    dist = np.zeros((processors, n + 1), dtype=int)

    for idx in range(total):
        k = bin(idx).count("1")
        p = min(idx // block, processors - 1)
        dist[p, k] += 1

    return dist

def plot_area(n, processors):
    dist = distribution(n, processors)
    ks = np.arange(n + 1)
    plt.figure(figsize=(10, 6))
    colors = plt.cm.Oranges(np.linspace(0, 1, processors))
    plt.stackplot(ks, dist, colors = colors, labels = [f"Процессор {i+1}" for i in range(processors)], edgecolor = "black")

    plt.xlabel("Число единичных координат k")
    plt.ylabel("Количество векторов")
    plt.title("Распределение векторов укладки по процессорам")
    plt.legend(loc = "upper left")
    plt.grid(True)
    plt.tight_layout()
    plt.show()

n = 15
plot_area(n, processors = 2)
plot_area(n, processors = 4)
plot_area(n, processors = 8)
plot_area(n, processors = 12)
plot_area(n, processors = 16)
