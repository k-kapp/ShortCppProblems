import random

with open("test.txt", "w") as in_file:
	print("200000 50", file=in_file, end="")
	print(file=in_file)
	for i in range(200000):
		print(random.randrange(1, 200), file=in_file, end=" ")
