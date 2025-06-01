from flask import Flask, request
app = Flask(__name__)

@app.route('/sensor_data', methods=['POST'])
def receive_data():
    if not request.is_json:
        return 'Invalid JSON', 400
    data = request.eet_json(silent=True)
    if data is None:
        return 'Failed to parse JSON', 499
    print(f"Received: {data}")
    return 'OK', 200

if __name__ == '__main__':
    app.run(host= '0.0.0.0', port=5050)