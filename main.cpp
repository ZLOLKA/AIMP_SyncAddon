#include <string>

#include "yaml.hpp"
#include "network.hpp"
#include "GUI.hpp"

using std::string;

int main() {
    GUI Gui = GUI();
    const string root_dir_name = Gui.root_dir_name;
    const string fname_our_tree = Gui.fname_our_tree;
    const string fname_getted_tree = Gui.fname_getted_tree;
    const port_t port = Gui.port;
    const bool im_server = Gui.im_server;
    const ip_t broadcast_ip = 255; //TODO Set real broadcast ip address

    write_to_yaml_file_directory_tree(root_dir_name, root_dir_name, fname_our_tree);

    if(im_server) {
        ip_t client_ip = get_client_ip(broadcast_ip, port);
        uint number_files_client_needs = get_number_of_files_client_needs(client_ip, port, fname_our_tree);
        if (number_files_client_needs > 0) {
            send_necessary_files_to_client(client_ip, port, number_files_client_needs);
        }
    }else{
        ip_t server_ip = get_server_ip(port);
        indexed_fnames_t number_and_file_need_client = send_and_return_number_and_files_of_files_client_needs(
                server_ip, port, fname_getted_tree, fname_our_tree);
        uint number_files_client_needs = number_and_file_need_client.size();
        if (number_files_client_needs > 0){
            get_files_need_by_client_from_server(server_ip, port, number_and_file_need_client);
        }
    }
    return 0;
}
