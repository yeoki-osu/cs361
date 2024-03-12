# python -m pip install --upgrade pip --user
# pip install Flask requests
# pip install Flask-CORS
# https://api.nookipedia.com/doc#/paths/~1villagers/get

# python ac_api.py

import requests
from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

BASE_URL = "https://api.nookipedia.com/villagers"
API_KEY = "26b6d7c2-a054-4308-bce3-4f8c956cc675"

def query_characters(species="", gender="", personality="", name=""):
    params = {
        "api_key": API_KEY,
        "species": species,
        "gender": gender,
        "personality": personality,
        "name": name
    }
    response = requests.get(BASE_URL, params=params)
    if response.status_code == 200:
        characters = response.json()
        if gender:
            characters = [character for character in characters if character.get('gender', '').lower() == gender.lower()]
        return characters
    else:
        return []

@app.route('/query', methods=['POST'])
def query():
    request_data = request.json
    species = request_data.get('species', '')
    gender = request_data.get('gender', '')
    personality = request_data.get('personality', '')
    name = request_data.get('name', '')

    characters = query_characters(species, gender, personality, name)
    return jsonify(characters)

if __name__ == '__main__':
    app.run(debug=True)