import argparse
import math

def diagToWidthHeight(diagonal, aspect):
    height = math.sqrt(diagonal ** 2 / (aspect ** 2 + 1))
    width = aspect * height
    return width, height

def toInch(cm):
    return cm / 2.54;

def toCm(inch):
    return inch * 2.54;


parser = argparse.ArgumentParser()

parser.add_argument("diagonal", type=float,
                    help="Diagonal of display (default is centimeters)")
parser.add_argument("-a", "--aspect", type=float, default=16.0/9.0,
                    help="Aspect ratio. (Default is 1.777777)")
parser.add_argument("-i", "--inches", action="store_true",
                    help="Input valus is in inches.")
parser.add_argument("-c", "--centimeters", action="store_true",
                    help="Input valus is in centimeters.")

args = parser.parse_args()

# all calculations are done in centimeters
if args.inches:
    diagonal = toCm(args.diagonal)
else:
    diagonal = args.diagonal

width, height = diagToWidthHeight(diagonal, args.aspect)


print("Diagonal: {}cm Width: {}cm Height: {}cm".format(diagonal, width, height))
print("Diagonal: {}\" Width: {}\" Height: {}\"".format(toInch(diagonal), toInch(width), toInch(height)))


