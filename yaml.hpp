//
// Created by Zlolka on 03.02.2021.
//

#ifndef ADDONFORAIMP_YAML_HPP
#define ADDONFORAIMP_YAML_HPP

#include <filesystem>
#include <map>
#include <string>
#include <fstream>

using std::ofstream;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;
using std::map;

typedef map<uint, string> indexed_fnames_t;

indexed_fnames_t compare_trees_in_yaml(const string& file_name_1, const string& file_name_2); // TODO

void write_to_yaml_file_directory_tree(const string& path, const string& prev_dir, ofstream& output_file, u_int deep = 0){

    string spaces = "";
    string res = "";
    string spath = "";

    for(int i = 0; i < deep; i++){ // equivalent "  " * (deep + 1)
        spaces.append("  ");
    }

    string name_dir = path.substr(prev_dir.length());
    output_file << spaces << "- " <<
                (name_dir[0] == '/' or name_dir[0] == '\\' ? name_dir.substr(1) : name_dir)
                << ":" << endl;

    for(auto& p: directory_iterator(path)) {
        if(p.is_directory()) [[unlikely]] {
            write_to_yaml_file_directory_tree(p.path().string(), path, output_file, deep + 1); //TODO maybe this is tail call ?
        }else [[likely]]{
            spath = p.path().string().substr(1);
            res = spath.substr(path.length());
            output_file << spaces << "  - \"" << res << '"' << endl;
        }
    }
}
inline void write_to_yaml_file_directory_tree(const string& path, const string& prev_dir, const string& output_fname){
    ofstream output_file(output_fname);
    write_to_yaml_file_directory_tree(path, prev_dir, output_file);
    output_file.close();
}

#endif //ADDONFORAIMP_YAML_HPP
