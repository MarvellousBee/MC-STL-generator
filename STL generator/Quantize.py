from PIL import Image 
import sys

#r'C:\Users\Konrad\source\vs22\STL generator\STL generator\skin.png'
INPUT_PATH = sys.argv[1];

if len(sys.argv) > 2:
    NUM_OF_COLORS = int(sys.argv[2]);
else:
    NUM_OF_COLORS = 8;

im2 = Image.open(INPUT_PATH) 
im2 = im2.convert('P', dither=Image.NONE, palette=Image.ADAPTIVE, colors=NUM_OF_COLORS)
im2.save("output/downscaled_image.png")
