import pytest
import subprocess
import time

SANDBOX = "build/examples/sandbox_example"
VALGRIND = ["valgrind", "--leak-check=full", "--error-exitcode=1"]
TIMEOUT = 5.0  # sec


@pytest.mark.parametrize("layer", ["empty", "triangle"])
def test_example(layer, valgrind):
    sandbox_args = [SANDBOX, "-e", layer]
    if valgrind:
        sandbox_args = VALGRIND + sandbox_args
    sandbox = subprocess.Popen(sandbox_args)
    time.sleep(TIMEOUT)
    sandbox.terminate()
    sandbox.wait(TIMEOUT)
    assert sandbox.returncode == 0
