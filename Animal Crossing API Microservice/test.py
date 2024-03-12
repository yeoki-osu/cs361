# python test.py
# curl -X POST -H "Content-Type: application/json" -d '{"species": "", "gender": "", "personality": "", "name": ""}' http://127.0.0.1:5000/query

import requests
import json

def query_characters(json_data):
    url = "http://127.0.0.1:5000/query"
    headers = {"Content-Type": "application/json"}
    response = requests.post(url, headers=headers, data=json.dumps(json_data))
    if response.status_code == 200:
        return response.json()
    else:
        return []

def main():
    species = "wolf"
    gender = "female"
    personality = "peppy"
    name = ""

    json_data = {
        "species": species,
        "gender": gender,
        "personality": personality,
        "name": name
    }

    characters = query_characters(json_data)
    print(characters)

if __name__ == "__main__":
    main()