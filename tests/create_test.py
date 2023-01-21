from os import listdir
from os.path import isfile, join
import os
from dataclasses import dataclass, asdict

import subprocess
import yaml

@dataclass
class TestCase:
    file: str
    stdout: str
    stderr: str
    exit_code: int


def run_shell(file : str) -> subprocess.CompletedProcess:
    return subprocess.run(["bash", "--posix", file], capture_output=True, text=True)

def create_testcase_from_file(file : str) -> TestCase:
    process = run_shell(file)
    return TestCase(file, process.stdout, process.stderr != "", process.returncode)
    

if __name__ == "__main__":
    test_directory = ["comment", "edge_case", "echo", "exit", "empty", "if", "simple_command", "script", "single_quote", "true_false", "redirection"]
    #test_directory = ["redirection", "42trash_criterion"]
    #test_directory = ["variable"]

    nb_dir = len(test_directory)

    path = "./tests/data.yml"

    if(isfile(path)):
        os.remove(path)
    
    for i in range(nb_dir): 
        test_repo = "./tests/"+ test_directory[i]
        test_files = [join(test_repo, file) for file in sorted(listdir(test_repo), key=str.lower) if isfile(join(test_repo, file))]
        test_cases = [create_testcase_from_file(file) for file in test_files]

        test_dicts = [asdict(case) for case in test_cases]

        
        
        with open(path, "a+") as outfile:
            yaml.dump(test_dicts, outfile)