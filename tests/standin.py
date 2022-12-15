# stand-in Python script for integrating

import os
from time import sleep
import json
from shutil import rmtree, copytree


PYTHON_DIR = ""#"../files/python"
END_FILE_NAME = ""#"TESTS.DONE"
MC_EXE_PATH = ""#"/mnt/c/Program Files (x86)/Minecraft Launcher/MinecraftLauncher.exe"
MC_DP_PATH = ""#

# set relevant configs from configs.json
with open("../universal/configs.json", 'r') as file:
    js_conts = json.load(file)
    PYTHON_DIR = js_conts["PYTHON_PATH"]
    END_FILE_NAME = js_conts["END_FILE_NAME"]
    MC_EXE_PATH = js_conts["MC_EXE_PATH"]
    MC_DP_PATH = js_conts["MC_DP_PATH"]

# helper functions

# takes in file name/path and gets path relative to python directory
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

def copyPackFiles(pack):
    try:
        copytree(addPy(pack), f"{MC_DP_PATH}/{pack}")
    except:
        rmtree(f"{MC_DP_PATH}/{pack}")
        copytree(addPy(pack), f"{MC_DP_PATH}/{pack}")

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
            copyPackFiles(pack)
            with open(addPy(pack + '/' + "pack.mcmeta"), 'r') as file:
                js = json.load(file)
            # parse result
            rez = str(js["pack"]["pack_format"]) # rez == result of test (as string)
            # END **placeholder for pywinauto**

            # write output
            with open(addPy(pack) + '.txt', 'w') as file:
                file.write(rez)
            # delete compiled
            cleanupPack(pack)