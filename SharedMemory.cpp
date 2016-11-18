#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>


int main(int argc, char *argv[])
{
   using namespace boost::interprocess;
   std::cout << "_________________________" << std::endl;
   std::cout << "process #" << argc << std::endl;

   if(argc == 1){  //Parent process
	      //Remove shared memory on construction and destruction
	      struct shm_remove
	      {
	         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
	         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
	      } remover;

	      //Create a shared memory object.
	      shared_memory_object shm (create_only, "MySharedMemory", read_write);

	      //Set size
	      shm.truncate(1);

	      //Map the whole shared memory in this process
	      mapped_region region(shm, read_write);

	      //Write all the memory to 1
	      int val = 1;
	      std::memset(region.get_address(), val, region.get_size());
	      std::cout << "main process writes '"<<val<<"'  to shared memory" << std::endl;


	      //Launch child process
	      std::string s(argv[0]); s += " child ";
	      if(0 != std::system(s.c_str()))
	         return 1;

   }
   else{
	      //Open already created shared memory object.
	      shared_memory_object shm (open_only, "MySharedMemory", read_only);

	      //Map the whole shared memory in this process
	      mapped_region region(shm, read_only);

	      //Check that memory was initialized to 1
	      char *mem = static_cast<char*>(region.get_address());
	      //std::cout << region.get_size() <<std::endl;
	      for(std::size_t i = 0; i < region.get_size(); ++i){
  	    	     int val =  (int)*mem++;
		    	 //std::cout <<  val << std::endl;
			     std::cout << "Child process retrieves '"<< val <<"' from shared memory" << std::endl;
	      }
   }
   return 0;
}
