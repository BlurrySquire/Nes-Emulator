import os
import subprocess

# INSTRUCTIONS ON USE #
# You need to have downloaded all tests
# from https://github.com/SingleStepTests/65x02/tree/main/nes6502.
# Place all the .json files in a folder called 'tests'.
#
# I made this script so I know which tests need further investigation.

official_opcodes = [
    "00", "01", "05", "06", "08", "09", "0a", "0d", "0e", "10", "11", "15", "16", 
    "18", "19", "1d", "1e", "20", "21", "24", "25", "26", "28", "29", "2a", "2c", 
    "2d", "2e", "30", "31", "35", "36", "38", "39", "3d", "3e", "40", "41", "45", 
    "46", "48", "49", "4a", "4c", "4d", "4e", "50", "51", "55", "56", "58", "59", 
    "5d", "5e", "60", "61", "65", "66", "68", "69", "6a", "6c", "6d", "6e", "70", 
    "71", "75", "76", "78", "79", "7d", "7e", "81", "84", "85", "86", "88", "8a", 
    "8c", "8d", "8e", "90", "91", "94", "95", "96", "98", "99", "9a", "9d", "a0", 
    "a1", "a2", "a4", "a5", "a6", "a8", "a9", "aa", "ac", "ad", "ae", "b0", "b1", 
    "b4", "b5", "b6", "b8", "b9", "ba", "bc", "bd", "be", "c0", "c1", "c4", "c5", 
    "c6", "c8", "c9", "ca", "cc", "cd", "ce", "d0", "d1", "d5", "d6", "d8", "d9", 
    "dd", "de", "e0", "e1", "e4", "e5", "e6", "e8", "e9", "ea", "ec", "ed", "ee", 
    "f0", "f1", "f5", "f6", "f8", "f9", "fd", "fe"
]

# Try local build, if not then normal user running the tests
executable = "build/Debug/NesEmu.exe"
if not os.dir.exists(executable):
    executable = "build/Release/NesEmu.exe"
if not os.dir.exists(executable):
    executable = "NesEmu.exe"

for filename in os.listdir("tests"):
    if filename.endswith(".json"):
        opcode = filename.split('.')[0]
        
        if opcode in official_opcodes:
            test_path = os.path.join("tests", filename)
            
            try:
                result = subprocess.run(["build/Debug/NesEmu.exe", "--single-step-test", test_path], capture_output=True, text=True)

                if result.returncode == 0:
                    print(f"{opcode}: passed ✅")
                else:
                    print(f"{opcode}: failed ❌")
            except Exception as e:
            	print(f"{e}")

print("Finished all tests.")