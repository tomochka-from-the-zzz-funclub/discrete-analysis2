#include <iostream>
#include <string>
#include <vector>
#include <string.h>

using namespace std;
int without_case(const string& str1, const string& str2){
    for (size_t i = 0; i < min(str1.length(), str2.length()); i++){

		if (tolower(str1[i]) < tolower(str2[i]))
			return -1;
		if (tolower(str1[i]) > tolower(str2[i]))
			return +1;
	}

	if (str1.length() < str2.length()) 
        return -1;
	if (str1.length() > str2.length())
		return +1;

	return 0;
}



struct words{
	string word;
	size_t line;
	size_t pos;
	words(const string& word_, const size_t line_, const size_t pos_);

};

words::words(const string& word_, const size_t line_, const size_t pos_){
	word = word;
	line = line_;
	pos = pos_;
}

vector<words> read(){
	vector<words> results;
	string str;			
	for (size_t line = 0; getline(cin, str); line++){
	    vector<string> parts;

        while (true){
            size_t idx = str.find(" ");
            if (idx == string::npos) {
                if (str.length() != 0)
                    parts.push_back(str);

                break;
            }
            if (str.substr(0, idx).length() != 0){
                parts.push_back(str.substr(0, idx));
            }
            str = str.substr(idx + 1);
        }
		for (size_t pos = 0; pos < parts.size(); pos++){
			string word = parts[pos];
			words info(word, line, pos+1);  
			results.push_back(info);
		}
	}
	return results;
}

vector<size_t> z_function(const vector<words>& input){
	vector<size_t> z(input.size(), 0);
	size_t left = 0, right = 0;

	for (size_t i = 1; i < input.size(); i++){
		if (i <= right)
			z[i] = min(right - i + 1, z[i - left]);
		while ((i + z[i] < input.size())){
			words wi1 = input[z[i]];
			words wi2 = input[i + z[i]];
			if (without_case(wi1.word, wi2.word) != 0)
				break;
			z[i]++;
		}
		if (i + z[i] - 1 > right){
			right = i + z[i] - 1;
			left = i;
		}
	}
	return z;
}

vector<size_t> find(const vector<words>& pattern, const vector<words>& text){
	vector<words> all;
    words sent("$", 0, 0);
	for (size_t i = 0; i < pattern.size(); i++)
		all.push_back(pattern[i]);
	    all.push_back(sent);
	for (size_t i = 0; i < text.size(); i++)
		all.push_back(text[i]);

	vector<size_t> results;
	vector<size_t> z = z_function(all);

	for (size_t i = 0; i < text.size(); i++){	
		size_t j = i + pattern.size() + 1;	
		if (z[j] == pattern.size())		
			results.push_back(i);
	}
	return results;
}


int main(){
	vector<words> all_data = read();
	vector<words> pattern;	
	vector<words> text;	
	for (size_t i = 0; i < all_data.size(); i++){
		words w = all_data[i];
		if (w.line == 0)
			pattern.push_back(w);
		else
			text.push_back(w);
	}

	vector<size_t> positions = find(pattern, text);   

	for (size_t i = 0; i < positions.size(); i++){
		words w = text[positions[i]];
		cout << w.line << ", " << w.pos << endl;
	}
}

