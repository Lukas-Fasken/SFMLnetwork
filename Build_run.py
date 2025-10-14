# import os

# os.system("cmake -S . -B build")
# os.system("cmake --build build --config  Release")
# os.system(r"start .\build\bin\main.exe")
import subprocess

# Run CMake configure
subprocess.run(["cmake", "-S", ".", "-B", "build"], check=True)

# Run CMake build
subprocess.run(["cmake", "--build", "build", "--config", "Release"], check=True)

# Run the built executable and forward its output to the terminal
subprocess.run([r".\build\bin\main.exe"], check=True)