import subprocess
import sqlite3
import csv
from tqdm import tqdm
import mysql.connector

db_file = 'nabbed.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

if __name__ == '__main__':
  a = c.execute('select COUNT(*) from sequence where listlength = ? and requestlength = ?', (5, 4))
  for i in a:
    print(i)

  # a = c.execute('select * from cost where sequenceID = ?', ('8666941',))
  # for i in a:
  #   print(i)

  # a = c.execute('select * from algorithm')
  # for i in a:
  #   print(i)