import random

num_cases = 1

file_obj = open("my_in.txt", "w")

print(num_cases, file=file_obj)

for i in range(num_cases):
	#arr_size = random.randint(2, 100000)
	arr_size = 100000
	modulo = random.randint(1, 1e14)
	print(str(arr_size) + " " + str(modulo), file=file_obj)
	for j in range(arr_size):
		print(random.randint(1, 1e18), file=file_obj, end=" ")


