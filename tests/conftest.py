import pytest


def pytest_addoption(parser):
    parser.addoption("--valgrind", action="store_true", help="Run examples with valgrind")


@pytest.fixture()
def valgrind(pytestconfig):
    return pytestconfig.getoption("valgrind")
