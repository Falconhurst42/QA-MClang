import os
from time import sleep
import json
from shutil import rmtree, copytree

PYTHON_DIR = "../files/python"
END_FILE_NAME = "TESTS.DONE"
MC_PATH = ""

def addPy(p):
    return PYTHON_DIR + '/' + p

def getPacks(d):
    return [c for c in os.listdir(PYTHON_DIR) if os.path.isdir(addPy(c))]

def checkForEndfile(d):
    return END_FILE_NAME in [c for c in os.listdir(PYTHON_DIR) if os.path.isfile(addPy(c))]

def cleanupPack(pack_name):
    # https://stackoverflow.com/a/303225
    rmtree(addPy(pack_name))

def cleanupFile(f):
    os.remove(addPy(f))

def movePackFiles(pack):
    copytree(addPy(pack), MC_PATH)

if __name__ == "__main__":
    # pre-emptively try to remove done file
    try:
        cleanupFile(END_FILE_NAME)
    except:
        pass

    # loop until done signal received
    while True:
        # get packs
        packs = getPacks(PYTHON_DIR)
        # while there are no packs...
        while (len(packs) == 0):
            # check for done signal
            if(checkForEndfile(PYTHON_DIR)):
                # exit
                cleanupFile(END_FILE_NAME)
                print(f"Python Done")
                exit()
            # wait
            sleep(0.1)
            # get packs again
            packs = getPacks(PYTHON_DIR)
        
        # for each pack in packs
        js = {}
        for pack in packs:
            # BEGIN **placeholder for pywinauto**
            # open pack files
            with open(addPy(pack) + '/' + "pack.mcmeta", 'r') as file:
                js = json.load(file)
            # parse result
            rez = str(js["pack"]["pack_format"])
            # END **placeholder for pywinauto**

            # write output
            with open(addPy(pack) + '.txt', 'w') as file:
                file.write(rez)
            # delete compiled
            cleanupPack(pack)