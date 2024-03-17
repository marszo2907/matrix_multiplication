import argparse
import random

def main():
    parser = argparse.ArgumentParser(prog="generate_matrix.py",
                                     description="The script generates a specified matrix and saves it to a specified file")
    parser.add_argument("-s", "--size", type=int, required=True)
    parser.add_argument("-o", "--output", required=True)
    parser.add_argument("-r", "--random", action="store_true", default=False)
    args = vars(parser.parse_args())
    
    file_name = args["output"] if args["output"].endswith(".txt") else args["output"] + ".txt"
    file = open(file_name, "w")
    file.write(f"{args['size']} {args['size']}\n")
    for row in range(args["size"]):
        for column in range(args["size"]):
            if args["random"]:
                file.write(f"{random.randint(0, 9)} ")
            else:
                file.write("1 ")
        file.write("\n")
    file.close()

if __name__ == "__main__":
    main()
