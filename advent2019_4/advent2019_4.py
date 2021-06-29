
count1 = 0
count2 = 0

# Loop through the range given in my input
for i in range(372304, 847061):

  # Store in another var, because we trash it
  current = i

  # Last digit we saw (to check decreasing)
  lastdigit = 10

  # Double check
  hasdouble = False
  hasanydouble = False

  # Repetition count
  repcount = 0

  # Are we seeing decreasing?
  decreasing = True

  # Loop through the digits - note we're doing this
  # right to left
  while current > 0:

    # Get the current digit ('ones')
    digit = current % 10
    current = current // 10

    # Check we're decreasing
    if lastdigit < digit:
      decreasing = False
      break

    # Check for repetition
    if lastdigit == digit:
      repcount = repcount + 1
    else:
      # Check part1 and part2 double rules
      if repcount == 1:
        hasdouble = True
      if repcount != 0:
        hasanydouble = True

      repcount = 0

    # Remember last digit
    lastdigit = digit

  # Check double again (at end of loop)
  if repcount == 1:
    hasdouble = True
  if repcount != 0:
    hasanydouble = True

  # If we've a double, and decreasing, then good to go
  if hasanydouble and decreasing:
    count1 = count1 + 1
  if hasdouble and decreasing:
    count2 = count2 + 1

print ("Count 1 = ", count1)
print ("Count 2 = ", count2)
