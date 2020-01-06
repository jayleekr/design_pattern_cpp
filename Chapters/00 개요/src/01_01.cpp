#include <string>
#include <vector>
#include <fstream> // ofstream

using namespace std;

struct Journal
{
    string title;
    vector<string> entries;

    explicit Journal(const string& title) : title(title) {}
    
    void Journal::add(const string& entry)
    {
        static int count = 1;
        entries.push_back(boost::lexical_cast<string>(count ++) + ": " + entry);
    }

    void Journal::save(const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s : entries)
        {
            ofs<< s << endl;
        }
    }
};

struct PersistenceManager
{
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s: j.entries)
            ofs << s << endl;
    }
};