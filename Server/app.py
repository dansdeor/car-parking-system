from flask import Flask, request, make_response
from base64 import b64decode
from datetime import datetime
from enum import IntEnum


class HTTP_CODE(IntEnum):
    OK = 200  # Let's goooo
    BAD_REQUEST = 400  # Request is not what we accepted
    NOT_FOUND = 404  # Not detected
    NOT_ACCEPTABLE = 406  # Detected but not approved


app = Flask(__name__)


def parse_json(json_data):
    try:
        node_id = json_data.get('node_id')
        image_base64 = json_data.get('image')
        # change the time to your format. firebase and stuff...
        time = str(datetime.now())

        if node_id is None:
            return 'Missing "id" field', HTTP_CODE.BAD_REQUEST

        if image_base64 is None:
            return 'Missing "image" field',  HTTP_CODE.BAD_REQUEST
        try:
            image_data = b64decode(image_base64)
            # you can now save the image
            with open('saved_image.jpg', 'wb') as image_file:
                image_file.write(image_data)
        except ValueError:
            return 'Invalid base64 encoded "image" data', HTTP_CODE.BAD_REQUEST

        # This will only contain the parking number when image detected correctly, for example 69
        response = "69"
        return response, HTTP_CODE.OK
    except Exception as e:
        return str(e), HTTP_CODE.BAD_REQUEST


@app.route('/awake', methods=['POST'])
def awake_handle():
    json_data = request.get_json()
    print("node {}: woke up".format(json_data.get('node_id')))
    return "", HTTP_CODE.OK


@app.route('/car-left', methods=['POST'])
def car_left_handle():
    json_data = request.get_json()
    print("node {}: car has left".format(json_data.get('node_id')))
    return "", HTTP_CODE.OK


@app.route('/image', methods=['POST'])
def image_handle():
    json_data = request.get_json()
    print(json_data)
    str_response, status_code = parse_json(json_data)
    response = make_response(str_response, status_code)
    return response


if __name__ == '__main__':
    app.run(host='0.0.0.0')
