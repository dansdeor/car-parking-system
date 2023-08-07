from flask import Flask, request, jsonify, render_template
import base64
from datetime import datetime


app = Flask(__name__)


def parse_json(json_data):
    try:
        id = json_data.get('id')
        time = json_data.get('time')
        image_base64 = json_data.get('image')

        if id is None or not isinstance(id, int):
            return {'error': 'Invalid or missing "id" field'}, 400

        if time is None:
            return {'error': 'Missing "time" field'}, 400
        try:
            time = datetime.fromisoformat(time)
        except ValueError:
            return {'error': 'Invalid "time" format'}, 400

        if image_base64 is None:
            return {'error': 'Missing "image" field'}, 400
        try:
            image_data = base64.b64decode(image_base64)
        except ValueError:
            return {'error': 'Invalid base64 encoded "image" data'}, 400

        # Process the parsed data here
        response = {
                    'message': 'JSON data received and parsed successfully',
                    'id': id,
                    'time': time,
                    'image': image_base64
                    }
        return response, 200
    except Exception as e:
        return {'error': str(e)}, 400


@app.route('/json_endpoint', methods=['POST'])
def json_endpoint():
    try:
        data = request.get_json()  # Get JSON data from the request
        response, status_code = parse_json(data)
        if status_code == 200:
            return render_template('result.html', json_data=response)
        else:
            return jsonify(response), status_code
    except Exception as e:
        return jsonify({'error': str(e)}), 400


if __name__ == '__main__':
    app.run(debug=True)