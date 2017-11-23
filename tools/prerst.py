"""prerst: Preprocess certain reStructuredText pattersn not supported by github.


Features:
 - includeliteral-like substitution
 - exclusions (delete lines, useful for private notes)


Examples:

$PRERST$INCLUDE$src/src.c(1-8,34,indent=3)
- This line gets replaced by a block containing the lines 1-8 and 34 of src/src.c.
All of those lines will start with the given indentation (3 spaces here).

$PRERST$EXCLUDE$To be, or not to be - that is the question
 - This line will not appear in the output at all.


/*****************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Lucas Siqueira <lucas.morais.siqueira@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************/
"""


import os
import re
import sys

PRERST_PATTERN = "\$PRERST\$(?P<parser>[a-zA-Z]+)\$(?P<text>.+)"
PARSER_INCLUDE_PATTERN = (
    "^ *"                        # possible leading spaces
    "(?P<filename>[^\( ]+) *"    # group filename
    "\( *"                       # opening parenthesis
    "(?P<lines>[\d,-]+) *"       # group lines
    "(indent=(?P<indent>\d+))? *"  # group indent
    "\) *$"                      # closing parenthesis
)
LINE_RANGE_SINGLE = '^ *(?P<a>\d+) *$'
LINE_RANGE_MULTI = '^ *(?P<a>\d+)[ -]*(?P<b>\d+)? *$'


def log_line(filename, line_n, msg=""):
    """  File "./tools/prerst.py", line 28, in parse_line"""
    print("  File \"{}\", line {}, {msg}".format(filename, line_n, msg))


def parse_line_ranges(filename, lines, indent):
    block = []
    with open(filename) as f:
        linelist = f.readlines()
    for l_range in lines:
        match = re.match(LINE_RANGE_MULTI, l_range)
        a = match.group('a')
        b = match.group('b') or a
        a, b = int(a) - 1, int(b) - 1
        block.extend(linelist[a:b + 1])
        print("parse_line_ranges:", a, b)
    if indent > 0:
        block = [(" " * indent) + l for l in block]
    print("parse_line_ranges block:", block)
    return ' '.join(block)


def parser_include(text):
    match = re.match(PARSER_INCLUDE_PATTERN, text)
    try:
        filename = match.group('filename')
        lines = [
            line
            for line in match.group('lines').replace(" ", "").split(",")
            if line
        ]
        indent = int(match.group('indent') or 0)
        print("parser_include(filename={}, lines={}, indent={})".format(
            filename, lines, indent))
    except AttributeError:
        raise
    return parse_line_ranges(filename, lines, indent)


def parser_exclude(text):
    print("parser_exclude called", text)
    return ""

PARSERS = {
    "INCLUDE": parser_include,
    "EXCLUDE": parser_exclude,
}


def parse_line(line, n):
    match = re.match(PRERST_PATTERN, line)
    try:
        parser = match.group('parser')
    except AttributeError:
        print('Error: invalid PRERST parser "{}"'.format(line[:-1]))
        sys.exit(1)
    try:
        return PARSERS[parser](match.group('text'))
    except AttributeError:
        print('Error: invalid parser arguments "{}"'.format(line[:-1]))
        sys.exit(1)


def preprocess(filename):
    with open(filename, 'r') as pre:
        linelist = pre.readlines()

    with open(filename.replace(".pre.rst", ".rst"), 'w') as rst:
        i = 1
        for line in linelist:
            if line.startswith("$PRERST$"):
                rst.write(parse_line(line, i))
            else:
                rst.write(line)
            i += 1


def walk_on_path(path):
    for root, dirs, files in os.walk(path):
        for name in files:
            if name.lower().endswith("pre.rst"):
                preprocess(name)


def main():
    path = os.getcwd()
    print("Current working directory: " + path)
    walk_on_path(path)


if __name__ == "__main__":
    main()
