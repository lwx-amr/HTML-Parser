#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>

using namespace std;

struct Tag {
    string name, parentName;
    map<string, string > attributes;
};

class Parser{
    private:
        int inputSize, queryNum;
        vector<string> input;
        vector<Tag> dom;
    public:
        Parser();
        void readInputs();
        void parseInputs();
        void makeQuery(string);
        string getTagName(int &, string);
        void getAttributes(Tag &, int &, string);
        string getQueryResult(string , string);
        void queryRunner();
};

Parser::Parser(){

}

void Parser::readInputs(){
    /*cin>>inputSize>>queryNum;
    cin.ignore();
    string line;
    /*for(int i=0; i<inputSize; i++){
        getline(cin, line);
        input.push_back(line);
    }*/
    input.push_back("<a value = \"GoodVal\">");
    input.push_back("<b value = \"BadVal\" size = \"10\">");
    input.push_back("<c height = \"auto\">");
    input.push_back("<d size = \"3\">");
    input.push_back("<e strength = \"200%\">");
    input.push_back("<f a1 = \"1\" a2 = \"2\" a3 = \"3\">");
    input.push_back("</f>");
    input.push_back("</e>");
    input.push_back("</d>");
    input.push_back("</c>");
    input.push_back("</b>");
    input.push_back("</a>");
}

string Parser::getTagName(int &i, string s){
    string temp = "";
    for( ;i<s.size() && (s[i] != ' '); i++){
        if(s[i]== '/' || s[i]=='<' || s[i]=='>' )
            continue;
        temp+=s[i];
    }
    return temp;
}

void Parser::getAttributes(Tag &obj, int &i, string s){
    string name, value;
    for(;i<s.size(); i++){
        name = "";
        value="";
        while(s[i]!=' '){
            name +=s[i];
            i++;
        }
        i+=4; // to ignore( = ");
        while(s[i]!=' ' && s[i]!='"'){
          value +=s[i];
          i++;
        }
        i++;
        if(name.size() != 0 && value.size() != 0)
            obj.attributes.insert(pair<string, string >(name, value));
    }

}

void Parser::parseInputs(){
    string currentTagName = "";
    Tag lastObj;
    for(auto s : input){
        string tagName = "";
        int i=0;
        if(s[i]=='<')
            tagName = getTagName(i, s);
        size_t pos = s.find('/');
        if(tagName==currentTagName || pos != string::npos){
            size_t posForDot = lastObj.name.find_last_of('.');
            if(posForDot != string::npos)
                lastObj.name = lastObj.name.substr(0, posForDot);
            else
                lastObj.name = "";
        }
        Tag currentObj;
        currentObj.name = tagName;
        getAttributes(currentObj, ++i, s);
        if(lastObj.name.size()!=0){
            currentObj.name = lastObj.name + "." + currentObj.name;
            lastObj = currentObj;
        }
        currentTagName = tagName;
        dom.push_back(currentObj);
        lastObj = currentObj;
    }
    for (auto tag : dom) {
        cout<<"Name: "<<tag.name<<endl;
        for (const auto& x : tag.attributes) {
            cout << x.first << ": " << x.second << "\n";
        }
        cout << "/*-------------------------*/"<< endl;
    }
}

string Parser::getQueryResult(string tagName, string prop){
    for (auto tag : dom) {
        if(tag.name == tagName){
            map<string, string>::iterator itr = tag.attributes.find(prop);
            if (itr != tag.attributes.end())
                return itr->second;
            break;
        }
    }
    return "Not Found!";
}

void Parser::makeQuery(string query){
    string tagName="", prop="", temp;
    stringstream ss(query);
    getline(ss, temp, '~');
    tagName = temp;
    getline(ss, temp, '~');
    prop= temp;
    cout<<getQueryResult(tagName, prop)<<endl;
}

void Parser::queryRunner(){
    /*cin.ignore();
    string line;
    /*for(int i=0; i<queryNum; i++){
        getline(cin, line);
        makeQuery(line);
    }*/

    makeQuery("a.b.c.d.e.f~a1");
    makeQuery("a.b.f~a1");
    makeQuery("a.b~size");
    makeQuery("a.b.c.d.e.f~a2");
    makeQuery("a.b.c.d.e.f~a3");
    makeQuery("a.c~height");
    makeQuery("a.b.d.e~strength");
    makeQuery("a.b.c.d.e~strength");
    makeQuery("d~sze");
    makeQuery("a.b.c.d~size");
}

int main() {
    Parser obj;
    obj.readInputs();
    obj.parseInputs();
    obj.queryRunner();
    return 0;
}
