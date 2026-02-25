import pandas as pd
import matplotlib.pyplot as plt
import chardet

with open("data.csv", 'rb') as f:
    raw_data = f.read()
    encoding = chardet.detect(raw_data)['encoding']

df = pd.read_csv("data.csv", encoding = encoding)
df = df.dropna()
print(df)

# график
plt.figure(figsize = (10, 6))
plt.plot(df['k'], df['if_ns'], 'o-', label = 'IF-ADD', color = 'blue', linewidth = 2, markersize = 8)
plt.plot(df['k'], df['mul_ns'], 's-', label = 'MUL-ADD', color = 'red', linewidth = 2, markersize = 8)
plt.xlabel('Количество единичных элементов k', fontsize = 12)
plt.ylabel('Время (нс)', fontsize = 12)
plt.title('', fontsize = 14, fontweight = 'bold')
plt.legend(fontsize = 11)
plt.grid(True, alpha = 0.3)
plt.ylim(bottom = 0, top = 100)

plt.tight_layout()
plt.savefig('simple_plot.png', dpi = 150)
plt.show()
