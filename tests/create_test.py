from os import listdir
from os.path import isfile, join

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
    test_directory = ["comment", "echo", "edge_case", "if", "script", "simple_command", "single_quote", "true_false", "redirection"]

    for dir in sorted(test_directory): 
        test_repo = "./tests/"+dir+"/"
        test_files = [join(test_repo, file) for file in sorted(listdir(test_repo), key=str.lower) if isfile(join(test_repo, file))]
        test_cases = [create_testcase_from_file(file) for file in test_files]

        test_dicts = [asdict(case) for case in test_cases]

        with open("./tests/data.yml", "a+") as outfile:
            yaml.dump(test_dicts, outfile)