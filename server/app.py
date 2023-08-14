from flask import Flask, request, make_response
import base64
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
            # just for now TODO:base64.b64decode(image_base64)
            image_data = "image"
        except ValueError:
            return 'Invalid base64 encoded "image" data', HTTP_CODE.BAD_REQUEST

        # This will only contain the parking number when image detected correctly, for example 69
        response = "69"
        return response, HTTP_CODE.OK
    except Exception as e:
        return str(e), HTTP_CODE.BAD_REQUEST


@app.route('/', methods=['POST'])
def json_endpoint():
    json_data = request.get_json()  # Get JSON data from the request
    print(json_data)
    str_response, status_code = parse_json(json_data)
    response = make_response(str_response, status_code)
    return response


if __name__ == '__main__':
    app.run(debug=True)
