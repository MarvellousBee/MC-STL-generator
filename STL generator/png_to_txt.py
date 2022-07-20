from ctypes import resize
import cv2
import numpy as np
from PIL import Image
from sty import bg
from sklearn.cluster import KMeans
from deprecated import deprecated
merge_colors = False
look_for_white_like_color = False
num_of_colors = 20
#num_of_rounds = 1#500

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

@deprecated(reason="Does not reduce colors to the correct amount. Use kmeans_improved().")
def kmeans_color_quantization(image, clusters, rounds):
    h, w = image.shape[:2]
    clusters+=1
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

def kmeans_improved(img, clusters):
    clusters+=1
    w,h,_ = img.shape
    img = img.reshape(w*h,4)
    kmeans = KMeans(n_clusters=clusters, random_state=0).fit(img)

    # find out which cluster each pixel belongs to.
    labels = kmeans.predict(img)

    # the cluster centroids is our color palette
    identified_palette = np.array(kmeans.cluster_centers_).astype(int)

    # recolor the entire image
    recolored_img = np.copy(img)
    for index in range(len(recolored_img)):
        recolored_img[index] = identified_palette[labels[index]]
        
    # reshape for display
    return recolored_img.reshape(w,h,4)

def blue_swap(pixel):
    temp = []
    prev_color = pixel[0]
    temp.append(pixel[2])
    temp.append(pixel[1])
    temp.append(prev_color)
    temp.append(pixel[3])
    return temp

white = np.array([255,255,255,255])

image = cv2.imread('skin.png', flags = cv2.IMREAD_UNCHANGED)

marker_colors = []
if merge_colors:
    assert num_of_colors < len(get_unique_colors(image))

    result = kmeans_improved(image, clusters=num_of_colors)

    unique_colors = get_unique_colors(result)
    if look_for_white_like_color:

        to_white_thresh = 200
        white_candidates = []
        for i in unique_colors:
            if ( sum(i[:3]) / len(i[:3]) > to_white_thresh):
                white_candidates.append(i)

        max_color = -1
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
    print(len(unique_colors), num_of_colors)
    assert len(unique_colors) == num_of_colors
    #for some reason, they R and B values get swapped along the way, but the image renders fine!
    #fix colors for display

    for pixel in unique_colors:
        if tuple(white[:3]) == pixel[:3]:
            marker_colors.append(white)

    for pixel in unique_colors:
        if tuple(white[:3]) != pixel[:3]:
            marker_colors.append(blue_swap(pixel))

else:
    cv2.imwrite("output.png", image)
    unique_colors = get_unique_colors(image)
    for pixel in unique_colors:
        marker_colors.append(blue_swap(pixel))



#display colors
i = 1
for color in marker_colors:
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