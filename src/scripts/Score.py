score = 0

def calculate(food):
    global score
    if food == 1:
        score += 1
    else:
        score += 5

def setScore(_score):
    global score
    score = _score

def getScore():
    return str(score)
