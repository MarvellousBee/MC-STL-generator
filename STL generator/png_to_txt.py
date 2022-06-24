from ctypes import resize
from sys import flags
import cv2
import numpy as np
from PIL import Image

num_of_colors = 8

def get_unique_colors(img):
    uniqueColors = set()
    for line in img:
        for i in line:
            uniqueColors.add(tuple(i))
    return uniqueColors
    """
    totalUniqueColors = len(uniqueColors)
    for i in uniqueColors:
        print(i)
    print(totalUniqueColors)
    """

def kmeans_color_quantization(image, clusters=8, rounds=1):
    h, w = image.shape[:2]
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

result = kmeans_color_quantization(image, clusters=num_of_colors, rounds = 500)
unique_colors = get_unique_colors(result)

to_white_thresh = 200
white_candidates = []
for i in unique_colors:
    print(i)
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
    result = kmeans_color_quantization(image, clusters=num_of_colors-1, rounds = 500)

cv2.imwrite("output.png", result)

print("Image generated!\nYou may edit it now.\nPress Enter to generate a text file...")
input()

im = Image.open("output.png") 
pixel_values = list(im.getdata())
width, height = im.size
pixel_values = np.array(pixel_values)
np.savetxt("text_output.txt", pixel_values)