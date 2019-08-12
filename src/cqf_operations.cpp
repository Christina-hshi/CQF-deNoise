/*
 * ============================================================================
 *  
 *        Authors: Christina SHI <hshi@cse.cuhk.edu.hk>
 *                 Kevin Yip <kevinyip@cse.cuhk.edu.hk>
 *
 * ============================================================================
 */

#include "core/CQF_mt.h"

boost::program_options::variables_map downsize_getOpts(int argc, char* argv[]){
  namespace po = boost::program_options;
  po::options_description desc(string(argv[0])+" "+string(argv[1])+"  <options>\nOptions:");
  desc.add_options() 
    ("help,h", "print help messages") 
    ("input,i", po::value<string>()->required(), "input cqf file")
    (",q", po::value<int>()->required(), "number of bits of each hash value after downsizing")
    ("output,o", po::value<string>()->default_value("output.cqf"), "cqf file for output"); 
  po::variables_map vm;
  
  try{
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (argc==2 || vm.count("help")) {
      cerr << desc << "\n";
      exit(0);
    }
    
    po::notify(vm);
    return vm;
  }catch(std::exception& e){
    cerr<<e.what()<<endl;
    exit(0);
  }
}

boost::program_options::variables_map intersect_getOpts(int argc, char* argv[]){
  namespace po = boost::program_options;
  po::options_description desc(string(argv[0])+" "+string(argv[1])+" <options>\nOptions:");
  desc.add_options() 
    ("help,h", "print help messages") 
    ("input1,a", po::value<string>()->required(), "cqf file a")
    ("input2,b", po::value<string>()->required(), "cqf file b")
    ("output1,A", po::value<string>()->default_value("output1.cqf"), "cqf file to output common objects with corresponding ounts in cqf file a")
    ("output2,B", po::value<string>()->default_value("output2.cqf"), "cqf file to output common objects with corresponding ounts in cqf file b");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  
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
    ("input1,a", po::value<string>()->required(), "cqf file a")
    ("input2,b", po::value<string>()->required(), "cqf file b")
    ("output,o", po::value<string>()->default_value("output.cqf"));
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  
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
  if(cqf.qf->metadata->nelts==0){
    cerr<<input<<" is empty."<<endl;
    exit(0);
  }
  int offset=hb-new_hb;
  CQF_mt new_cqf(new_hb-8, new_hb);
  cqf.set_iterator_2pos(0);
  uint64_t key, new_key, count, new_count;
  cqf.get(key, count);
  new_key = key >>offset;
  new_count = count;
  cqf.next();
  while(cqf.get(key, count)){
    if((key>>offset) == new_key){
      new_count += count;
    }else{
      if(!new_cqf.insert(new_key, new_count)){
        cerr<<"The size of new CQF is too small to hold all the values!"<<endl;
        cerr<<"Try to downsize to value of size larger than "<<new_hb<<"!"<<endl;
        exit(0);
      }
      new_key = key>>offset;
      new_count = count;
    }
    if(! cqf.next()) break;
  }
  if(!new_cqf.insert(new_key, new_count)){
    cerr<<"The size of new CQF is too small to hold all the values!"<<endl;
    cerr<<"Try to downsize to value of size larger than "<<new_hb<<"!"<<endl;
    exit(0);
  }
  new_cqf.save(output); 
}

void cqf_intersect(const boost::program_options::variables_map& opts){
  string input1 = opts["input1"].as<string>();
  string input2 = opts["input2"].as<string>();
  string output1 = opts["output1"].as<string>();
  string output2 = opts["output2"].as<string>();
  CQF_mt cqf1, cqf2;
  cqf1.load(input1);
  cqf2.load(input2);
  if((cqf1.qf->metadata->key_bits != cqf2.qf->metadata->key_bits) || (cqf1.qf->metadata->value_bits != cqf2.qf->metadata->value_bits)){
    cerr<<"Settings of both CQFs should be the same, including key bits, value bits"<<endl;
    exit(0);
  }
  int hb = cqf1.qf->metadata->key_bits;
  CQF_mt cqf1_output(hb-8, hb);
  CQF_mt cqf2_output(hb-8, hb);

  cqf1.set_iterator_2pos(0);
  cqf2.set_iterator_2pos(0);
  uint64_t key1, count1, key2, count2;
  while(cqf1.get(key1, count1) && cqf2.get(key2, count2)){
    if(key1 == key2){
      cqf1_output.insert(key1, count1);
      cqf2_output.insert(key2, count2);
      if(!cqf1.next()) break;
      if(!cqf2.next()) break;
    }else if(key1 < key2){
      if(!cqf1.next()) break;
    }else{
      if(!cqf2.next()) break;
    }
  }
  cqf1_output.save(output1);
  cqf2_output.save(output2);
}

