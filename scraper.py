from flask import Flask, request, jsonify, Response
from flask_cors import CORS, cross_origin
from github import Github

# create a Flask application
scraper = Flask(__name__)
CORS(scraper, supports_credentials=True) # enable CORS for all routes

# define route for collecting GitHub data
@scraper.route('/github', methods=['GET', 'POST'])
@cross_origin()
def github():
    # create a Github instance using a personal access token
    g = Github("")
    # sort by trending repositories
    # will max out at 1000
    treningRepos = g.search_repositories(query="stars:>1000", sort="stars", order="desc")
    # create a list of file extensions to filter
    fileExtensions = [".py", ".java", ".c", ".cc", ".cpp",".h",".hpp",".js",".html",".css",".go",".rb",".php"]
    # make a dictionary to store the data
    data = {}
    for repo in treningRepos:
        # collect all commits for the repositories
        for commit in repo.get_commits():
            # get when commit was added to repo
            commit_date = commit.commit.author.date
            # if the file parsed has an extension in the list, store it in the database
            for file in commit.files:
                if file.filename.endswith(tuple(fileExtensions)):
                    data.update({commit_date: {file.filename: file.changes}})

# nested dictionary: dates as keys, technology and count as values

if __name__ == "__main__":
    scraper.run(port=5555, debug=True)