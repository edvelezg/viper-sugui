#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

void build_map_vector()
{
    uint32_t numcols = 2;
    for(size_t i = 0; i < numcols; ++i)
    {
        map<uint32_t, uint32_t> a_map;
        histo.push_back(a_map);
    }    
}


uint32_t card_at(uint32_t column)
{
    return histo[column].size();
}


int main( )
{
    
    build_map_vector();
    histo[0]["Ding"]++;
    histo[0]["Dog"]++;
    histo[0]["Dirk"]++;
    histo[0]["Rat"]++;
    histo[0]["Dirk"]++;
    
    histo[1]["Dirk"]++;
    histo[1]["Dog"]++;
    histo[1]["Dirk"]++;
    histo[1]["Rat"]++;
    histo[1]["Dirk"]++;


//     map<string,uint> latestmap;
// answer.push_back(latestmap);
//
// for( i = 0; i < count; ++i)
// {
//    /* code */
// }
//
    for (map_vector::const_iterator i = histo.begin(); i!= histo.end(); ++i)
    {
        const map<uint32_t, uint32_t> a_histo = *i;
        for( map<uint32_t, uint32_t>::const_iterator iter = a_histo.begin(); iter != a_histo.end(); iter++ )
        {
            cout << iter->first << "\t" << iter->second << endl;
        }
        cout << endl;
    }
    
    cout << card_at(0);

// strMap[0]
// strMap[0]["Dirk"]++;
// strMap["Monday"] = "Montag";
// strMap["Tuesday"] = "Dienstag";
// strMap["Wednesday"] = "Mittwoch";
// strMap["Thursday"] = "Donnerstag";
// strMap["Friday"] = "Freitag";
// strMap["Saturday"] = "Samstag";
// strMap.insert(make_pair("Sunday", "Sonntag"));
//     strMap.insert(pair<string, int>("Sunday", 1));
//
// strMap["Gum"]++;
// strMap["Sunday"]++;
// strMap["Gum"]++;
// strMap["Sunday"]++;
// for (map<string, int>::iterator p = strMap.begin( );
//     p != strMap.end( ); ++p )
// {
//     cout << p->first << " => Count: " << p->second << endl;
// }
// cout << endl;
// strMap.erase(strMap.find("Tuesday"));
// for (map<string, string>::iterator p = strMap.begin( );
//     p != strMap.end( ); ++p )
// {
//     cout << "English: " << p->first
//         << ", German: " << p->second << endl;
// }
}
