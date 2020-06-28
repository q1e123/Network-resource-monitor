import os
import sys

def get_args():
    args = {}
    if "-h" in sys.argv:
        args["header"] = sys.argv[sys.argv.index("-h")+1]

    if "-s" in sys.argv:
        args["src"] = 1
    else:
        args["src"] = 0

    return args

def create_header(name):
    header_file = open(name + ".h","w+")    
    header_file.write("#ifndef " + name.capitalize() + "_H\n")
    header_file.write("#define " + name.capitalize() +"_H\n")
    header_file.write("class " + name + "{\n\tpulbic:\n\tprivate:\n")
    header_file.write("#endif\n")

def create_source(name):
    src_file = open(name + ".cpp","w+")

def create_class():
    args = get_args()
    if "header" in args:
        create_header(args["header"]) 
    if args["src"]==1:
        create_source(args["header"])

create_class()

