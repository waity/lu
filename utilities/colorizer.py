from tkinter import S
from colorama import init
init()

item_colors = ['\033[31m', '\033[32m', '\033[34m', '\033[35m', '\033[37m']

def main():
  with open('tmp/out') as out:
    for l in out:
      q = False
      if l.startswith('['):
        #
        #
        s = [int(a.strip()) for a in l.strip()[1:-1].split(',')]
        print('-', end='')
        for e in s:
          print(item_colors[e] + '\u2588', end='')

        print('\033[39m- ', end='')
        print(''.join([str(e) for e in s]), end='')
        print('\033[39m')
        #
        #
      elif not l.startswith('start') and len(l.strip()) != 0:
        print(l)
      elif not q:
        print()
        q = True

if __name__ == '__main__':
  main()