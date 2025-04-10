from flask import Flask, jsonify
from flask_cors import CORS, cross_origin
from github import Github
from collections import defaultdict
from datetime import datetime
from pytrends.request import TrendReq
import pandas as pd
import math

scraper = Flask(__name__)
CORS(scraper, supports_credentials=True)

@scraper.route('/github', methods=['GET', 'POST'])
@cross_origin()
def github_route():
    g = Github("your_github_token_here")  # Replace with your actual token

    trending_repos = g.search_repositories(query="stars:>1000", sort="stars", order="desc")

    file_extensions = {
        ".py": "python", ".java": "java", ".c": "c", ".cc": "c++", ".cpp": "c++",
        ".h": "c++", ".hpp": "c++", ".js": "javascript", ".html": "html", ".css": "css",
        ".go": "go", ".rb": "ruby", ".php": "php"
    }
    language_list = list(set(file_extensions.values()))

    result = defaultdict(lambda: defaultdict(int))

    for repo in trending_repos[:1]:
        try:
            for commit in repo.get_commits()[:5]:
                commit_date = commit.commit.author.date.date().isoformat()

                # Ensure every language is initialized to 0
                for lang in language_list:
                    result[commit_date][lang] += 0  # Ensures presence without affecting real counts

                try:
                    for file in commit.files:
                        for ext, lang in file_extensions.items():
                            if file.filename.endswith(ext):
                                result[commit_date][lang] += 1
                except Exception:
                    continue
        except Exception:
            continue

    return jsonify(result)

@scraper.route('/google', methods=['GET', 'POST'])
@cross_origin()
def google_route():
    trends = TrendReq(hl='en-US', tz=360)
    language_list = ["python", "java", "c", "c++", "javascript", "html", "css", "go", "ruby", "php"]
    result = defaultdict(lambda: defaultdict(int))
    for lang in language_list:
        try:
            trends.build_payload(kw_list=[lang], timeframe='today 1-week')
            data = trends.interest_over_time()
            data = data.reset_index()
            data['date'] = data['date'].dt.date
            for index, row in data.iterrows():
                try:
                    date_str = row['date'].isoformat()
                    result[date_str][lang] += row[lang]
                except Exception:
                    continue
        except Exception:
            continue

    return jsonify(result)

if __name__ == "__main__":
    scraper.run(port=5555, debug=True)
