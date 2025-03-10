import requests
import json

class TestLSTMNetwork():

    def test_lstm_network(self):
        # Assuming your LSTMNetwork class is defined in lstm_network.py

        response = requests.get('/data/technology/Python')

        # Check the response data
        response_data = json.loads(response.data)
        if response.status_code != 200:
            print("Error: get_data_technology()")
            return False
        
        print("Response data:", response_data)


if __name__ == '__main__':
    test = TestLSTMNetwork()
    test.test_lstm_network()
    print("Test passed")