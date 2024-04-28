while True:
    owed = 0
    try:
        owed = float(input("Change owed: "))
    except:
        pass
    if 0 < owed:
        break
owed = int(owed * 100)
coins = [25, 10, 5, 1]
n = 0

for i in coins:
    n += owed // i
    owed -= (owed // i) * i


print(n)
