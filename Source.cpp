#include "Trie.cpp"
#include "Graph.cpp"

#define MAX_LENGTH 1000

struct Score
{
    int index;
    float score;
    Score(int i, float s)
    {
        index = i;
        score = s;
    }
};

Trie* keyword_cased_trie = new Trie();
Trie* keyword_uncased_trie = new Trie();
map<string, int> webpages;
map<float, string> sWebpages;
vector<float> scores;
vector<int> clks;
vector<int> imps;
Graph webgraph;

void init()
{
    string link = "", line = "", str = "";
    fstream Keywords;
    fstream web_graph;
    fstream clicks;
    fstream impressions;
    Keywords.open("CSVs/key_words.csv", ios::out | ios::in);
    web_graph.open("CSVs/web_graph.csv", ios::out | ios::in);
    clicks.open("CSVs/clicks.csv", ios::out | ios::in);
    impressions.open("CSVs/Impressions.csv", ios::out | ios::in);

    int c = 0;
    while (getline(Keywords, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        webpages[link] = c;
        for (int i = 0; i < MAX_LENGTH; i++) {
            getline(iss, str, ',');
            if (str.size() == 0)
            {
                break;
            }
            keyword_cased_trie->insert(str, link, true);
            keyword_uncased_trie->insert(str, link, false);
        }
        c++;

    }

    vector<Edge> edges;
    while (getline(web_graph, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        getline(iss, str, ',');
        edges.push_back(Edge(webpages[link], webpages[str]));
    }
    webgraph = Graph(edges, c, 0.85, 0.1);
    webgraph.pagerank();
    edges.clear();

    while (getline(clicks, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        getline(iss, str, ',');
        if (str.size() == 0)
        {
            break;
        }
        clks.push_back(stoi(str));
    }

    while (getline(impressions, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        getline(iss, str, ',');
        if (str.size() == 0)
        {
            break;
        }
        imps.push_back(stoi(str));
    }

    Keywords.close();
    Keywords.open("CSVs/key_words.csv", ios::out | ios::in);

    int i = 0;
    while (getline(Keywords, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        float score = 0.4 * webgraph.pr[i];
        float p1 = 1 - ((0.1 * imps[i]) / (1 + (0.1 * imps[i])));
        p1 *= webgraph.pr[i];
        float p2 = ((0.1 * imps[i]) / (1 + (0.1 * imps[i])));
        float ctr = clks[i] / imps[i];
        p2 *= ctr;
        float z = (p1 + p2) * 0.6;
        score += z;
        scores.push_back(score);
        sWebpages[score] = link;
        i++;
    }

    web_graph.close();
    Keywords.close();
    clicks.close();
    impressions.close();
}

void update_impressions(vector<string> data)
{
    int n = data.size();
    for (int i = 0; i < n; i++) {
        imps[webpages[data[i]]]++;
    }
    string link = "", line = "", str = "";
    fstream impressions;
    impressions.open("CSVs/Impressions.csv", ios::out | ios::in);

    vector<string> tmp;
    while (getline(impressions, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        getline(iss, str, ',');
        str = to_string(imps[webpages[link]]);
        tmp.push_back(link + "," + str);
    }
    impressions.close();

    impressions.open("CSVs/Impressions.csv", ios::out | ios::trunc);
    map<string, int>::iterator it;
    for (int i = 0; i < tmp.size(); i++)
    {
        impressions << tmp[i] << endl;
    }
    tmp.clear();
    impressions.close();
}

void update_clicks(string website)
{
    string link = "", line = "", str = "";
    clks[webpages[website]]++;
    fstream clicks;
    clicks.open("CSVs/clicks.csv", ios::out | ios::in);

    vector<string> tmp;
    while (getline(clicks, line)) {
        stringstream iss(line);
        getline(iss, link, ',');
        getline(iss, str, ',');
        if (website == link)
        {
            str = to_string(clks[webpages[link]]);
        }
        tmp.push_back(link + "," + str);
    }
    clicks.close();

    clicks.open("CSVs/clicks.csv", ios::out | ios::trunc);
    map<string, int>::iterator it;
    for (int i = 0; i < tmp.size(); i++)
    {
        clicks << tmp[i] << endl;
    }
    tmp.clear();
    clicks.close();
}

vector<string> and_search(string query)
{
    string whole = "";
    stringstream iss(query);
    string key;
    vector<vector<string>> data;
    vector<string> common;
    int i = 1;
    while (iss >> key) {
        if (key == "AND")
        {
            continue;
        }
        else
        {
            if (i == 1)
            {
                whole = whole + key + " ";
                i++;
            }
            else
            {
                whole += key;
            }
            data.push_back(keyword_uncased_trie->search(key, false));
        }
    }
    data.push_back(keyword_uncased_trie->search(whole, false));

    vector<string> stick;
    set_intersection(data[0].begin(), data[0].end(), data[1].begin(), data[1].end(), back_inserter(common));
    set_intersection(common.begin(), common.end(), data[2].begin(), data[2].end(), back_inserter(stick));

    return stick;
}

vector<string> or_search(string query)
{
    string whole = "";
    stringstream iss(query);
    string key;
    vector<vector<string>> data;
    vector<string> both;
    int i = 1;
    while (iss >> key) {
        if (key == "OR")
        {
            continue;
        }
        else
        {
            if (i == 1)
            {
                whole = whole + key + " ";
                i++;
            }
            else
            {
                whole += key;
            }
            data.push_back(keyword_uncased_trie->search(key, false));
        }
    }
    vector<string> stick = keyword_uncased_trie->search(whole, false);

    both.insert(both.begin(), data[0].begin(), data[0].end());
    both.insert(both.end(), data[1].begin(), data[1].end());
    both.insert(both.end(), stick.begin(), stick.end());

    unordered_set<string> s(both.begin(), both.end());
    both.assign(s.begin(), s.end());

    return both;
}
vector<string> qou_search(string query)
{
    query.erase(query.begin());
    query.erase(query.end() - 1);
    return keyword_cased_trie->search(query, true);
}

vector<string> search(string query)
{
    stringstream iss(query);
    string key;
    if (query[0] == '"')
    {
        return qou_search(query);
    }
    int c = 0;
    while (iss >> key) {
        if (key == "AND")
        {
            return and_search(query);
        }
        else if (key == "OR")
        {
            return or_search(query);
        }
        c++;
    }
    if (c > 1)
    {
        return or_search(query);
    }
    return keyword_uncased_trie->search(query, false);
}


// Driver
int main()
{
    init();

    bool close = false;
    int x;
    string queury;
    cout << "Welcome!" << endl;
    cout << "What do you like to do?" << endl;
    cout << "--------------------------------" << endl;
    while (!close)
    {
    home:
        cout << "1. New search" << endl;
        cout << "2. Exit" << endl;
        cout << "--------------------------------" << endl;
        cout << "Choice : ";
        cin >> x;
        cout << "--------------------------------" << endl;
        if (x == 1)
        {
        search:
            cout << "Your search query : ";
            getline(cin, queury);
            getline(cin, queury);
            vector<string> results = search(queury);
            if (results.size() == 0)
            {
                cout << "--------------------------------" << endl;
                cout << "Not found" << endl;
                cout << "--------------------------------" << endl;
                goto home;
            }
            vector<float> tmp;
            for (int i = 0; i < results.size(); i++)
            {
                tmp.push_back(scores[webpages[results[i]]]);
            }
            make_heap(tmp.begin(), tmp.end());
        results:
            cout << "--------------------------------" << endl;
            cout << "Your search results:" << endl;
            cout << "--------------------------------" << endl;
            for (int i = 0; i < results.size(); i++)
            {
                cout << i + 1 << ". " << sWebpages[tmp[i]] << endl;
            }
            cout << "--------------------------------" << endl;
            cout << "1. Open one of them" << endl;
            cout << "2. New search" << endl;
            cout << "3. Exit" << endl;
            cout << "--------------------------------" << endl;
            update_impressions(results);
            cout << "Choice : ";
            cin >> x;
            if (x == 1)
            {
                cout << "--------------------------------" << endl;
                cout << "Which one to open? ";
                cin >> x;
                cout << "--------------------------------" << endl;
                update_clicks(sWebpages[tmp[x - 1]]);
                cout << "Your are now viewing : " << sWebpages[tmp[x - 1]] << endl << endl;
                cout << "1. Back to search results" << endl;
                cout << "2. New search" << endl;
                cout << "3. Exit" << endl;
                cout << "--------------------------------" << endl;
                cout << "Choice : ";
                cin >> x;
                if (x == 1)
                {
                    goto results;
                }
                else if (x == 2)
                {
                    goto search;
                }
                else
                {
                    close = true;
                }
            }
            else if (x == 2)
            {
                goto search;
            }

            else
            {
                close = true;
            }
        }
        else
        {
            close = true;
        }
    }
    cout << "--------------------------------" << endl;
    cout << "Thanks." << endl;

    return 0;
}