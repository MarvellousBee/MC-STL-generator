from ctypes import resize
from sys import flags
import cv2
import numpy as np
from PIL import Image
from sty import bg

num_of_colors = 7
num_of_rounds = 500

def print_color(color):
    print(bg(color[0], color[1], color[2]) + "   " + bg.rs, end="")


def get_unique_colors(img):
    uniqueColors = []
    for line in img:
        for color in line:
            color_tuple = tuple(color)
            if int(color[3]) != 0 and color_tuple not in uniqueColors:
                uniqueColors.append(color_tuple)
    return uniqueColors

def kmeans_color_quantization(image, clusters=7, rounds=1):
    h, w = image.shape[:2]
    clusters+=2
    samples = np.zeros([h*w,4], dtype=np.float32)
    count = 0

    for x in range(h):
        for y in range(w):
            samples[count] = image[x][y]
            count += 1

    compactness, labels, centers = cv2.kmeans(samples,
            clusters, 
            None,
            (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10000, 0.0001), 
            rounds, 
            cv2.KMEANS_RANDOM_CENTERS)

    centers = np.array(centers)
    res = centers[labels.flatten()]
    return res.reshape((image.shape))

image = cv2.imread('skin.png', flags = cv2.IMREAD_UNCHANGED)

result = kmeans_color_quantization(image, clusters=num_of_colors, rounds = num_of_rounds)

unique_colors = get_unique_colors(result)

to_white_thresh = 200
white_candidates = []
for i in unique_colors:
    if ( sum(i[:3]) / len(i[:3]) > to_white_thresh):
        white_candidates.append(i)

max_color = -1
white = np.array([255,255,255,255])
top_candidate = white

for i in white_candidates:
    if max_color < sum(i):
        top_candidate = i
        max_color = sum(i)

if max_color != -1:
    print("Detected white-like color!")
    for x in range(64):
        for y in range(64):
            if tuple(result[x,y]) == top_candidate:
                result[x,y] = white
else:
    print("white-like color NOT DETECTED! Regenerating...")
    result = kmeans_color_quantization(image, clusters=num_of_colors-1, rounds = num_of_rounds)

cv2.imwrite("output.png", result)

print("Image generated!\nYou may edit it now.\nPress Enter to generate a text file...")
input()
image2 = cv2.imread('output.png', flags = cv2.IMREAD_UNCHANGED)

unique_colors = get_unique_colors(image2)
assert len(unique_colors) <= num_of_colors or (len(unique_colors) <= num_of_colors-1 and max_color == -1)


#for some reason, they R and B values get swapped along the way, but the image renders fine!
#fix colors for display
marker_colors = []


for pixel in unique_colors:
    if tuple(white[:3]) == pixel[:3]:
        marker_colors.append(white)

for pixel in unique_colors:
    if tuple(white[:3]) == pixel[:3]:
        continue
    #for some reason, R and B values get swapped along the way. I'm fixing that here.
    temp = []
    prev_color = pixel[0]
    temp.append(pixel[2])
    temp.append(pixel[1])
    temp.append(prev_color)
    temp.append(pixel[3])
    marker_colors.append(temp)



#display colors
i = 1
for color in marker_colors:
    #if color != [255, 255, 255, 255]:
    print(i, ' ', end = '')
    print_color(color)
    print("  " + str(color[0]) + '  \t' + str(color[1]) + '  \t' + str(color[2]))
    i+=1

im = Image.open("output.png") 
pixel_values = list(im.getdata())
width, height = im.size
pixel_values = np.array(pixel_values)
np.savetxt("text_output.txt", pixel_values)
print("\n\nDone!")