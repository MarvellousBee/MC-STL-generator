from PIL import Image 
import numpy as np
import sys

#r'C:\Users\Konrad\source\vs22\STL generator\STL generator\skin.png'
if len((sys.argv))
	INPUT_PATH = sys.argv[1];
else:
	INPUT_PATH = "output/downscaled_image.png";

im2 = Image.open(INPUT_PATH) 

pixel_values = list(im2.getdata())
pixel_values = np.array(pixel_values)
np.savetxt("output/image_RGBA_data.txt", pixel_values)