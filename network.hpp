//
// Created by Zlolka on 03.02.2021.
//

#ifndef ADDONFORAIMP_NETWORK_HPP
#define ADDONFORAIMP_NETWORK_HPP

#include <string>

//TODO #include <boost/asio.hpp>
#include "yaml.hpp"

using std::string;

typedef uint port_t; //TODO
typedef uint ip_t;   //TODO

ip_t get_my_ip_in_local_net(); //TODO

template<class T>
void send_via_tcp_data_with_type(const ip_t& to, const port_t& port, T data); //TODO

template<class T>
T listen_port_as_tcp_until_get_data_with_type(const port_t& port); //TODO

void send_file_via_ftp(const ip_t& to, const port_t& port, const string& fname); //TODO

void listen_port_as_ftp_until_get_file_and_write_file(const port_t& port, const string& fname_to_write); // TODO

ip_t get_client_ip(const ip_t& broadcast_ip, const port_t& port){
    ip_t my_ip = get_my_ip_in_local_net();

    // Sending my IP over a broadcast channel in the hope that the client will receive it
    send_via_tcp_data_with_type<ip_t>(broadcast_ip, port, my_ip);

    // Get client ip address
    ip_t client_ip = listen_port_as_tcp_until_get_data_with_type<ip_t>(port);
    return client_ip;
}

uint get_number_of_files_client_needs(const ip_t& client_ip, const port_t& port, const string& fname_of_our_tree) {
    // Send our directory tree to the client
    send_file_via_ftp(client_ip, port, fname_of_our_tree);

    // Receiving from the client the number of files he needs
    uint number_files_client_needs = listen_port_as_tcp_until_get_data_with_type<uint>(port);
    return number_files_client_needs;
}

void send_necessary_files_to_client(const ip_t& client_ip, const port_t& port, const uint& number_files){
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

ip_t get_server_ip(const port_t& port){
    ip_t my_ip = get_my_ip_in_local_net();

    // Listen port as tcp until server sends its ip address
    ip_t server_ip = listen_port_as_tcp_until_get_data_with_type<ip_t>(port);

    // Sending our ip address to the server
    send_via_tcp_data_with_type<ip_t>(server_ip, port, my_ip);
    return server_ip;
}

indexed_fnames_t send_and_return_number_and_files_of_files_client_needs(const ip_t& server_ip, const port_t& port,
        const string& fname_of_getted_tree, const string& fname_of_our_tree){
    // Get the server directory tree and write it to a file named fname_of_server_tree
    listen_port_as_ftp_until_get_file_and_write_file(port, fname_of_getted_tree);

    // Comparing our directory tree to the server's directory tree
    indexed_fnames_t number_and_file = compare_trees_in_yaml(fname_of_getted_tree, fname_of_our_tree);
    uint number_files_client_need = number_and_file.size();

    // Sending to the server the number of files we need
    send_via_tcp_data_with_type<uint>(server_ip, port, number_files_client_need);
    return number_and_file;
}

void get_files_need_by_client_from_server(const ip_t& server_ip, const port_t& port, indexed_fnames_t& indexed_fnames){
    uint number_files = indexed_fnames.size();
    string fname_track;

    for(uint files_left = number_files; files_left > 0; files_left--){
        // If the server sent us a file with the same number as we expect
        if(listen_port_as_tcp_until_get_data_with_type<uint>(port) == files_left){

            fname_track = indexed_fnames[files_left];

            // Send the server the name of the file we need
            send_via_tcp_data_with_type<string>(server_ip, port, fname_track);

            // We get the file we need and write it to the right place
            listen_port_as_ftp_until_get_file_and_write_file(port, fname_track);
        }
    }
}

#endif //ADDONFORAIMP_NETWORK_HPP
