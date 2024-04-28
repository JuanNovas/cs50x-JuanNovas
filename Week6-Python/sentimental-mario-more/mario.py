while True:
    n = 0
    try:
        n = int(input("Height: "))
    except:
        pass
    if 0 < n < 9:
        break
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 1) + "  " + "#" * (i + 1))
