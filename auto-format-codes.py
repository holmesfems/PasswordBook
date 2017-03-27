#!/usr/bin/env python3

import os
from pathlib import Path
from multiprocessing import Pool

SRC_DIRS = ['src',]
C_SRC_EXT = ['c', 'cpp', 'h', 'cxx', 'cc']
def get_file_pattern():
    for src in SRC_DIRS:
        for ext in C_SRC_EXT:
            yield (src, ext)

AUTO_FORMAT_BIN = 'clang-format -i'
p = Path('.')

def auto_format_code(pattern):
    f_name = "{src}/*.{ext}".format(src = pattern[0], ext = pattern[1])
    for files in p.glob(f_name):
        print(os.getpid(), files)
        cmd = "{AUTO_FORMAT_BIN} {files}".format(AUTO_FORMAT_BIN = \
                AUTO_FORMAT_BIN, files = files)
        os.system(cmd)

#for sdir in SRC_DIRS

with Pool(processes = os.cpu_count()) as pool:
    pool.map(auto_format_code, get_file_pattern())

