//
// Created by Zlolka on 03.02.2021.
//

#ifndef ADDONFORAIMP_GUI_HPP
#define ADDONFORAIMP_GUI_HPP

#include <string>
#include "network.hpp"

using std::string;

class GUI{
public:
    string root_dir_name;
    string fname_our_tree;
    string fname_getted_tree;
    port_t port;
    bool im_server;

    GUI(){
        root_dir_name     = get_root_dir_name();
        fname_our_tree    = get_fname_our_tree();
        fname_getted_tree = get_fname_getted_tree();
        port              = get_port();
        im_server         = get_im_server();
    };
protected:
    static string get_root_dir_name(){ // TODO
        //string root_dir = "C:/Music/Music/Music";
        return "C:/Music/";
    };
    static string get_fname_our_tree(){ // TODO
        return "../test.yaml";
    };
    static string get_fname_getted_tree(){ // TODO
        return "../*.yaml";
    };
    static port_t get_port(){ // TODO
        return 65;
    };
    static bool get_im_server(){ // TODO
        return true;
    };
};

#endif //ADDONFORAIMP_GUI_HPP
