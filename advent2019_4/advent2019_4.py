
c = 0
for i in range(372304, 847061):
  x = i
  digits = []
  ld = 10
  hasdouble = False
  repct = 0
  decreasing = True
  while x > 0:
    d = x % 10
    x = x // 10
    if ld < d:
      decreasing = False
      break
    if ld == d:
      repct = repct + 1
    else:
      if repct == 1:
        hasdouble = True
      repct = 0
    ld = d
  if repct == 1:
    hasdouble = True
  if hasdouble and decreasing:
    c = c + 1
    #print (i)

print (c)
