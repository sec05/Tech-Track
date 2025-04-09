import praw
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.decomposition import LatentDirichletAllocation
from statsmodels.tsa.arima.model import ARIMA

scraper = Flask(__name__)
CORS(scraper, supports_credentials=True)

@scraper.route('/reddit', methods=['GET', 'POST'])
@cross_origin()
def reddit_route():
    g = Reddit("your_reddit_token_here")

# Step 1: Scrape Reddit Data
# Initialize PRAW for Reddit API
reddit = praw.Reddit(
    client_id="NzcF0c9Poew9iox42RPZxQ",
    client_secret="XcdcnQ9ZiiL2yb9rH2fWmm-HX6lqSQ",
    user_agent="Tech_Track by u/TechTrack2025"
)

# Collect top posts from r/technology
post_data = []
for post in reddit.subreddit("technology").top(time_filter="all", limit=100):
    post_data.append({
        "title": post.title,
        "score": post.score,
        "created": post.created_utc,
        "num_comments": post.num_comments
    })

# Step 2: Preprocess Data
df = pd.DataFrame(post_data)
df["created_dt"] = pd.to_datetime(df["created"], unit="s")
df["year_month"] = df["created_dt"].dt.to_period("M")

# Step 3: Define Custom Stopwords
stop_words = [
    "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", 
    "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", 
    "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", 
    "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", 
    "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", 
    "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", 
    "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", 
    "against", "between", "into", "through", "during", "before", "after", "above", 
    "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", 
    "again", "further", "then", "once", "here", "there", "when", "where", "why", 
    "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", 
    "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", 
    "s", "t", "can", "will", "just", "don", "should", "now"
]

# Step 4: Keyword Frequency Analysis
keywords = ["AI", "blockchain", "machine learning", "quantum", "cloud"]
keyword_counts = {keyword: [] for keyword in keywords}

# Count keyword mentions over time
for keyword in keywords:
    for period, group in df.groupby("year_month"):
        count = group["title"].str.contains(keyword, case=False).sum()
        keyword_counts[keyword].append(count)

# Convert period to datetime for plotting
trend_data = pd.DataFrame(keyword_counts, index=pd.to_datetime(df["year_month"].unique().astype(str)))

# Plot the keyword trends over time
trend_data.plot(kind="line", figsize=(10, 6))
plt.title("Tech Trends Over Time")
plt.xlabel("Time")
plt.ylabel("Mentions per Month")
plt.show()

# Step 5: Topic Modeling (LDA)
# Vectorize the titles using TF-IDF
vectorizer = TfidfVectorizer(stop_words=stop_words, max_features=1000)
X = vectorizer.fit_transform(df["title"])

# Apply LDA (Latent Dirichlet Allocation) for topic modeling
lda = LatentDirichletAllocation(n_components=5, random_state=42)
lda.fit(X)

# Print top words for each topic
terms = vectorizer.get_feature_names_out()
for idx, topic in enumerate(lda.components_):
    print(f"Topic #{idx}:")
    print([terms[i] for i in topic.argsort()[-10:]])
    print("-" * 40)

# Step 6: Time Series Prediction (ARIMA)
# Let's predict "AI" mentions over time
ai_mentions = df[df["title"].str.contains("AI", case=False)].groupby("year_month").size()

# Convert PeriodIndex to DateTime for ARIMA
ai_mentions.index = ai_mentions.index.to_timestamp()

# Fit the ARIMA model
model = ARIMA(ai_mentions, order=(5, 1, 0))  # You can adjust the order (p, d, q)
model_fit = model.fit()

# Forecast the next 12 months
forecast = model_fit.forecast(steps=12)

# Plot actual vs. forecasted mentions
plt.figure(figsize=(10, 6))
plt.plot(ai_mentions, label="Actual AI Mentions")
plt.plot(pd.date_range(ai_mentions.index[-1], periods=13, freq="M")[1:], forecast, label="Forecasted AI Mentions", linestyle='--')
plt.legend()
plt.title("AI Mentions Forecast")
plt.show()

# Step 7: Save Data and Predictions
# Save the tech post data to CSV
df.to_csv("reddit_tech_posts.csv", index=False)

# Save the forecasted data to CSV
forecast_df = pd.DataFrame({"Forecasted AI Mentions": forecast}, index=pd.date_range(ai_mentions.index[-1], periods=13, freq="M")[1:])
forecast_df.to_csv("forecasted_ai_mentions.csv", index=True)
