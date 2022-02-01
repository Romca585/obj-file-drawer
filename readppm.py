import sys
import binascii
import base64

def setHeader(width, rows, depth, tokens):  
    for i in tokens:
        if(i != '\n' and len(i) != 0 and i != " "):
            if(width == "" and rows == "" and depth == ""):
                width = i;

            elif (width != "" and rows == "" and depth == ""):
                rows = i;

            elif (width != "" and rows != "" and depth == ""):
                depth = i;
    return width, rows, depth

def parseHeader(f, isP6):
    line = ""
    depth = ""
    width = ""
    rows = ""
    tokens = []
    while (width == "" or rows == "" or depth == ""):
        line = f.readline()
        if(isP6 == 1):
            try:
                line = line.decode("UTF8")
            except AttributeError:
                pass
                
            #line = binascii.b2a_base64(line)
            #line = ' '.join([str(ord(i)) for i in line.decode('base64')])
        tokens = line.split(" ")       
        if("#" not in line and "P3" not in line and "P6" not in line):
            width, rows, depth = setHeader(width, rows, depth, tokens)               


    return int(width), int(rows), int(depth)

def setContent(line, data):
    #print(line)
    line = line.split(" ")
    for i in line:
        #print(i)
        i = i.split('\n')
        for f in i:
            if (f != ' ' and f != '\n' and len(f) != 0):
                data.append(int(f))
    return data

def parseContent(f, width, rows, isP6):
    line = ""
    data = []
    notEOF = True
    tokens = []
    length = 0
    while notEOF:
        line = f.readline()
        if not line:
            notEOF = False
        if(isP6 == 1):
            try:
                line = binascii.b2a_base64(line)
                line = ' '.join([str(i) for i in base64.b64decode(line)])
            except TypeError:
                line = line.encode()
                line = binascii.b2a_base64(line)
                line = ' '.join([str(i) for i in base64.b64decode(line)])
        if('#' not in line and line != '' and line != '\n'):
            data = setContent(line, data)

    return data

def main(filename):
    filename = filename + ".ppm"
    isP6 = 0;
    try:
        f = open(filename, "r")
        line = f.readline()
        if "P6" in line:
            isP6 = 1
            f.close()
            f = open(filename, "rb") 
            line = f.readline()
    except UnicodeDecodeError:
        f.close()     
        f = open(filename, "rb") 
        line = f.readline()
        line = line.decode("UTF8")
        if "P6" in line: 
            isP6 = 1
      
    header = parseHeader(f, isP6)
    content = parseContent(f, int(header[0]), int(header[1]), isP6)
    f.close()
    data = []
    for i in header:
        data.append(i)
    for i in content:
        data.append(i)
    return data


