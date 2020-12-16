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

struct number_and_file_struct{
    uint number_files_client_needs;
    ofstream file_compare_tree;
};

//string root_dir = "C:/Music/Music/Music";
const string root_dir_name = "C:/Music/";       // | TODO _GUI implement a GUI to enter this data
const string output_file_name = "../test.yaml"; // | TODO _GUI
const string file_name_of_server_tree = "../*.yaml"; // | TODO _GUI
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

ip_t get_server_ip(){
    /*TODO
     *  ip_t my_ip = get_my_own_ip();
     *  ip_t server_ip = listen_as_tcp_until_get_server_ip(port=port);
     *  send_data_via_tcp(to=server_ip, port=port, data=my_ip);
     *  return server_ip;
     */
}

number_and_file_struct send_and_return_number_and_files_of_files_client_needs(ip_t server_ip, const string& file_name_of_our_tree){
    /*TODO
     *  ??ofstream?? server_tree = listen_as_ftp_until_get_server_directory_tree(port=port);
     *  number_and_file_struct number_and_file = compare_trees_in_yaml(file_name_of_server_tree, file_name_of_our_tree);
     *  uint number_files_client_need = number_and_file.number_files_client_needs;
     *  send_data_via_tcp(to=server_ip, port=port, data=number_files_client_need);
     *  return number_and_file;
     */
}

void get_files_need_by_client_from_server(ip_t server_ip, number_and_file_struct& number_and_file){
    /*TODO
     *  uint number_files = number_and_file.number_files_client_needs;
     *  ofstream file = number_and_file.file_compare_tree;
     *  ofstream server_file;
     *  for(uint files_left = number_files; files_left > 0; files_left--){
     *      if(listen_as_tcp(port=port) == files_left){
     *          send_data_via_tcp(to=server_ip, port=port, data=files_left);
     *          server_file = listen_as_ftp_until_get_file(port=port);
     *          write_file_in_its_place(file=server_file, place=find_out_place_by_number(files_left));
     *      }
     *  }
     */
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
        ip_t server_ip = get_server_ip();
        number_and_file_struct number_and_file_need_client = send_and_return_number_and_files_of_files_client_needs(
                server_ip, output_file_name);
        uint number_files_client_needs = number_and_file_need_client.number_files_client_needs;
        if (number_files_client_needs > 0){
            get_files_need_by_client_from_server(server_ip, number_and_file_need_client);
        }
    }
    return 0;
}
