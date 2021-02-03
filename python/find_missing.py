import subprocess
import sqlite3
import csv
from tqdm.auto import trange
import mysql.connector

def areEqual(arr1, arr2, n, m):
 
    # If lengths of array are not
    # equal means array are not equal
    if (n != m):
        return False
    b1 = arr1[0]
    b2 = arr2[0]
     
    # find xor of all elements
    for i in range(1, n - 1):
        b1 ^= arr1[i]
 
    for i in range(1, m - 1):
        b2 ^= arr2[i]
 
    all_xor = b1 ^ b2
     
    # If all elements were same then xor will be zero
    if(all_xor == 0):
        return True
 
    return False
 

def get_next(sequence,  ll):
  index = len(sequence)- 1
  for i in range(len(sequence) - 1, -1, -1):
    sequence[i] += 1
    if sequence[i] == ll:
      sequence[i] = 0
    else:
      break
  
  return sequence

if __name__ == '__main__':
  ll = 5
  rl = 10
  test = [0 for i in range(rl)]
  test[len(test) - 1] = -1
  this_file = False
  line = 1
  for i in range(0, 10):
    with open('results/results_' + str(i) + '.csv', 'r') as f:
      line = 1
      this_file = False
      first = True
      for l in csv.reader(f, delimiter=","):
        if first:
          first = False
          continue
        l[0] = [int(n) for n in l[0].split(',')]
        if not areEqual(l[0], get_next(test, ll), 10, 10):
          if not this_file:
            print(l[0])
            print(test)
            print(i, 'line ' + str(line))
            print('results/results_' + str(i) + '.csv')
            this_file = True
        line += 1