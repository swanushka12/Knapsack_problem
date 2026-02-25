import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("reg.csv")

plt.figure(figsize=(10,7))

for bits in sorted(df["element_bits"].unique()):
    sub = df[df["element_bits"] == bits].sort_values("k")
    plt.plot(sub["k"], sub["if_ns"], marker='o', linewidth=2, label=f"{bits} бит")

plt.xlabel("Число единичных координат k")
plt.ylabel("Время (нс)")
plt.grid(True)
plt.legend()
plt.show()
