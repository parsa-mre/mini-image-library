import numpy as np
import cv2
from matplotlib import cm
import mini_image_lib as ilib

img = None


def use(image, threads=2):
    global img
    img = image
    ilib.use(image.reshape(-1).tolist(),
             image.shape[0], image.shape[1], threads)


def kmeans(k=8, iterations=32):
    global img
    tmp = np.empty([(img.shape[0])*(img.shape[1])], dtype=np.float64)
    ilib.kmean(tmp, k, iterations)
    tmp = tmp.reshape((img.shape[0], img.shape[1]))
    tmp = tmp / tmp.max()
    segmented = cm.jet(tmp) * 255
    return segmented


def blur(filer_size=75, iterations=5, sigma=3.8):
    global img
    blured = np.empty([(img.shape[0])*(img.shape[1])*3], dtype=np.float64)
    ilib.blur(blured, iterations, filer_size, sigma)
    blured = blured.reshape(img.shape)
    return blured


def gray():
    global img
    gray = np.empty([(img.shape[0])*(img.shape[1])], dtype=np.float64)
    ilib.gray_scale(gray)
    gray = gray.reshape((img.shape[0], img.shape[1]))
    return gray


def edge():
    global img
    edge = np.empty([(img.shape[0])*(img.shape[1])], dtype=np.float64)
    ilib.detect_edge(edge)
    edge = edge.reshape((img.shape[0], img.shape[1]))
    return edge
