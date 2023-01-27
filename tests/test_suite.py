from argparse import ArgumentParser
from pathlib import Path
from os import listdir
from os.path import isfile, join
from difflib import unified_diff
from dataclasses import dataclass

import yaml
import subprocess
import termcolor

OK_TAG = f"[{termcolor.colored('OK', 'green')}]"
KO_TAG = f"[{termcolor.colored('KO', 'red')}]"

@dataclass
class TestCase:
    file: str
    stdout: str
    stderr: str
    exit_code: int


def diff(expected : str, actual : str) -> str:
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return ''.join(unified_diff(expected_lines, actual_lines, fromfile="expected", tofile="actual"))

def run_shell(shell : str, test : str) -> subprocess.CompletedProcess:
    return subprocess.run([shell, test], capture_output=True, text=True)

def performc_checks(test : TestCase, actual : subprocess.CompletedProcess):
    assert (not test.stderr) or actual.stderr != "", \
        "something was expected on stderr"

    assert test.stdout == actual.stdout, \
        f"sdout differs \n{diff(test.stdout, actual.stdout)}"

    assert test.exit_code == actual.returncode, \
        f"Exited with {actual.returncode} expected {test.exit_code}"



if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    args = parser.parse_args()

    binary_path = "build/42sh"
    
    with open("./tests/data.yml", "r") as file:
        testsuite = [TestCase(**test) for test in yaml.safe_load(file)]

    for testcase in testsuite:
        file = testcase.file

        sh_proc = run_shell(binary_path, file)

        try:
            performc_checks(testcase, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            pass
        
        print("\t===================================\t")
