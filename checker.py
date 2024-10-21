import subprocess
import os

executable_name = 'json_eval'
input_dir = 'tests'
output_dir = 'output'
refs_dir = 'refs'
os.makedirs(output_dir, exist_ok=True)
def count_subdirectories(directory):
    subdirs = [d for d in os.listdir(directory) if os.path.isdir(os.path.join(directory, d))]
    return len(subdirs)

def run_command(command):
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    return result.stdout

run_command('make clean')
run_command('make')

count = count_subdirectories(input_dir)
for i in range(1, count + 1):

    dir_path = os.path.join(input_dir, "test" + str(i))
    test_json = os.path.join(dir_path, "test.json")

    with open(os.path.join(dir_path, "query.txt"), 'r') as file:
        query = file.read()
    query = f"\"{query}\""
    
    answer = run_command(f'./{executable_name} {test_json} {query}')

    new_output = os.path.join(output_dir, "test" + str(i) + ".out")
    
    with open(new_output, 'w') as file:
        file.write(answer)

failed = []
for i in range(1, count + 1):
    ref = os.path.join(refs_dir, "test" + str(i) + ".ref")
    output = os.path.join(output_dir, "test" + str(i) + ".out")
    ans_ref = ""
    ans_out = ""
    with open(output, 'r') as file:
        ans_out = file.read()
    with open(ref, 'r') as file:
        ans_ref = file.read()
    format = "--------------------------------------------------------"
    if i < 10:
        format += '-'
    if (ans_ref == ans_out):
        print(f"Test {i} {format} Passed")
    elif (ans_ref[:5] == "Error" and ans_out[:5] == "Error"):
        print(f"Test {i} {format} Passed")
    else:
        print(f"Test {i} {format} Failed")
        failed.append(i)

if len(failed) == 0:
    print("All of the outputs are correct.", end="")
else:
    print("It fails on this cases: ", end ="")
    for i in failed:
        print(i, end=" ")
print('')
        