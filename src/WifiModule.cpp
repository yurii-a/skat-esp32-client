// #include <WiFi.h>
// #include <libconfig.h++>
// #include <iostream>

// void setupWifi() {
//     libconfig::Config cfg;
//     cfg.readFile("config.cfg");

//     const std::string ssid = cfg.lookup("ssid").c_str();
//     const std::string password = cfg.lookup("password").c_str();

//     // Use api_key in your application
//     std::cout << "SSID: " << ssid << std::endl;
//     std::cout << "Password: " << password << std::endl;
// }