import random
from itertools import islice
NUM_REGISTERS = 16

def take(n, iterable):
    "Return first n items of the iterable as a list"
    return list(islice(iterable, n))


offsets = []
permute_indexes = []

for i in range(65536):
    bin_str = format(i, "016b")
    bin_str_as_list = [x for x in bin_str][::-1]
    first_one_bit = 0
    to_shuffle = []
    first_one_bit = 0
    for i, bit in enumerate(bin_str_as_list):
        if bit == '0':
            random.shuffle(to_shuffle)
            permute_indexes.extend(to_shuffle)
            permute_indexes.append(i)
            to_shuffle = []
            first_one_bit = i
        else:
            to_shuffle.append(i)

    random.shuffle(to_shuffle)
    permute_indexes.extend(to_shuffle)

print("char shuffle_control_masks[16*65536] = {")
permute_indexes_iter = iter(permute_indexes)

while True:
    line = take(16, permute_indexes_iter)
    if len(line) == 0:
        break
    print("    " + ",".join([str(x) for x in line]) + ",")

print("};")

