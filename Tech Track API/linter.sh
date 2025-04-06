#!/bin/bash
find . -name "*.cpp" -o -name "*.h" | xargs python3 cpplint.py --linelength=100