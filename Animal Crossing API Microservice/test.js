// node test.js

const FLASK_SERVER_URL = 'http://127.0.0.1:5000/query';

async function queryCharacters(data) {
  try {
    const response = await fetch(FLASK_SERVER_URL, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
    });

    if (!response.ok) {
      throw new Error('Network response was not ok');
    }

    const characters = await response.json();
    console.log(characters);
  } catch (error) {
    console.error('Error:', error);
  }
}

const queryData = {
  species: 'wolf',
  gender: 'female',
  personality: 'peppy',
  name: ''
};

queryCharacters(queryData);