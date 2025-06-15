import importlib.util

Import("env")

def is_installed(pkg_name):
    return importlib.util.find_spec(pkg_name) is not None

if not is_installed("psutil"):
    env.Execute("$PYTHONEXE -m pip install psutil")
