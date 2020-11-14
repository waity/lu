if __name__ == '__main__':
  nums = [0, 1]
  f = open('input_4321', 'w')
  for i in range(1, 41):
    for j in range(0, i):
      f.write(str(nums[(len(nums) - 1 - j) % len(nums)]))
    f.write("\n")