#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
	if (argc!=3)
	{
		cout<<"Usage: "<<argv[0]<<" file"<<endl;
		return 0;
	}
	
    vector<string> list;
    ifstream in_stream;
    string line;
    in_stream.open(argv[1]);
	bool ok=true;
	int n=0, ne=0;
	
    while(!in_stream.eof())
    {
		line = "";
        getline( in_stream, line);
		if (line.find("#ifdef CORRECTION")!=string::npos) 
		{ 
			ok=false; 
			list.push_back( string("\t// A faire\n")); 
		}
        
		if (ok) { list.push_back(line); ++n; } else ne++;
		
		if (line.find("#endif //CORRECTION")!=string::npos) ok=true;
    }
	if (!ok) { cout<<"end file without \"#endif //CORRECTION\" "<<endl; assert(ok); }
    in_stream.close();
	

	ofstream o(argv[2]);
	unsigned int i;
	for(i=0;i<list.size();++i)
	{
		o<<list[i]<<endl;
	}
	o.close();

	if (!ok)
		cout<<"ERROR: ";
	else
		cout<<"OK: ";
	cout<<argv[2]<<"<="<<argv[1];
	cout<<" lines="<<n<<", excluded="<<ne;
	cout<<endl;

	return 0;
}

