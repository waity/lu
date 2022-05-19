from colorama import init
init()

colors = ['\033[31m', '\033[32m', '\033[34m', '\033[35m', '\033[37m']

def main():
  with open('tmp/out') as out:
    q = False
    for l in out:
      if l.startswith('['):
        if q is True:
          q = False
        #
        #
        state = [int(a.strip()) for a in l.strip()[1:-1].split(',')]
        print('-', end='')
        for e in state:
          print(colors[e] + '\u2588', end='')
        print('\033[39m-')
        #
        #
      elif not l.startswith('start') and len(l.strip()) != 0:
        print(l)
      elif not q:
        print()
        q = True

if __name__ == '__main__':
  main()