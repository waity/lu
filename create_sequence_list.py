import json

def main():
  with open('top.json', 'r') as f:
    contents = f.read()
    j = json.loads(contents)
    for sequence in j:
      print(sequence['sequence'])

if __name__ == '__main__':
  main()