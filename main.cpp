
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include "network.h"
#include <iostream>
#include <new>
int main(int argc, char* argv[])
{
   boost::program_options::options_description desc("Allowed Options");
   boost::program_options::variables_map options;
   desc.add_options()
       ("help", "produce help message")
       ("target", boost::program_options::value< std::vector< std::string > >(), "target to hit");
   boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), options);
   if(options.count("help"))
   {
       std::cout << desc << std::endl;
       return 1;
   }
   

   network net;
   std::vector<std::string> targets = options["target"].as< std::vector<std::string> >();
   for(std::vector<std::string>::iterator i = targets.begin(); i != targets.end(); ++i)
   {
       net.addTarget(i->c_str());
   }
   net.recv();
   net.send("Hello, World");

}





