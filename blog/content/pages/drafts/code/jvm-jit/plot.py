#%%

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def parse_csv(fname):
    df = pd.read_csv(fname, header=None, names = ('run_id', 'time'))
    df.run_id = pd.to_numeric(df.run_id.str.replace('#', ''))
    return df

    


#%%
DIR = r'/home/isaiahp/projects/jvmJIT/'
df = parse_csv(DIR + 'jit.times.csv')

df_aot = parse_csv(DIR + 'aot.times.csv')
df_aot_tiered = parse_csv(DIR + 'aot.tiered.csv')

ax = plt.subplot(facecolor='gray')

plt.yscale('log')
plt.ylabel('time (nanos)')
plt.xlabel('run')
plt.yticks([100, 200, 400, 100_000, 1000_000, 1000_000_0, 100_000_000])
plt.ylim((min(df.time), max(df_aot.time)))
plt.plot(df.run_id, df.time, color='yellow', lw=1, label='JIT-Compiled')
#plt.plot(df_aot.run_id, df_aot.time, color='red', lw=1, label='AOT-Compiled')

ax.annotate('Interpreter', xy=(3, 667920), xytext=(5, 1000_000),
            arrowprops=dict(arrowstyle='->', facecolor='black'))


plt.annotate("", xy=(13, 113000), xycoords='data',
             xytext=(39, 113000), textcoords='data',
             arrowprops=dict(arrowstyle="<->", connectionstyle="arc3"))
plt.text(27, 1000, r'C1 JIT',
         {'color': 'k', 'fontsize': 12, 'ha': 'center', 'va': 'center',
          'bbox': dict(boxstyle="round", fc="w", ec="k", pad=0.2)})


plt.annotate("", xy=(40, 1000), xycoords='data',
             xytext=(99, 1000), textcoords='data',
             arrowprops=dict(arrowstyle="<->", connectionstyle="arc3"))
plt.text(65, 1000, r'C2 JIT',
         {'color': 'k', 'fontsize': 12, 'ha': 'center', 'va': 'center',
          'bbox': dict(boxstyle="round", fc="w", ec="k", pad=0.2)})

ax.annotate('Deoptimized', xy=(105, 924303), xytext=(100, 10_000_000),
            arrowprops=dict(arrowstyle='->', facecolor='black'))

#plt.legend()
IMG_OUT_DIR = r'/home/isaiahp/projects/isaiah-perumalla.github.io/blog/content/imgs/jvm-jit/'
plt.savefig(IMG_OUT_DIR +'time.svg')
plt.show()


