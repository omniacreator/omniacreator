#!/usr/bin/env python

################################################################################
# @file
# Preprocess Script
#
# @version @n 1.0
# @date @n 3/29/2014
#
# @author @n Kwabena W. Agyeman
# @copyright @n (c) 2014 Kwabena W. Agyeman
# @n All rights reserved - Please see the end of the file for the terms of use
#
# @par Update History:
# @n v1.0 - Original release - 3/29/2014
################################################################################

import argparse, os, sys, re

IGNORE = [
            "Arduino.h",
            "WProgram.h",
            "simpletools.h",
            "mstimer.h",
            "simpletext.h",
            "serial.h",
            "fdserial.h",
            "serialdevice.h"
         ]

def preprocess_file(file_name, include_paths, include_history = []):

    lines = []
    regex = re.compile(r"#include\s+[\"<](.+?)[\">]")

    with open(file_name) as file:
        for line in file:
            lines.append(line)

            match = regex.search(line)

            if match:
                name = match.group(1)

                if name not in IGNORE:
                    for path in include_paths:
                        include = os.path.abspath(os.path.join(path, name))

                        if not os.path.isfile(include): continue

                        lines.pop()
                        lines.append('\n')

                        if include in include_history: break

                        include_history.append(include)
                        lines.extend(preprocess_file(include, include_paths,
                                                     include_history))

                        break

    return lines

if __name__ == "__main__":

    __folder__ = os.path.dirname(os.path.abspath(__file__))

    parser = argparse.ArgumentParser(description = "Preprocess Script")

    parser.add_argument("-I", "--include", nargs = '?', default = '.',
                        action = "append")

    parser.add_argument("input_file")
    parser.add_argument("output_file")

    args = parser.parse_args()

    if not os.path.exists(args.input_file):
        sys.exit("Src File \"%s\" does not exist!" % args.input_file)

    # Preprocess ##############################################################

    with open(args.output_file, 'w') as file:
        for line in preprocess_file(args.input_file, args.include):
            file.write(line)

    # Format ##################################################################

    with open(args.output_file, 'r') as file:
        text = re.sub(r"\n\n+", "\n\n", file.read())

    with open(args.output_file, 'w') as file:
        file.write(text)

################################################################################
# @file
# @par MIT License - TERMS OF USE:
# @n Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# @n
# @n The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# @n
# @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
################################################################################
