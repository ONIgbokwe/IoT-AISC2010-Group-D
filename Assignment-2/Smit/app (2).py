from flask import Flask, render_template
import pandas as pd
import plotly.graph_objs as go
import plotly.io as pio

app = Flask(__name__)

@app.route("/")
def index():
    # Load the CSV
    df = pd.read_csv("data.csv")

    # Rename or clean columns if needed
    df.columns = [col.strip() for col in df.columns]
    time_col = df.columns[0]
    accX = df.columns[1]
    accY = df.columns[2]
    accZ = df.columns[3]

    # Create Plotly figure
    fig = go.Figure()
    fig.add_trace(go.Scatter(x=df[time_col], y=df[accX], mode='lines', name='accX'))
    fig.add_trace(go.Scatter(x=df[time_col], y=df[accY], mode='lines', name='accY'))
    fig.add_trace(go.Scatter(x=df[time_col], y=df[accZ], mode='lines', name='accZ'))

    fig.update_layout(title="Accelerometer Data",
                      xaxis_title=time_col,
                      yaxis_title="Acceleration (m/s²)",
                      legend_title="Axis")

    # Convert Plotly fig to HTML div
    graph_html = pio.to_html(fig, full_html=False)

    return render_template("index.html", graph_html=graph_html)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
