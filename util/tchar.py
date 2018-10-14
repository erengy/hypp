for c in range(0, 128):
  if c % 8 == 0:
    print('\n', end='')
  if chr(c).isalnum() or chr(c) in "!#$%&'*+-.^_`|~":
    print(" '{}', ".format(chr(c)), end='')
  else:
    print('   0, ', end='')
