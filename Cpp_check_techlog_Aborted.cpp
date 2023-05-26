#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <map>

#define MAX_LINES 30000

#define DEBUG   true

//#define FIND_TIMEOUT_TEST

using namespace std;

//"D:\work\_DOCS\Notes\46460\46460_techlog.txt"
//zka - TechnologySelectCRTDS.cpp ([....]): Aborted

//D:\work\_DOCS\Logs\ZPD06591\ZPD06591.txt

//C:\Users\Administrator\Documents\Engineering\Software\Cpp_language\Cpp_check_techlog\DKV_Aborted.txt


vector<string> DataType;

//string str_to_find_debug_debug = "zka - TechnologySelectCRTDS.cpp [0-9]+: Aborted";
//string str_to_find_debug = ": Aborted";
//string str_to_find_debug = "transparent mode";
//string template_for_search = "zka - TechnologySelectCRTDS.cpp (): Aborted";

string str_to_find_debug = "Track2:";

void search_template_in_file(string file_out_path, string &str_search,
                             string * arr_of_lines, int lines);
int load_data_in_arr(string f_in_path, string * arr_of_lines, int arr_of_lines_size);
int load_data_in_map(string f_in_path, map<int, string> &map_of_lines);
void print_array(string * arr, int arr_size);
int load_transactions_in_map(map<int, string>  &map_of_lines, map<int, map<int, string> > &transactions);
bool find_timeout(const string first, const string second);

int main() {
//    int lines = 0;
    string search_str, file_in_path, file_out_path, arr_of_lines[MAX_LINES];
    map<int, string> *map_of_lines = new map<int, string>();
    map<int, map<int, string> > *transactions = new map<int, map<int, string> >();

#ifdef FIND_TIMEOUT_TEST
    string second = "2023-03-02 00:00:40.82 zka - MainLoop.cpp (3448)";
    string first = "2023-03-02 23:59:40.82 zka - MainLoop.cpp (3448)";

    find_timeout(first, second);
    return 0;
#endif

    if(DEBUG){
//        file_in_path = "C:\\Users\\Administrator\\Documents\\Engineering\\Software\\Cpp_language\\Cpp_check_techlog\\ZPD01008.txt";
        file_in_path = "D:\\work\\_DOCS\\Tickets_log\\53273.txt";
        file_out_path = "Output_file.txt";

        cout<<"Input the file name with path : "<<file_in_path<<endl;
        cout<<"Input the file name with path : "<<file_out_path<<endl;

        search_str = str_to_find_debug;
    }
    else{
        cout<<"Input the file name with path : ";
        cin>>file_in_path;

        cout<<"Output file name : ";
        cin>>file_out_path;
        cout<<"Input a string for search : ";
        cin>>search_str;
    }

//    lines = load_data_in_arr(file_in_path, arr_of_lines, MAX_LINES);
//
//    print_array(arr_of_lines, lines);

    load_data_in_map(file_in_path, *map_of_lines);

    load_transactions_in_map(*map_of_lines, *transactions);

//    for(map<int, string>::iterator i = map_of_lines->begin(); i != map_of_lines->end(); i++){
////        file_out<<j<<endl;
//        cout<<i->first<<" : "<<i->second<<endl;
//
//    }
//    search_template_in_file(file_out_path, search_str, arr_of_lines, lines);

//    if (regex_match ("softwareTesting", regex("(soft)(.*)") ))
//      cout << "string:literal => matched\n";

    return 0;
}
void search_template_in_file(string file_out_path, string &str_search,
									string * arr_of_lines, int lines){
    ofstream file_out;
    file_out.open(file_out_path, ios::out);

    vector<string> data;
//    string str_line, arr_of_lines[MAX_LINES];

    for (int counter = 0; counter < lines; counter++){
        size_t found = arr_of_lines[counter].find(str_search);
        if(found != string::npos){
            data.push_back(arr_of_lines[counter]);
//            cout<<arr_of_lines[counter]<<endl;
        }
    }
    cout<<"We search : "<<str_search<<endl;
    for(auto j:data){
        file_out<<j<<endl;
        cout<<j<<endl;
    }
}
int load_data_in_arr(string f_in_path, string * arr_of_lines, int arr_of_lines_size){
    ifstream file_in;
    file_in.open(f_in_path, ios::in);
    string str_line;

    int elements_nr=0;
    while(!file_in.eof()) {
        getline(file_in, str_line);
        arr_of_lines[elements_nr]=str_line;
        elements_nr++;
    }
    return elements_nr;
}
void print_array(string * arr, int arr_size){
    cout<<"Print Array"<<endl;
    for(int counter = 0; counter < arr_size; counter++){
        cout<<arr[counter]<<endl;
    }
}
int load_data_in_map(string f_in_path, map<int, string> &map_of_lines){
    ifstream file_in;
    file_in.open(f_in_path, ios::in);
    string str_line;

    int elements_nr=0;
    while(!file_in.eof()) {
        getline(file_in, str_line);
        map_of_lines[elements_nr]=str_line;
        elements_nr++;
    }
    return elements_nr;
}
int load_transactions_in_map(map<int, string>  &map_of_lines, map<int, map<int, string> > &transactions){
    int transaction_nr = 0;
//    string str_search = ":0601";
    string str_search = "): Aborted";
    string key_stop = "KEY_STOP";
    size_t nmr_Aborted = 0;
    size_t nmr_Aborted_timeouts = 0;

    for(map<int, string>::iterator i = map_of_lines.begin(),
        prev = map_of_lines.end(); i != map_of_lines.end(); prev = i, ++i){

//        size_t found = i->second.find(str_search);
//        if(found != string::npos){
        if(i->second.find(str_search)!= string::npos){
//            cout<<"-----------------------------------------------------------------------"<<endl;
            if(!(prev->second.find(key_stop) != string::npos)){

                cout<<"***********************************************************************"<<endl;

                if(find_timeout(prev->second, i->second))
                    nmr_Aborted_timeouts++;

                cout<<prev->first <<" : "<<prev->second<<endl;
                cout<<i->first<<" : "<<i->second<<endl;
                nmr_Aborted++;
            }
        }
    }
    cout<<str_search + " all         = "<<nmr_Aborted<<endl;
    cout<<str_search + " by timeouts = "<<nmr_Aborted_timeouts<<endl;

    return 0;
}
bool find_timeout(const string first, const string second){
    string hours_1, hours_2, minutes_1, minutes_2, seconds_1, seconds_2, result;
    result = second.substr(14, 5);

    hours_1   = first.substr(11, 2);
    minutes_1 = first.substr(14, 2);
    seconds_1 = first.substr(17, 2);
    hours_2   = second.substr(11, 2);
    minutes_2 = second.substr(14, 2);
    seconds_2 = second.substr(17, 2);

#ifdef FIND_TIMEOUT_TEST
    cout<<hours_1<<" "<<minutes_1<<" "<<seconds_1<<endl;
    cout<<hours_2<<" "<<minutes_2<<" "<<seconds_2<<endl;
#endif // FIND_TIMEOUT_TEST

    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    int timeout = 0;

    hours   = stoi(hours_2) - stoi(hours_1);
    if(hours < 0)
        hours = 1;

    minutes = stoi(minutes_2) - stoi(minutes_1);
    seconds = stoi(seconds_2) - stoi(seconds_1);

    timeout = hours*60*60 + minutes*60 + seconds;
    if(timeout > 28){
        cout<<" Timeout = "<<timeout<<endl;
        return true;
    }
    return false;
}

