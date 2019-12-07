import sys
import configparser

print ("Generating config file for ST")
data = sys.argv[1]

binaries = data.split(";")

config = configparser.ConfigParser()
config.add_section("BINARIES")
config["BINARIES"] = {}

for binary in binaries:
    if len(binary) == 0:
        continue
    binary_data = binary.split(":")
    name = binary_data[0]
    path = binary_data[1]
    print("Adding {", name, ": ", path, "}")
    config["BINARIES"].update({name: path})


with open("config.ini", "w") as f:
    config.write(f)
