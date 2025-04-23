# A formatting script for your server library. 
# 
# This tool makes the required metadata tags on your server for OGXC 
# to operate. Read README.md for full operation details. 
# 
# Any folders with an existing game.ogxc metadata will be skipped. If 
# you wish to re-process, delete these files.
#
# The tool will ask for final permission before building metadata. 

import os
import sys
import hashlib

# MIT License, taken from: https://code.activestate.com/recipes/577058/
def query_yes_no(question, default="yes"):
    valid = {"yes": True, "y": True, "ye": True, "no": False, "n": False}
    if default is None:
        prompt = "[y/n] "
    elif default == "yes":
        prompt = "[Y/n] "
    elif default == "no":
        prompt = "[y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    while True:
        sys.stdout.write(question + prompt)
        choice = input().lower()
        if default is not None and choice == "":
            return valid[default]
        elif choice in valid:
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' " "(or 'y' or 'n').\n")

# check before write
go = query_yes_no("\nI will build metadata tags in the following library directory, is this what you want?\n" + os.getcwd() + "\n", "no")

if not go:
    exit(0)
print ("\n")

start_path = "."
for game in os.listdir(start_path):
    print("Processing: " + game)

    for file in os.listdir(os.path.join(start_path, game)):
        full_file_path = os.path.join(start_path, game, file)

        if os.path.isfile(full_file_path):
            with open(full_file_path, 'rb') as file_to_hash:
                raw_file = file_to_hash.read()
                md5 = hashlib.md5(raw_file).hexdigest()

                print("\tMD5: " + md5 + "\tFile: " + file)