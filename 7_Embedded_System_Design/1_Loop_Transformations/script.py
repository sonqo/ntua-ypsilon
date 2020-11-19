import subprocess
from subprocess import PIPE

# Default PHODS
subprocess.run('gcc -O0 phods.c -o phods', shell=True)
sum_default = 0
max_default = 0
min_default = 10000
for i in range(10):
    curr = subprocess.run('./phods', stdout=PIPE)
    info = int(curr.stdout.decode('utf-8').split("'")[0])
    if info < min_default:
        min_default = info
    if info > max_default:
        max_default = info
    sum_default += info
average_default = sum_default/10
print(min_default, average_default, max_default)

# Loop Transformed PHODS
subprocess.run('gcc -O0 phods_opt_v1.c -o phods_opt_v1', shell=True)
sum_opt_v1 = 0
max_opt_v1 = 0
min_opt_v1 = 10000
for i in range(10):
    curr = subprocess.run('./phods_opt_v1', stdout=PIPE)
    info = int(curr.stdout.decode('utf-8').split("'")[0])
    if info < min_opt_v1:
        min_opt_v1 = info
    if info > max_opt_v1:
        max_opt_v1 = info
    sum_opt_v1 += info
average_opt_v1 = sum_opt_v1/10
print(min_opt_v1, average_opt_v1, max_opt_v1)

# Space Exploration PHODS
acc = [1, 2, 4, 8, 16]
subprocess.run('gcc -O0 phods_opt_v2.c -o phods_opt_v2', shell=True)
average_opt_v2 = 10000
# for B in acc:
curr_sum_opt_v2 = 0
curr_max_opt_v2 = 0
curr_min_opt_v2 = 10000
for i in range(10):
    curr = subprocess.run('./phods_opt_v2 ' + str(1), stdout=PIPE, shell=True)
    print(curr)
    # info = int(curr.stdout.decode('utf-8').split("'")[0])
#     if info < curr_min_opt_v2:
#         curr_min_opt_v2 = info
#     if info > curr_max_opt_v2:
#         curr_max_opt_v2 = info
#     curr_sum_opt_v2 += info
# curr_average_opt_v2 = curr_sum_opt_v2/10
# if curr_average_opt_v2 < average_opt_v2:
#     best_B = B
#     min_opt_v2 = curr_min_opt_v2
#     max_opt_v2 = curr_max_opt_v2
#     average_opt_v2 = curr_average_opt_v2
# print(best_B, min_opt_v2, average_opt_v2, max_opt_v2)