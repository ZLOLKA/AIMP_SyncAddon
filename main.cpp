#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

//TODO #include <boost/asio.hpp>

using std::ofstream;
using std::endl;
using std::string;
using std::filesystem::directory_iterator;
using std::map;

typedef uint port_t; //TODO
typedef uint ip_t;   //TODO
typedef map<uint, string> indexed_filenames_t;

//string root_dir = "C:/Music/Music/Music";
const string root_dir_name = "C:/Music/";                           // | TODO _GUI implement a GUI to enter this data
const string file_name_of_our_tree = "../test.yaml";                // | TODO _GUI
const string file_name_of_getted_tree = "../*.yaml";                // | TODO _GUI
const port_t port = 65;                                             // | TODO _GUI
const ip_t broadcast_ip = 255; //TODO Set real broadcast ip address

//-----------------------------------------Go to the network.hpp--------------------------------------------------------

ip_t get_my_ip_in_local_net(); //TODO

template<class T>
void send_via_tcp_data_with_type(ip_t to, port_t port, T data); //TODO

template<class T>
T listen_port_as_tcp_until_get_data_with_type(port_t port); //TODO

void send_file_via_ftp(ip_t to, port_t port, string file_name); //TODO

void listen_port_as_ftp_until_get_file_and_write_file(port_t port, const string filename_to_write); // TODO

//----------------------------------------------------------------------------------------------------------------------

template<class T>
T gui_enter(); //TODO plug

indexed_filenames_t compare_trees_in_yaml(string file_name_1, string file_name_2); // TODO

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
    ip_t my_ip = get_my_ip_in_local_net();

    // Sending my IP over a broadcast channel in the hope that the client will receive it
    send_via_tcp_data_with_type<ip_t>(broadcast_ip, port, my_ip);

    // Get client ip address
    ip_t client_ip = listen_port_as_tcp_until_get_data_with_type<ip_t>(port);
    return client_ip;
}

uint get_number_of_files_client_needs(ip_t client_ip) {
    // Send our directory tree to the client
    send_file_via_ftp(client_ip, port, file_name_of_our_tree);

    // Receiving from the client the number of files he needs
    uint number_files_client_needs = listen_port_as_tcp_until_get_data_with_type<uint>(port);
    return number_files_client_needs;
}

void send_necessary_files_to_client(ip_t client_ip, uint number_files){
    string file_name_of_needed_by_client;

    for(uint files_left = number_files; files_left > 0; files_left--){
        // We send the client the file number that we are going to transfer to him
        send_via_tcp_data_with_type<uint>(client_ip, port, files_left);

        // We get the name of the file that will be sent to the client
        file_name_of_needed_by_client = listen_port_as_tcp_until_get_data_with_type<string>(port);

        // Send the file to the client
        send_file_via_ftp(client_ip, port, file_name_of_needed_by_client);
    }
}

ip_t get_server_ip(){
    ip_t my_ip = get_my_ip_in_local_net();

    // Listen port as tcp until server sends its ip address
    ip_t server_ip = listen_port_as_tcp_until_get_data_with_type<ip_t>(port);

    // Sending our ip address to the server
    send_via_tcp_data_with_type<ip_t>(server_ip, port, my_ip);
    return server_ip;
}

indexed_filenames_t send_and_return_number_and_files_of_files_client_needs(ip_t server_ip){
    // Get the server directory tree and write it to a file named file_name_of_server_tree
    listen_port_as_ftp_until_get_file_and_write_file(port, file_name_of_getted_tree);

    // Comparing our directory tree to the server's directory tree
    indexed_filenames_t number_and_file = compare_trees_in_yaml(file_name_of_getted_tree, file_name_of_our_tree);
    uint number_files_client_need = number_and_file.size();

    // Sending to the server the number of files we need
    send_via_tcp_data_with_type<uint>(server_ip, port, number_files_client_need);
    return number_and_file;
}

void get_files_need_by_client_from_server(ip_t server_ip, indexed_filenames_t& indexed_filenames){
    uint number_files = indexed_filenames.size();
    string file_name_track;

    for(uint files_left = number_files; files_left > 0; files_left--){
        // If the server sent us a file with the same number as we expect
        if(listen_port_as_tcp_until_get_data_with_type<uint>(port) == files_left){

            file_name_track = indexed_filenames[files_left];

            // Send the server the name of the file we need
            send_via_tcp_data_with_type<string>(server_ip, port, file_name_track);

            // We get the file we need and write it to the right place
            listen_port_as_ftp_until_get_file_and_write_file(port, file_name_track);
        }
    }
}

int main() {
    ofstream output_file(file_name_of_our_tree);
    write_to_yaml_file_directory_tree(root_dir_name, root_dir_name, output_file);
    output_file.close();
    bool im_server = gui_enter<bool>(); //TODO _GUI selected in GUI
    if(im_server) {
        ip_t client_ip = get_client_ip();
        uint number_files_client_needs = get_number_of_files_client_needs(client_ip);
        if (number_files_client_needs > 0) {
            send_necessary_files_to_client(client_ip, number_files_client_needs);
        }
    }else{
        ip_t server_ip = get_server_ip();
        indexed_filenames_t number_and_file_need_client = send_and_return_number_and_files_of_files_client_needs(
                server_ip);
        uint number_files_client_needs = number_and_file_need_client.size();
        if (number_files_client_needs > 0){
            get_files_need_by_client_from_server(server_ip, number_and_file_need_client);
        }
    }
    return 0;
}
