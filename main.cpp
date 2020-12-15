#include <iostream>
#include <fstream>
#include <filesystem>

//TODO #include <boost/asio.hpp>

using std::ofstream;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;

typedef uint port_t; //TODO
typedef uint ip_t;   //TODO

//string root_dir = "C:/Music/Music/Music";
const string root_dir_name = "C:/Music/";       // | TODO _GUI implement a GUI to enter this data
const string output_file_name = "../test.yaml"; // | TODO _GUI
const port_t port = 65;                         // | TODO _GUI

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

ip_t get_client_ip(){
    /*TODO
     *  ip my_ip = get_my_own_ip();
     *  send_broadcast_tcp_request(data=my_ip, port=port);
     *  ip client_ip = wait_for_the_client_tcp_response(port=port); // get client ip address
     *  return client_ip
     * */
}

uint get_number_of_files_client_needs(ip_t client_ip) {
    /*TODO
     *  send_file_via_ftp(to=client_ip, port=port, file=out);
     *  uint number_files_client_needs = wait_for_the_client_tcp_response(port=port);
     *  return number_files_client_needs;
     */
}

void send_necessary_files_to_client(ip_t client_ip, uint number_files){
    /*TODO
     *  string file_name;
     *  for(uint files_left = number_files; files_left > 0; files_left--){
     *      send_data_via_tcp(to=client_ip, port=port, data=files_left);
     *      file_name = wait_for_client_tcp_response(port=port); // get name remaining file with number files_left
     *      send_file_via_ftp(to=client_ip, port=port, file=file_name);
     *  }
     * */
}

int main() {
    ofstream output_file(output_file_name);
    write_to_yaml_file_directory_tree(root_dir_name, root_dir_name, output_file);
    output_file.close();
    bool im_server = true; //TODO _GUI selected in GUI
    if(im_server) {
        ip_t client_ip = get_client_ip();
        uint number_files_client_needs = get_number_of_files_client_needs(client_ip);
        if (number_files_client_needs > 0) {
            send_necessary_files_to_client(client_ip, number_files_client_needs);
        }
    }else{

    }
    return 0;
}
