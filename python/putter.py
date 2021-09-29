import subprocess
import sqlite3
import csv
from tqdm import tqdm
import mysql.connector

db_file = 'lu-complete-fixed.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

def insert(s, v):
  c.execute(s, v)
  return c.lastrowid

if __name__ == '__main__':
  f = open('results/results.csv')
  first = True
  for line in tqdm(csv.reader(f, delimiter=",")):
    if first:
      first = False
      continue
    values = list(line)
    a = c.execute('select * from sequence where sequence = ? and listLength = ? and requestLength = ?', (values[0], 5, 10))
    row = tuple(c.fetchone())
    # c.execute('delete from Cost where sequenceID = ?  and algorithmID = ?', (row[0], 5))
    sequence_id = insert('INSERT INTO Cost (sequenceID, cost, algorithmID) VALUES (?, ?, ?)',
        (row[0], values[1], 5))
  
  connection.commit()