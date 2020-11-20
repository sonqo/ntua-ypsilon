import numpy
import subprocess
import pandas as pd
import seaborn as sns
from subprocess import PIPE
import matplotlib.pyplot as plt

# Default PHODS - Question 1
acc_v0 = []
subprocess.run('gcc -O0 phods.c -o phods.app', shell=True)
sum_default = 0
max_default = 0
min_default = 10000
for i in range(10):
    curr = subprocess.run('./phods.app', stdout=PIPE, shell=True)
    info = int(curr.stdout.decode('utf-8').split("'")[0])
    while info < 0:
        curr = subprocess.run('./phods.app', stdout=PIPE, shell=True)
        info = int(curr.stdout.decode('utf-8').split("'")[0])
    acc_v0.append(info)
    if info < min_default:
        min_default = info
    if info > max_default:
        max_default = info
    sum_default += info
average_default = sum_default/10
print(min_default, average_default, max_default)

# Loop Transformed PHODS - Question 2
acc_v1 = []
subprocess.run('gcc -O0 phods_opt_v1.c -o phods_opt_v1.app', shell=True)
sum_opt_v1 = 0
max_opt_v1 = 0
min_opt_v1 = 10000
for i in range(10):
    curr = subprocess.run('./phods_opt_v1.app', stdout=PIPE, shell=True)
    info = int(curr.stdout.decode('utf-8').split("'")[0])
    while info < 0:
        curr = subprocess.run('./phods_opt_v1.app', stdout=PIPE, shell=True)
        info = int(curr.stdout.decode('utf-8').split("'")[0])
    acc_v1.append(info)
    if info < min_opt_v1:
        min_opt_v1 = info
    if info > max_opt_v1:
        max_opt_v1 = info
    sum_opt_v1 += info
average_opt_v1 = sum_opt_v1/10
print(min_opt_v1, average_opt_v1, max_opt_v1)

# Space Exploration V1_PHODS - Question 3
specific_acc_v1 = []
subprocess.run('gcc -O0 phods_opt_v2.c -o phods_opt_v2.app', shell=True)
average_opt_v2 = 10000
for B in [1, 2, 4, 8, 16]:
    curr_acc_v2 = []
    curr_sum_opt_v2 = 0
    curr_max_opt_v2 = 0
    curr_min_opt_v2 = 10000
    for i in range(10):
        curr = subprocess.run('./phods_opt_v2.app ' + str(B), stdout=PIPE, shell=True)
        info = int(curr.stdout.decode('utf-8').split("'")[0])
        while info < 0:
            curr = subprocess.run('./phods_opt_v2.app ' + str(B), stdout=PIPE, shell=True)
            info = int(curr.stdout.decode('utf-8').split("'")[0])
        curr_acc_v2.append(info)
        if info < curr_min_opt_v2:
            curr_min_opt_v2 = info
        if info > curr_max_opt_v2:
            curr_max_opt_v2 = info
        curr_sum_opt_v2 += info
    curr_average_opt_v2 = curr_sum_opt_v2/10
    specific_acc_v1.append(curr_acc_v2)
    if curr_average_opt_v2 < average_opt_v2:
        best_B = B
        acc_v2 = curr_acc_v2
        min_opt_v2 = curr_min_opt_v2
        max_opt_v2 = curr_max_opt_v2
        average_opt_v2 = curr_average_opt_v2
print(best_B, min_opt_v2, average_opt_v2, max_opt_v2)

# Plotting different B values
final_v1 = numpy.swapaxes(numpy.asarray(specific_acc_v1), 0, 1)
B_Values = pd.DataFrame(data=final_v1, columns=['B=1', 'B=2', 'B=4', 'B=8', 'B=16'])
ax = sns.boxplot(data=B_Values, showfliers=False).set(xlabel='Block Values', ylabel='Execution Time')
plt.show()

# Space Exploration V2_PHODS - Question 4
specific_acc_v2 = []
subprocess.run('gcc -O0 phods_opt_v3.c -o phods_opt_v3.app', shell=True)
average_opt_v3 = 10000
for Bx in [1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 36, 48, 72]:
    for By in [1, 2, 4, 8, 11, 16, 22, 44, 88]:
        curr_acc_v3 = []
        curr_sum_opt_v3 = 0
        curr_max_opt_v3 = 0
        curr_min_opt_v3 = 10000
        for i in range(10):
            curr = subprocess.run('./phods_opt_v3.app ' + str(Bx) + ' ' + str(By), stdout=PIPE, shell=True)
            info = int(curr.stdout.decode('utf-8').split("'")[0])
            while info < 0:
                curr = subprocess.run('./phods_opt_v3.app ' + str(Bx) + ' ' + str(By), stdout=PIPE, shell=True)
                info = int(curr.stdout.decode('utf-8').split("'")[0])
            curr_acc_v3.append(info)
            if info < curr_min_opt_v3:
                curr_min_opt_v3 = info
            if info > curr_max_opt_v3:
                curr_max_opt_v3 = info
            curr_sum_opt_v3 += info
        curr_average_opt_v3 = curr_sum_opt_v3/10
        specific_acc_v2.append(curr_acc_v3)
        if curr_average_opt_v3 < average_opt_v3:
            best_Bx = Bx
            best_By = By
            acc_v3 = curr_acc_v3
            min_opt_v3 = curr_min_opt_v3
            max_opt_v3 = curr_max_opt_v3
            average_opt_v3 = curr_average_opt_v3
print(best_Bx, best_By, min_opt_v3, average_opt_v3, max_opt_v3)

# Plotting different Bx-By pairs
final_v2 = numpy.swapaxes(numpy.asarray(specific_acc_v2), 0, 1)
BPair_Values = pd.DataFrame(data=final_v2)
fig, ax = plt.subplots()
g = sns.boxplot(data=BPair_Values, showfliers=False, ax=ax)
g.set(xlabel='B-Pair Values', ylabel='Execution Time')
g.tick_params(bottom=False)
g.set(xticklabels=[])
plt.show()

# Plotting - Question 6
final = numpy.stack([acc_v0, acc_v1, acc_v2, acc_v3], axis=1)
df = pd.DataFrame(data=final, columns=['Default', 'Optimized', 'Best B', 'Best Bx-By'])
ax = sns.boxplot(data=df, showfliers=False).set(xlabel='Sub-Questions', ylabel='Execution Time')
plt.show()