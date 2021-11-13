import subprocess
import sqlite3
import csv
from tqdm import tqdm
import mysql.connector

db_file = 'bijective.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

if __name__ == '__main__':
  a = c.execute('select * from sequence limit 20 offset 16590')
  for i in a:
    print(i)
  # a = c.execute('select * from algorithm')
  # for i in a:
  #   print(i)