import subprocess

output = subprocess.check_output("cat sample_input.txt | ./nums", shell=True)
output = output.decode("utf-8").strip()

expected = ""
with open("sample_expected_output.txt", "r") as in_file:
	for line in in_file:
		expected += line

expected = expected.strip()

print ("Expected output: ", expected)
print ("Obtained output: ", output)

if (expected == output):
	print ("Test case successful")
