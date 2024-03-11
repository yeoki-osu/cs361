# CS361 - Communication Contract
> Running the Microservice
1. Download the ac_api.py file and place it somewhere in your local environment
2. Open a console or a terminal
3. Install "pip" and "Flask requests" if they're not already installed in your HOME directory
- [ ] Download Commands:
- python -m pip install --upgrade pip --user
- pip install Flask requests
5. Navigate to the directory of the ac_api.py file
6. Launch the microservice by typing in "python ac_api.py"
> Requesting Data
- [ ] OPTION 1: CURL terminal command
1. Open a new terminal or console and enter in the following command (insert query values in between "" marks):
- curl -X POST -H "Content-Type: application/json" -d '{"species": "", "gender": "", "personality": "", "name": ""}' http://127.0.0.1:5000/query
- [ ] OPTION 2: Direct integration into existing code
1. Use the test.py or test.js file as a template to insert the functionality for storing the return data from the API directly into the existing implementation
- For using the test.py or test.js file, navigate to the directory of the file location and type in "python test.py" or "node test.js". To change the test query values, go into the code and change the values in main() or the const JSON struct
> Receiving Data
- [ ] OPTION 1: CURL terminal command
- This option automatically returns the API data directly to the console/terminal. If needed, the output data can be redirected and captured into a variable.
- [ ] OPTION 2: Direct integration into existing code
- As long as the integration or the test.py/test.js is ran correctly, the data is automatically stored into a JSON variable (currently set to print to the terminal for testing purposes).
> UML Sequence Diagram
![S2 6 UML Sequence Diagram](https://github.com/yeoki-osu/cs361/assets/102574539/9e131a30-9e61-4cde-be7c-e4ee0733a8de)
