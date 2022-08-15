from cs50 import get_int
scores = []
for i in range(3):
    score = get_int("Score:")
    scores += [score]
   # scores.append(score)
#scores = [72,73,33]

average =  sum(scores) / len(scores)
print(f"Average:{average}")