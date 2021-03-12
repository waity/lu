import subprocess
import sqlite3
import csv
from tqdm.auto import trange
import mysql.connector

db_file = 'lu-complete.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

def get_sequences(algorithm, algorithm_vs, list_length, request_length):
  a_params = []
  b_params = []
  a = "SELECT * FROM COST JOIN SEQUENCE ON Sequence.sequenceID = Cost.sequenceID JOIN ALGORITHM ON COST.algorithmid = Algorithm.algorithmid WHERE COST.algorithmid = ? "
  a_params.append(algorithm)
  b = "SELECT * FROM COST JOIN SEQUENCE ON Sequence.sequenceID = Cost.sequenceID JOIN ALGORITHM ON COST.algorithmid = Algorithm.algorithmid WHERE COST.algorithmid = ? "
  b_params.append(algorithm_vs)
  c = "SELECT A.sequenceID, A.sequence, ((A.cost * (1.0)) / B.cost)  AS ratio, A.cost AS c1, B.cost AS c2 FROM B JOIN A ON B.sequenceid = A.sequenceid GROUP BY A.sequenceid ORDER BY ratio DESC LIMIT 100"
  if list_length != 'ALL':
    a += " AND listlength = ? "
    b += " AND listlength = ? "
    a_params.append(list_length)
    b_params.append(list_length)

  if request_length != 'ALL':
    a += " AND requestlength = ? "
    b += " AND requestlength = ? "
    a_params.append(request_length)
    b_params.append(request_length)

  query = "WITH A AS (" + a + "), " + "B AS (" + b + ")" + c

  return query, tuple(a_params + b_params)

def get_costs(sequence_id):
  return ("SELECT * FROM Cost WHERE sequenceID = ?", (sequence_id,))

if __name__ == '__main__':
  # get algs and alg numbers
  algorithms = {}
  r = c.execute("select * from algorithm")
  for v in r:
    algorithms[v[0]] = v[1]

  sequences = []
  costs = {}

  ll = input('list length: ')
  rl = input('request length: ')

  # start

  # query, params = get_sequences(1, 2, 5, 10)
  # rv = c.execute(query, params)
  # for r in rv:
  #   sequences.append((r[0], r[1]))

  # for s in sequences:
  #   query, params = get_costs(s[0])
  #   rv = c.execute(query, params)
  #   for r in rv:
  #     if r[0] not in costs:
  #       costs[r[0]] = {}
  #     costs[r[0]][r[2]] = r[1]

  # print("sequences," + ",".join(algorithms.values()))
  # for s in sequences:
  #   print("\"" + s[1] + "\"," + ','.join([str(c) for c in costs[s[0]].values()]))
  # # end
  # exit(0)
  
  for j in trange(1, 6):
    for k in range(1, 6):
      query, params = get_sequences(j, k, ll, rl)
      rv = c.execute(query, params)
      for r in rv:
        sequences.append((r[0], r[1]))

      for s in sequences:
        query, params = get_costs(s[0])
        rv = c.execute(query, params)
        for r in rv:
          if r[0] not in costs:
            costs[r[0]] = {}
          costs[r[0]][r[2]] = r[1]


  f = open('important_rl_' + str(rl) + '_ll_' + str(ll) + '.csv', 'w')
  sequences = list(set(sequences))
  f.write("sequence," + ",".join(algorithms.values()) + "\n")
  for s in sequences:
    f.write("\"" + s[1] + "\"," + ','.join([str(c) for c in costs[s[0]].values()]) + "\n")
  
  f.close()