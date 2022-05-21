from tkinter import S
from colorama import init
init()

item_colors = ['\033[31m', '\033[32m', '\033[34m', '\033[35m', '\033[37m', '\033[33m', '\033[36m', '\033[38m', '\033[39m']

def main():
  with open('tmp/out') as out:
    sequence = []
    # request index
    ri = -1
    prev = None
    for l in out:
      if l.startswith('"'):
        sequence = [int(i) for i in l.strip()[1:-1].split(',')]
      elif l.startswith('['):
        s = [int(a.strip()) for a in l.strip()[1:-1].split(',')]
        print('-', end='')
        for e in s:
          print(item_colors[e] + '\u2588', end='')

        if ri >= 0:
          rv = sequence[ri]
          fei = None
          old = ' '
          diff = ['_' for i in range(len(s))]
          if s.index(rv) < prev.index(rv):
            fei = s.index(rv)
            diff[fei] = 'F'
            old = prev.index(rv)
            prev.remove(rv)
            prev.insert(s.index(rv), rv)
          for e in range(max(sequence)):
            if prev.index(e) < s.index(e):
              diff[s.index(e)] = 'P'
          # diff = [s for s in prev]
          print('\033[39m- ', end='')
          print(''.join([str(e) for e in diff]) + ' ' + str(rv) + ' [' + str(old), end=']')
          print('\033[39m')
        else:
          print()
        prev = s
        ri += 1
      elif not l.startswith('start') and len(l.strip()) != 0:
        print(l)
      elif ri > -1:
        ri = -1
        prev = None
        print()

if __name__ == '__main__':
  main()