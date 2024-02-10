
import subprocess

input = ["../testcase/public1.txt", "../testcase/public2.txt", "../testcase/public3.txt", "../testcase/public4.txt", "../testcase/public5.txt", "../testcase/public6.txt" ]

output = ["../output/public1.out", "../output/public2.out", "../output/public3.out", "../output/public4.out", "../output/public5.out", "../output/public6.out" ]

time = []

for i in range(0, 6):
    command = "time ../bin/hw2 {} {}".format(input[i], output[i])
    print("start " + command)
    result = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

    stdout, stderr = result.communicate()
    time.append(stderr)



with open("./time.txt", "w") as timeFile:
    for i in range(0, 6):
        timeFile.write("public" + str(i+1))
        timeFile.write(time[i] + "\n")