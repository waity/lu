def main():
  list_length = int(input('List Length: '))
  initial_list = [i for i in range(list_length)]

  ending_configuration = list(input("Ending configuration: "))
  ending_configuration = [int(k) for k in ending_configuration]
  
  sequence = list(input("Sequence: "))
  sequence = [int(k) for k in sequence]

  output = []

  for k in sequence:
    index = initial_list.index(k)
    output.append(str(ending_configuration[index]))

  print(''.join(output))

if __name__ == '__main__':
  main()