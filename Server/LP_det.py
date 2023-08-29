import LP_utils
import numpy as np
import pytesseract
import cv2


def lcp_detector(input_image):
    # convert to numpy array
    image = cv2.imdecode(np.fromstring(
        input_image, dtype=np.uint8), cv2.IMREAD_UNCHANGED)

    gray_image = LP_utils.gray_scale(image)
    blurred = cv2.GaussianBlur(gray_image, (5, 5), 0)
    edges = LP_utils.auto_canny(blurred)
    contours, new = cv2.findContours(
        edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=LP_utils.cv2.contourArea,
                      reverse=True)[0:30]
    new_img = None

    for c in contours:
        # approximate the license plate contour
        contour_perimeter = cv2.arcLength(c, True)

        approx = cv2.approxPolyDP(c, 0.01 * contour_perimeter, True)
        # Look for contours with 4 corners
        if len(approx) == 4:
            screenCnt = approx
            # find the coordinates of the license plate contour
            x, y, w, h = cv2.boundingRect(c)
            new_img = gray_image[y: y + h, x: x + w]
            break

    if new_img:
        thresh, im_bw = cv2.threshold(
            new_img, 127, 255, cv2.THRESH_BINARY)
        cv2.imwrite("bw_image.jpg", im_bw)
        LP_utils.display("bw_image.jpg")
        text = pytesseract.image_to_string(im_bw, config='--psm 6')
        print(text)
        return text
    return ""
