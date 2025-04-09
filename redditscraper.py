import praw
import pandas as pd

scraper = Flask(__name__)
CORS(scraper, supports_credentials=True)

@scraper.route('/reddit', methods=['GET', 'POST'])
@cross_origin()
def reddit_route():
    g = Reddit("your_reddit_token_here")

# Authenticate
reddit = praw.Reddit(
    client_id="NzcF0c9Poew9iox42RPZxQ",
    client_secret="XcdcnQ9ZiiL2yb9rH2fWmm-HX6lqSQ",
    user_agent="Tech_Track by u/TechTrack2025"
)

# Search posts by keyword:
for submission in reddit.subreddit("all").search("machine learning", limit=10):
    print(submission.title)


# Scrape and save top posts from r/technology to a CSV for trend prediction
post_data = []
for post in reddit.subreddit("technology").top(time_filter="month", limit=100):
    print(f"Fetching post: {post.title}")  # Debugging line
    post_data.append({
        "title": post.title,
        "score": post.score,
        "created": post.created_utc,
        "num_comments": post.num_comments
    })

# Check if any posts were fetched
if not post_data:
    print("No posts were fetched. Please check the subreddit or parameters.")

# Convert to DataFrame and save to CSV
df = pd.DataFrame(post_data)
df.to_csv("reddit_tech_posts.csv", index=False)

print("Scraping complete. Saved to 'reddit_tech_posts.csv'.")