void cqf_add(const boost::program_options::variables_map& opts){
  string input1 = opts["input1"].as<string>();
  string input2 = opts["input2"].as<string>();
  string output = opts["output"].as<string>();
  CQF_mt cqf1, cqf2;
  cqf1.load(input1);
  cqf2.load(input2);
  if((cqf1.qf->metadata->key_bits != cqf2.qf->metadata->key_bits) || (cqf1.qf->metadata->value_bits != cqf2.qf->metadata->value_bits)){
    cerr<<"Settings of both CQFs should be the same, including key bits, value bits"<<endl;
    exit(0);
  }
  int hb = cqf1.qf->metadata->key_bits;
  CQF_mt cqf_output(hb-8, hb);

  cqf1.set_iterator_2pos(0);
  cqf2.set_iterator_2pos(0);
  uint64_t key1, count1, key2, count2;
  while(cqf1.get(key1, count1) && cqf2.get(key2, count2)){
    if(key1 == key2){
      cqf_output.insert(key1, count1+count2);
      if(!cqf1.next()) break;
      if(!cqf2.next()) break;
    }else if(key1 < key2){
      cqf_output.insert(key1, count1);
      if(!cqf1.next()) break;
    }else{
      cqf_output.insert(key2, count2);
      if(!cqf2.next()) break;
    }
  }
  while(cqf1.get(key1, count1)){
    cqf_output.insert(key1, count1);
    if(!cqf1.next()) break;
  }
  while(cqf2.get(key2, count2)){
    cqf_output.insert(key2, count2);
    if(!cqf2.next()) break;
  }
  cqf_output.save(output);
}

void cqf_subtract(const boost::program_options::variables_map& opts){
  string input1 = opts["input1"].as<string>();
  string input2 = opts["input2"].as<string>();
  string output = opts["output"].as<string>();
  CQF_mt cqf1, cqf2;
  cqf1.load(input1);
  cqf2.load(input2);
  if((cqf1.qf->metadata->key_bits != cqf2.qf->metadata->key_bits) || (cqf1.qf->metadata->value_bits != cqf2.qf->metadata->value_bits)){
    cerr<<"Settings of both CQFs should be the same, including key bits, value bits"<<endl;
    exit(0);
  }
  int hb = cqf1.qf->metadata->key_bits;
  CQF_mt cqf_output(hb-8, hb);

  cqf1.set_iterator_2pos(0);
  cqf2.set_iterator_2pos(0);
  uint64_t key1, count1, key2, count2;
  while(cqf1.get(key1, count1) && cqf2.get(key2, count2)){
    if(key1 == key2){
      if(count1-count2 > 0) cqf_output.insert(key1, count1-count2);
      if(!cqf1.next()) break;
      if(!cqf2.next()) break;
    }else if(key1 < key2){
      cqf_output.insert(key1, count1);
      if(!cqf1.next()) break;
    }else{
      if(!cqf2.next()) break;
    }
  }
  while(cqf1.get(key1, count1)){
    cqf_output.insert(key1, count1);
    if(!cqf1.next()) break;
  }
  cqf_output.save(output);
}

void print_usage_general(int argc, char* argv[]){
  cout<<"Usage:"<<endl
    <<argv[0]<<" <operation> <opts>"<<endl
    <<"<operation> "<<endl
    <<"\tdownsize"<<endl
    <<"\tintersect"<<endl
    <<"\tadd"<<endl
    <<"\tsubstract"<<endl
    <<"# use -h to print help message for each operation"<<endl;
}

int main(int argc, char* argv[]){
  if(argc==1 || strcmp(argv[1], "-h") == 0){
    print_usage_general(argc, argv);
  }else{
    string op_str(argv[1]);
    if(op_str == "downsize"){
      boost::program_options::variables_map opts = downsize_getOpts(argc, argv);
      cqf_downsize(opts);
    }else if(op_str == "intersect"){
      boost::program_options::variables_map opts = intersect_getOpts(argc, argv);
      cqf_intersect(opts);
    }else if(op_str == "add"){
      boost::program_options::variables_map opts = binaryOper_getOpts(argc, argv);
      cqf_add(opts);
    }else if(op_str == "substract"){
      boost::program_options::variables_map opts = binaryOper_getOpts(argc, argv);
      cqf_subtract(opts);
    }else{
      cerr<<"Unrecognized operation : "<<argv[1]<<endl;
      print_usage_general(argc, argv);
    }
  }
  return 0;
}
