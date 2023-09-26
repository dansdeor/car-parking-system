
import LP_det

with open('./exp/q10.jpg','rb') as image_file:
    image_data = image_file.read()

print(LP_det.lcp_detector(image_data))





