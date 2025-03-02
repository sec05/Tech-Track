import requests

with open("config", "r") as f:
    config = f.read().split("\n")
    technologies = config[0].split("Technologies:")[1].split(",")
    companies = config[1].split("Companies:")[1].split(",")
    for i in range(len(technologies)):
        technologies[i] = technologies[i].strip()
    for i in range(len(companies)):
        companies[i] = companies[i].strip()
    
    for technology in technologies:
        for company in companies:
            url = f"http://localhost:8008/data/{company}/{technology}"
            response = requests.get(url)
            if response.status_code == 200:
                print(f"Success: {url}")
            else:
                print(f"Failure: {url}")
    for technology in technologies:
        url = f"http://localhost:8008/data/technology/{technology}"
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Success: {url}")
        else:
            print(f"Failure: {url}")
    