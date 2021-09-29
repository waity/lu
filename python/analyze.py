import subprocess
import sqlite3
import csv
from tqdm import tqdm
import mysql.connector

db_file = 'nabbed-fixed.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

def create_db():
  conn = sqlite3.connect('nabbed-fixed.db')
  c = conn.cursor()
  f = open('python/schema.sql')
  c.executescript(f.read())


def fetch_one(qs, v):
  c.execute(qs, v)
  r = c.fetchone()
  return r


def fetch_all(qs, v):
  c.execute(qs, v)
  r = c.fetchall()
  return r


def insert(s, v):
  c.execute(s, v)
  return c.lastrowid

def insert_many(s, v):
  c.executemany(s, v)
  return c.lastrowid


def run(i, listLength, requestLength, filename=False):
  first_row = True

  headers = []
  algorithms = []
  algorithm_id = {}

  f = None

  if filename:
    f = open(filename)
  else:
    f = open('results/results_' + str(i) + '.csv')

  for line in csv.reader(f, delimiter=","):
    values = list(line)

    # if first row, grab algorithms, insert
    if first_row:
      headers = values

      # insert algorithms
      for header in headers:
        if header != 'sequence':
          algorithms.append(header)
          r = fetch_one('SELECT * FROM Algorithm WHERE name=?', (header,))
          if r == None:
            alg_id = insert('INSERT INTO Algorithm (name) VALUES (?)', (header,))
            algorithm_id[header] = alg_id
          else:
            algorithm_id[header] = r[0]
      first_row = False

    else:
      sequence_id = None
      # Because sequential, this is much faster than looking up if sequence exists already.
      try:
        sequence_id = insert('INSERT INTO Sequence (listLength, requestLength, sequence) VALUES (?, ?, ?)',
          (listLength, requestLength, values[headers.index('sequence')]))
      except (sqlite3.IntegrityError):
        return
      
      if fetch_one('SELECT * FROM Cost WHERE sequenceID = ? LIMIT 1', (sequence_id,)) == None:
        to_insert = []
        for algorithm in algorithms:
          to_insert.append((sequence_id, values[headers.index(algorithm)], algorithm_id[algorithm]))
        insert_many('INSERT INTO Cost (sequenceID, cost, algorithmID) VALUES (?, ?, ?)', to_insert)
  f.close()

def run_one(listLength, requestLength, filename):
  run(1, listLength, requestLength, filename)

if __name__ == '__main__':
  # create_db()

  c = connection.cursor()

  ll = input('list length: ')
  rl = input('request length: ')

  
  run_one(ll, rl, "important_rl_" + rl + "_ll_" + ll + ".csv")
  connection.commit()

  # for i in tqdm(range(0, 1)):
    # run(i, listLength=ll, requestLength=rl)
    # connection.commit()