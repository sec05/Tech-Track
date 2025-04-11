from flask import Flask, jsonify
from flask_cors import CORS, cross_origin
from github import Github
from collections import defaultdict
from datetime import datetime
from pytrends.request import TrendReq
import pandas as pd
import math
import praw
from datetime import timezone
from time import sleep

scraper = Flask(__name__)
CORS(scraper, supports_credentials=True)

@scraper.route('/github', methods=['GET', 'POST'])
@cross_origin()
def github_route():
    g = Github("")  # Replace with your actual token

    trending_repos = g.search_repositories(query="stars:>1000", sort="stars", order="desc")

    file_extensions = {
        ".py": "python", ".java": "java", ".c": "c", ".cc": "c++", ".cpp": "c++",
        ".h": "c++", ".hpp": "c++", ".js": "javascript", ".html": "html", ".css": "css",
        ".go": "go", ".rb": "ruby", ".php": "php"
    }
    language_list = list(set(file_extensions.values()))

    result = defaultdict(lambda: defaultdict(int))

    for repo in trending_repos[:10]:
        try:
            for commit in repo.get_commits()[:100]:
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
        sleep(5)
        try:
            trends.build_payload(kw_list=[lang], timeframe='now 7-d')
            data = trends.interest_over_time()

            if data.empty or 'isPartial' not in data.columns:
                continue  # Skip if no data

            data = data.reset_index()
            data['date'] = data['date'].dt.date

            for index, row in data.iterrows():
                date_str = row['date'].isoformat()
                result[date_str][lang] += int(row.get(lang, 0))
            result[date_str][lang] //= 100  # Normalize
        except Exception as e:
            print(f"[ERROR] Failed to process keyword '{lang}': {e}")
            continue

    # Convert defaultdict to dict for jsonify compatibility
    result_dict = {date: dict(lang_data) for date, lang_data in result.items()}
    return jsonify(result_dict), 200

@scraper.route('/reddit', methods=['GET', 'POST'])
@cross_origin()
def reddit_route():
    languages = ["python", "java", "c", "c++", "javascript", "html", "css", "go", "ruby", "php"]
    subreddits = ["programming", "learnprogramming", "coding", "webdev", "cscareerquestions"]

    reddit = praw.Reddit(
        client_id='NzcF0c9Poew9iox42RPZxQ',
        client_secret='XcdcnQ9ZiiL2yb9rH2fWmm-HX6lqSQ',
        user_agent='Tech Track by u/TechTrack2025'
    )

    result = defaultdict(lambda: defaultdict(int))

    for lang in languages:
        for sub in subreddits:
            try:
                for post in reddit.subreddit(sub).search(lang, time_filter='week', sort='new', limit=10):
                    created_date = datetime.fromtimestamp(post.created_utc, tz=timezone.utc).date().isoformat()
                    result[created_date][lang] += 1
            except Exception as e:
                print(f"[ERROR] r/{sub}, lang={lang} → {e}")
                continue

    # Convert nested defaultdicts to dicts
    final_result = {date: dict(lang_data) for date, lang_data in result.items()}
    return jsonify(final_result), 200

if __name__ == "__main__":
    scraper.run(port=5555, debug=True)
