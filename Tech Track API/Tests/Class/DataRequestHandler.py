import requests
import json

class DataRequestHandler:
    def __init__(self, url):
        self.url = url

    def get_data_technology(self,technology):
        response = requests.get(self.url+"technology/"+technology)
        if response.status_code != 200:
            print("Error: get_data_technology(",technology,")", response.status_code)
            return None
        return response.text

    def get_data_company_technology(self, technology, company):
        response = requests.get(self.url+company+"/"+technology)
        if response.status_code != 200:
            print("Error: get_data_company_technology(",technology,",",company,")", response.status_code)
            return None
        return response.text
    
def driver():
    url = "http://127.0.0.1:8008/data/"
    drh = DataRequestHandler(url)
    try:
        json.loads(drh.get_data_technology("Python"))
    except:
        print("Error: get_data_technology returned invalid JSON")
    try:
        json.loads(drh.get_data_company_technology("Python", "Google"))
    except:
        print("Error: get_data_company_technology returned invalid JSON")
    print("DataRequestHandler driver passed")

if __name__ == "__main__":
    driver()