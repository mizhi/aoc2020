import itertools
import functools
import operator

NUMBER_OF_EXPENSES = 2

with open("input1.txt", "r") as input_file:
    expenses = [int(line.strip()) for line in input_file.readlines()]

# This code makes an assumption that there will be only one combination
# of expenses that sums to 2020. Since the itertools functions return
# iterators, we can't simply index into the single element result, so
# we use the next function to return it.
valid_pair = next(itertools.
                  filterfalse(
                      lambda expenses: sum(expenses) != 2020,
                      itertools.combinations(expenses, NUMBER_OF_EXPENSES)
                  ))

# Functools is so very very useful.
print(functools.reduce(operator.mul, valid_pair, 1))
