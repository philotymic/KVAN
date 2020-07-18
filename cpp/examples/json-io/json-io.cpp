#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
using namespace std;

#include <kvan/fuargs.h>
#include <kvan/json-io.h>

#include "../csv-io/person.h"

Person p1{.name = FullName{.first_name = "John", .last_name = "Smith", .salutation = Salutation::MR},
    .address = Address{.line1 = "1 Main St", .line2 = "", .city = "New York", .state = State::NY, .zip = "10001"},
       .c = Contact{.phone="123", .email="a@b.c"}, .backup_c = Contact(),
						      .age = 42, .height = 1.72};
Person p2{.name = FullName{.first_name = "Jim", .last_name = "Morrison", .salutation = Salutation::MR},
    .address = Address{.line1 = "1 Main St", .line2 = "", .city = "Boston", .state = State::MA, .zip = "02142"},
       .c = Contact(), .backup_c = Contact(),
	  .age = 27, .height = 1.8};


ADD_ACTION("write_json[]", [](const Fuargs::args&) {
    vector<Person> persons;
    persons.push_back(p1);
    persons.push_back(p2);
    persons.push_back(p2);
    
    Band b;
    b.name = "ura";
    b.band_members = persons;
    b.ws.push_back(1.0);
    b.ws.push_back(2.0);

#if 0
    //Parents pp;
    //pp.parents.push_back(persons[0]);
    PersonSeq psq;
    psq.push_back(persons[0]);
    psq.push_back(persons[1]);
    b.band_member_parents.push_back(psq);
    b.band_member_parents.push_back(psq);
#endif
    
    auto sd = get_struct_descriptor<Band>();
    JSONVisitor json_visitor;
    LOBKey k;
    sd.visit_members(&json_visitor, &k, b);    
    cout << json_visitor.out.str() << endl;
    
    return true;
  });

ADD_ACTION("read_json[fn]", [](const Fuargs::args& args) {
    string fn = args.get("fn");
    ifstream in(fn);
    if (!in) {
      cerr << "can't open file " << fn << endl;
      return false;
    }

    stringstream json; json << in.rdbuf();

    cout << "json: " << json.str() << endl;    

    auto kvs = from_json(json.str());
    cout << "kvs:" << endl;
    for (auto& el: kvs) {
      cout << el.first << ": " << el.second << endl;
    }
    
    return true;
  });

int main(int argc, char** argv)
{
  Fuargs::exec_actions(argc, argv);
}
