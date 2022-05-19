def main():
  sequence = list(input("Sequence: "))
  sequence = [int(k) for k in sequence]

  ec = list(input("Ending configuration: "))
  ec = [int(k) for k in ec]

  initial_list = [i for i in range(max(ec))]

  output = []

  for k in sequence:
    index = initial_list.index(k)
    output.append(str(ec[index]))

  print(''.join(output))

if __name__ == '__main__':
  main()