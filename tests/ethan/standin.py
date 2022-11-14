import os

PYTHON_DIR = "../files/python"

conts = os.listdir(PYTHON_DIR)
dirs = [PYTHON_DIR + "/" + c for c in conts if os.path.isdir(PYTHON_DIR + "/" + c)]
files = [PYTHON_DIR + "/" + c for c in conts if os.path.isfile(PYTHON_DIR + "/" + c)]

print(f"Dirs: [{', '.join(dirs)}]")
print(f"Files: [{', '.join(files)}]")