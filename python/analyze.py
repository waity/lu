import subprocess
import sqlite3

db_file = 'lu.db'


def create_db():
  conn = sqlite3.connect(db_file)
  c = conn.cursor()
  f = open('python/schema.sql')
  c.executescript(f.read())


def create_connection():
  db = sqlite3.connect(db_file)
  return db


def fetch_one(qs, v):
  connection = create_connection()
  c = connection.cursor()
  c.execute(qs, v)
  r = c.fetchone()
  return r


def fetch_all(qs, v):
  connection = create_connection()
  c = connection.cursor()
  c.execute(qs, v)
  r = c.fetchall()
  return r


def insert(s, v):
  connection = create_connection()
  c = connection.cursor()
  c.execute(s, v)
  connection.commit()
  return c.lastrowid


def run(listLength, requestLength):
  first_row = True

  headers = []
  algorithms = []
  algorithm_id = {}

  with open('results.csv') as f:
    for line in f:
      values = line.strip().split(',')

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
        r = fetch_one('SELECT * FROM Sequence WHERE sequence=?', (values[headers.index('sequence')],))
        sequence_id = None
        if r == None:
          sequence_id = insert('INSERT INTO Sequence (listLength, requestLength, sequence) VALUES (?, ?, ?)',
            (listLength, requestLength, values[headers.index('sequence')]))
        else:
          sequence_id = r[0]

        for algorithm in algorithms:
          if fetch_one('SELECT * FROM Cost WHERE sequenceID = ? AND algorithmID = ?', (sequence_id, algorithm_id[algorithm])) == None:
            insert('INSERT INTO Cost (sequenceID, cost, algorithmID) VALUES (?, ?, ?)', (sequence_id, values[headers.index(algorithm)], algorithm_id[algorithm]))

if __name__ == '__main__':
  # create_db()
  for i in range(1, 5):
    for j in range(1, 5):
      subprocess.call(['./bin/run', '-list_length', str(i), '-request_length', str(j), '-num_trials', '10000', '-ALL'])
      run(i, j)
    