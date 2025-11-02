import os
import platform
import subprocess
import sys

def main():
    if platform.system() != "Windows":
        print("Build supported only for Windows.")
        sys.exit(0)

    build_dir = "build-win32"

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    cmd = [
        "cmake",
        ".",
        "-B", build_dir,
        "-G", "Visual Studio 17 2022",
        "-A", "Win32"
    ]

    print("Running command:", " ".join(cmd))

    result = subprocess.run(cmd, shell=True)

    if result.returncode == 0:
        print("CMake project generated in the folder:", build_dir)
    else:
        print("CMake error.")
        sys.exit(result.returncode)

if __name__ == "__main__":
    main()
