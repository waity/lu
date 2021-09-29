import subprocess
import sqlite3
import csv
from tqdm import tqdm
import mysql.connector

db_file = 'lu-complete-fixed.db'

connection = sqlite3.connect(db_file)
c = connection.cursor()

if __name__ == '__main__':
  a = c.execute('select * from sequence where sequence = ? and listlength = ? and requestlength = ?', ("0,2,4,0,2,0,0,2,4", 5, 10))
  for i in a:
    print(i)

  a = c.execute('select * from cost where sequenceID = ?', ('11719238',))
  for i in a:
    print(i)

  # a = c.execute('select * from algorithm')
  # for i in a:
  #   print(i)