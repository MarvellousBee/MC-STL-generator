import cv2
import numpy as np
from PIL import Image
from sty import bg
from sklearn.cluster import KMeans

def print_color(color):
    print(bg(color[0], color[1], color[2]) + "   " + bg.rs, end="")

#Easier to parse in C++
def print_color_distinct(v1,v2,v3):
    print_color([v1,v2,v3])

def get_unique_colors(img):
    uniqueColors = []
    for line in img:
        for color in line:
            color_tuple = tuple(color)
            if int(color[3]) != 0 and color_tuple not in uniqueColors:
                uniqueColors.append(color_tuple)
    return uniqueColors

def kmeans_color_quantization(image, clusters, rounds):
    """Tends to output too little colors, deprecated. Use kmeans_improved()."""
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

def kmeans_improved_from_path(path, clusters, output = "output.png"):
    result = kmeans_improved(read_image(path), clusters)
    write_image(result, output)
    
def get_and_store_unique_colors(skin_to_parse = "output.png", img_path = 'uclrs.txt'):
    with open(img_path, "w", encoding = 'utf-8') as file:
            for i in get_unique_colors(read_image(skin_to_parse)):
                file.write(str(i[2]) + ' ' + str(i[1]) + ' ' + str(i[0]) + '\n')

"""
def blue_swap(pixel):
    temp = []
    prev_color = pixel[0]
    temp.append(pixel[2])
    temp.append(pixel[1])
    temp.append(prev_color)
    temp.append(pixel[3])
    return temp
"""

def read_image(path = "skin.png"):
    return cv2.imread(path, flags = cv2.IMREAD_UNCHANGED)

def write_image(result, path = "output.png"):
    cv2.imwrite(path, result)

def merge_colors(image):
    assert num_of_colors < len(get_unique_colors(image))
    result = kmeans_improved(image, clusters=num_of_colors)
    return result;

    #print("Image generated!\nYou may edit it now.\nPress Enter to generate a text file...")
    #input()

def generate_marker_colors(path = 'output.png'):
    image2 = cv2.imread(path, flags = cv2.IMREAD_UNCHANGED)
    marker_colors = []
    unique_colors = get_unique_colors(image2)
    print(len(unique_colors), num_of_colors)
    assert len(unique_colors) == num_of_colors
    #for some reason, they R and B values get swapped along the way, but the image renders fine!
    #fix colors for display
    for pixel in unique_colors:
        marker_colors.append(blue_swap(pixel))
    return marker_colors



def print_all_colors(marker_colors):
    i = 1
    for color in marker_colors:
        print(i, ' ', end = '')
        print_color(color)
        print("  " + str(color[0]) + '  \t' + str(color[1]) + '  \t' + str(color[2]))
        i+=1

def convert_skin_to_txt(path = 'output.png'):
    im = Image.open(path) 
    pixel_values = list(im.getdata())
    width, height = im.size
    pixel_values = np.array(pixel_values)
    np.savetxt("text_output.txt", pixel_values)