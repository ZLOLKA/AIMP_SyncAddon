#include <iostream>
#include <fstream>
#include <filesystem>

//TODO #include <boost/asio.hpp>

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

typedef int ip; //TODO
ip find_client_ip(){
    /*TODO
     *  ip my_ip = get_my_own_ip();
     *  send_broadcast_tcp_request(data=my_ip);
     *  ip client_ip = wait_for_the_client_tcp_response(); // get client ip address
     *  return client_ip
     * */
}

uint exchanging_tree_with_client(ip client_ip) {
    /*TODO
     *  send_file_via_ftp(to=client_ip, file=out);
     *  uint number_files_client_needs = wait_for_the_client_tcp_response();
     *  return number_files_client_needs;
     */
}

void give_client_necessary_files(ip client_ip, uint number_files){
    /*TODO
     *  string file_name;
     *  for(uint files_left = number_files; files_left > 0; files_left--){
     *      send_data_via_tcp(to=client_ip, data=files_left);
     *      file_name = wait_for_client_tcp_response(); // get name remaining file with number files_left
     *      send_file_via_ftp(to=client_ip, file=file_name);
     *  }
     * */
}

int main() {
    iter(root_dir, root_dir);
    bool im_server = true; //TODO selected in GUI
    if(im_server) {
        ip client_ip = find_client_ip();
        uint number_files_client_needs = exchanging_tree_with_client(client_ip);
        if (number_files_client_needs > 0) {
            give_client_necessary_files(client_ip, number_files_client_needs);
        }
    }else{

    }
    return 0;
}
