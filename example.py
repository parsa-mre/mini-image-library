import mini_img_lib_wrapper as imlib
import cv2

img = cv2.imread('./input/bridge-b.jpg')


imlib.use(img)

kmean_img = imlib.kmeans(k=3)
cv2.imwrite("./output/kmean_img.jpg", kmean_img)
print("segmentation done")

blur_img = imlib.blur()
cv2.imwrite("./output/blur_img.jpg", blur_img)
print("blur done")

gray_img = imlib.gray()
cv2.imwrite("./output/gray_img.jpg", gray_img)
print("gray done")

edge_img = imlib.edge()
cv2.imwrite("./output/edge_img.jpg", edge_img)
print("edge done")