# Simple Search Engine
It uses the connections between the websites to construct a web graph.
Then, it starts calculating the rank of each page using page rank. Moreover, the projects reads the number of
impressions (the number of times the web page was displayed as a search result) and clicks (the number of
times the web page was clicked by the user) from CSV files to calculate the score of each page using CTR
(click-through rate). It also reads the keywords of each website and map it. The project basically receives a
search query from the user; it may contain ’AND’, ’OR’, or ’"’ (quotation marks). The project start searching
for the keyword and return the websites associated with it. The next step is to sort the websites according to
their scores and update both the impressions and clicks. At the end of each session, the program saves the
new data into the CSV files so it can access them later on.
