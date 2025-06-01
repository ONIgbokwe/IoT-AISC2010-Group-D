from flask import Flask, request
from datetime import datetime
import os

app = Flask(__name__)
CSV_FILE = "phyphox_data.csv"

@app.route('/csv_upload', methods=['POST'])
def receive_csv():
    try:
        csv_data = request.data.decode()  # Raw CSV data from Phyphox
        with open(CSV_FILE, 'a') as f:
            f.write(csv_data + "\n")
        return "CSV saved!", 200
    except Exception as e:
        return f"Error: {e}", 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5050)