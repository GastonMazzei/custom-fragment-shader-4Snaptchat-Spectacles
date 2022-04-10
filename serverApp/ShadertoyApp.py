import os, sys
from flask import Flask
from flask import jsonify
from flask import request
from flask import make_response
from datetime import datetime

from flask_cors import CORS, cross_origin
# pip install -U flask-cors

from utils import (
		shaderToy2Snapchat,
		)

DELIM = "-*-*-*-*-*-*-*-*-*-*-*-"

app = Flask(__name__)
CORS(app)


@app.route('/latest_shader',  methods = ['POST'])
def return_latest_shader():
    """
    send latest shader
    """
    with open('latest_shader.txt','r') as f:
        d = '\n'.join(f.readlines())
    return d

@app.route('/update_shader', methods = ['GET','POST'])	
def record_new_shader():
    """
    Record a new shader
    """
    if True:
        key = 'data'
        dat = request.get_data().decode('utf8')
        print(f"\n{DELIM}\nReceived shader is:\n{DELIM}\n")
        print(dat)
        dat = shaderToy2Snapchat(dat)
        print(f"\n{DELIM}\nProcessed shader is:\n{DELIM}\n")
        print(dat)
        with open('latest_shader.txt','w') as f:
            f.write(dat)
    else:
        data = ""
        print('unsuccessfull call')
    data = {"status": "success", "some key":"some value"}
    return data, 200





if __name__ == '__main__':
    app.run(host='0.0.0.0')



