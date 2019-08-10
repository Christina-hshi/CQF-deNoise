#include "cqf_deNoise.h"

boost::program_options::variables_map downsize_getOpts(int argc, char* argv[]){
  namespace po = boost::program_options;
  po::options_description desc(string(argv[0])+" "+string(argv[1])+"  <options>\nOptions:");
  desc.add_options() 
    ("help,h", "print help messages") 
    ("input,i", po::value<string>()-required(), "input cqf file")
    (",q", po::value<int>()-required(), "number of bits of each value after downsizing")
    ("output,o", po::value<string>()->default("output.cqf")) 
  po::variables_map vm;
  po::store(po::parse_command_line(argc-2, argv+2, desc), vm);
  
  if (argc==2 || vm.count("help")) {
    cerr << desc << "\n";
    exit(0);
  }
  
  po::notify(vm);
  return vm;
}

boost::program_options::variables_map binaryOper_getOpts(int argc, char* argv[]){
  namespace po = boost::program_options;
  po::options_description desc(string(argv[0])+" "+string(argv[1])+" <options>\nOptions:");
  desc.add_options() 
    ("help,h", "print help messages") 
    ("input1,a", po::value<string>()-required(), "cqf file a")
    ("input2,b", po::value<string>()-required(), "cqf file b")
    ("output,o", po::value<string>()->default("output.cqf")) 
  po::variables_map vm;
  po::store(po::parse_command_line(argc-2, argv+2, desc), vm);
  
  if (argc==2 || vm.count("help")) {
    cerr << desc << "\n";
    exit(0);
  }
  
  po::notify(vm);
  return vm;
}

void cqf_downsize(const boost::program_options::variables_map& opts){
  string input=opts["input"].as<string>();
  string output=opts["output"].as<string>();
  int new_hb=opts["-q"].as<int>();
  CQF_mt cqf;
  cqf.load(input);
  int hb = cqf.qf->metadata->key_bits; 
  //check if the new_hb is valid
  if(hb <= new_hb){
    cerr<<"Orginal size of value in "+input+" is "+to_string(hb)+" bits"<<endl;
    cerr<<"Need to downsize to "<<new_hb<<" bits"<<endl;
    cerr<<"The size of value after downsizing should be smaller!"<<endl;
    exit(0);
  }else if(new_hb<=8){
    cerr<<"The size of value after downsizing should be > 8bits."<<endl;
    exit(0);
  }
  int offset=hb-new_hb;
  CQF_mt new_cqf(new_hb-8, new_hb);
  cqf.set_iterator_2pos(0);
  uint64_t key, new_key, count, new_count;
  cqf.get(key, count);
  new_key = key >>offset;
  new_count = count;
  while(cqf.next()){
    if(cqf.get(key, count)){
      if((key>>offset) == new_key){
        new_count += count;
      }else{
        if(new_cqf.insert(new_key, new_count)){
          cerr<<"The size of new CQF is too small to hold all the values!"<<endl;
          cerr<<"Try to downsize to value of size larger than "<<new_hb<<"!"<<endl;
          exit(0);
        }
        new_key = key>>offset;
        new_count = count;
      }
    }else{
      break;
    }
  }
  new_cqf.insert(new_key, new_count);
  new_cqf.save(output); 
}

void cqf_intersect(const boost::program_options::variables_map& opts){

}

void cqf_add(const boost::program_options::variables_map& opts){

}

void cqf_subtract(const boost::program_options::variables_map& opts){

}

void print_usage_general(int argc, char* argv[]){
  cout<<"Usage:"<<endl
    <<argv[0]<<" <mode> <opts>"<<endl
    <<"<mode> "<<endl
    <<"\tdownsize"<<endl
    <<"\tintersect"<<endl
    <<"\tadd"<<endl
    <<"\tsubstract"<<endl
    <<""
}

int main(int argc, char* argv[]){
  if(argc==1){
    
  }
}
