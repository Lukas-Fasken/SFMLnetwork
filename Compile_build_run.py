import os

os.system("cmake -S . -B build")
os.system("cmake --build build --config  Release")
os.system(r"start .\build\bin\main.exe")