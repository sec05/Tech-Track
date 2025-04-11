import praw
import pandas as pd
from datetime import datetime
from datetime import timezone



scraper = Flask(__name__)
CORS(scraper, supports_credentials=True)

@scraper.route('/reddit', methods=['GET', 'POST'])
@cross_origin()
def reddit_route():
    g = Reddit("your_reddit_token_here")


# Reddit API setup 
reddit = praw.Reddit(
    client_id='NzcF0c9Poew9iox42RPZxQ',
    client_secret='XcdcnQ9ZiiL2yb9rH2fWmm-HX6lqSQ',
    user_agent='Tech Track by u/TechTrack2025'
)

# Keywords and subreddits to search
languages = ["python", "java", "c", "c++", "javascript", "html", "css", "go", "ruby", "php"]
subreddits = ["programming", "learnprogramming", "coding", "webdev", "cscareerquestions"]

# Data collection
print("Fetching data from Reddit...")
data = []

for lang in languages:
    for sub in subreddits:
        try:
            for post in reddit.subreddit(sub).search(lang, time_filter='year', sort='new', limit=200):
                data.append({
                    "language": lang.lower(),
                    "subreddit": sub,
                    "created": datetime.fromtimestamp(post.created_utc, tz=timezone.utc)
                })
        except Exception as e:
            print(f"Error with r/{sub} on '{lang}': {e}")

# Convert to DataFrame
df = pd.DataFrame(data)
df["year"] = pd.to_datetime(df["created"]).dt.year

# Group by year + language
yearly_trends = df.groupby(["year", "language"]).size().unstack(fill_value=0)

# View individual language trend
choice = input("\nEnter a programming language to see its yearly trend (or press Enter to skip): ").strip().lower()
if choice and choice in yearly_trends.columns:
    print(f"\nYearly trend for '{choice}':\n")
    print(yearly_trends[[choice]])

