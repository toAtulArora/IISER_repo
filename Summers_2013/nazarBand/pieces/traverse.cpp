/*
Trying with boost | done
*/

#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include <iostream>

using namespace boost::filesystem;


bool find_file( const path & dir_path,         // in this directory,
                const std::string & file_name, // search for this name,
                path & path_found )            // placing path here if found
{
  if ( !exists( dir_path ) ) return false;
  directory_iterator end_itr; // default construction yields past-the-end
  for ( directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
  {
    if ( is_directory(itr->status()) )
    {
      if ( find_file( itr->path(), file_name, path_found ) ) return true;
    }
    else if ( itr->path().leaf() == file_name ) // see below
    {
      path_found = itr->path();
      return true;
    }
  }
  return false;
}

int main()
{
  path current_dir("."); //
  // boost::regex pattern("a.*"); // list all files starting with a
  //for (recursive_directory_iterator iter(current_dir), end;
	for (directory_iterator iter(current_dir), end;
       iter != end;
       ++iter)
  {
	  path cPath=iter->path().leaf();
    //std::string name = iter->path().leaf();
	  std::string name= cPath.string();
    // if (regex_match(name, pattern))
      std::cout << iter->path().string() << "\n";
  }
	   path pathfound;
	   if(find_file(current_dir,"nazarBand.log",pathfound))
			std::cout<<"FOUND!!"<<pathfound.string();
	   else
		   std::cout<<"COULDN'T FIND FILE";
	  std::string test;
	  std::cin>>test;

}
