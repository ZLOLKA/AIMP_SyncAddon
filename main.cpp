#include <iostream>
#include <fstream>
#include <filesystem>

using std::ofstream;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;

//string root_dir = "C:/Music/Music/Music";
const string root_dir = "C:/Music/";
ofstream out("../test.yaml");

void iter(const string& path, const string& prev_dir, u_int deep = 0){
    string spaces = "";
    string res = "";
    string spath = "";

    for(int i = 0; i < deep; i++){ // equivalent "  " * (deep + 1)
        spaces.append("  ");
    }

    string name_dir = path.substr(prev_dir.length());
    out << spaces << "- " <<
            (name_dir[0] == '/' or name_dir[0] == '\\' ? name_dir.substr(1) : name_dir)
         << ":" << endl;

    for(auto& p: directory_iterator(path)) {
        if(p.is_directory()) [[unlikely]] {
            iter(p.path().string(), path, deep + 1);
        }else [[likely]]{
            spath = p.path().string().substr(1);
            res = spath.substr(path.length());
            out << spaces << "  - \"" << res << '"' << endl;
        }
    }
}

int main() {
    iter(root_dir, root_dir);
    return 0;
}
