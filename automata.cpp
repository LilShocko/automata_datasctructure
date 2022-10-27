#include <iostream>
#include<map>
#include<vector>
#include<string>
#include<stack>
#include<algorithm>
#include<iomanip>
#include <iterator>

using namespace std;

class one {
public:
	int number;
	map <char, int> nexts;
	void add(char a, int next) {
		nexts.insert({ a,next });
	}
	void set_number(int _number) { number = _number; }
	one(int _number) {
		number = _number;
	}
	void print_one() {
		for (auto i : nexts) {
			cout << i.first << "-" << i.second << setw(5);
		}
		cout << endl;
	}
	void change_one(int plus) {
		this->number = this->number + plus;
		for (auto i = nexts.begin(); i != nexts.end(); ++i) {
			i->second += plus;
		}
	}
	vector<char> xd() {
		vector<char> dis;
		for (auto i : nexts) {
			dis.push_back(i.first);
		}
		return dis;
	}
	vector<int> dx() {
		vector<int> dis;
		for (auto i : nexts) {
			dis.push_back(i.first);
		}
		return dis;
	}

};
class automata {
private:
	vector<one> tail;
	string language;
	vector<int> final_states;
public:
	void set_final(int fin) {
		final_states.push_back(fin);
	}
	void set_language(string lang) {
		language = lang;
	}
	automata(int _num) {
		for (int i = 0; i < _num; i++) {
			one _one(i);
			tail.push_back(_one);
		}
	}

	void insert_connection(int _from, char _a, int _to) {
		tail[_from].add(_a, _to);
	}

	void print_automata() {
		cout << endl << endl;
		for (int i = 0; i < tail.size(); i++) {
			cout << "State " << tail[i].number << " has connections: ";
			tail[i].print_one();
		}
		cout << endl << endl << "Language is: " << this->language << endl << endl;
		cout << "Final states are: ";
		for (int i = 0; i < final_states.size(); i++) {
			cout << final_states[i] << "    ";
		}
		cout << endl << endl;
	}
	bool check_symbol_in_language(char s2) {
		bool check = false;
		for (int i = 0; i < language.size(); i++) {
			if (language[i] == s2)check = true;
		}
		return check;
	}
	bool check_if_zero_final() {
		bool check = false;
		for (int i = 0; i < final_states.size(); i++) {
			if (final_states[i] == 0)check = true;
		}
		return check;
	}
	bool check_word(string s1) {
		int i = 0;
		int x;
		if (s1.empty() == true && check_if_zero_final() == true)return true;

		while (!s1.empty()) {
			if (check_symbol_in_language(s1[0]) == false) return false;
			bool check = false;
			for (auto z : tail[i].nexts) {
				if (z.first == s1[0]) {
					check = true;
					x = z.second;
					i = x;
					s1.erase(s1.begin());
					break;
				}
			}
			if (check == false)return check;
		}

		if (find(final_states.begin(), final_states.end(), x) != final_states.end()) {
			return true;
		}
		else {
			return false;
		}
	}
	void  change_index(int plus) {
		for (int i = 0; i < final_states.size(); i++) {
			this->final_states[i] = this->final_states[i] + plus;
		}
		for (int i = 0; i < tail.size(); i++) {
			this->tail[i].change_one(plus);
		}
	}

	bool check_if_state_is_final(int _num) {
		bool check = false;
		for (int i = 0; i < final_states.size(); i++) {
			if (final_states[i] == _num)check = true;
		}
		return check;
	}

	automata& uninion(const automata& other) {
		//copie na other?
		automata copy_other = other;
		bool check = copy_other.check_if_zero_final();
		copy_other.change_index(this->tail.size() - 1);
		automata a1(this->tail.size() + other.tail.size() - 1);

		for (auto i : this->tail[0].nexts) {
			a1.tail[0].add(i.first, i.second);
		}
		for (auto i : copy_other.tail[0].nexts) {
			a1.tail[0].add(i.first, i.second);
		}

		a1.set_language(this->language);
		if (this->check_if_zero_final() == false && check == false) {
			for (int i = 0; i < this->final_states.size(); i++) {
				a1.final_states.push_back(this->final_states[i]);
			}
			for (int i = 0; i < copy_other.final_states.size(); i++) {
				a1.final_states.push_back(copy_other.final_states[i]);
			}
		}

		else {
			for (int i = 0; i < this->final_states.size(); i++) {
				a1.final_states.push_back(this->final_states[i]);
			}
			for (int i = 0; i < copy_other.final_states.size(); i++) {
				a1.final_states.push_back(copy_other.final_states[i]);
			}
			a1.final_states.push_back(0);
		}
		for (int i = 1; i < this->tail.size(); i++) {
			a1.tail[i].nexts = this->tail[i].nexts;
		}
		for (int i = this->tail.size(); i < copy_other.tail.size() + this->tail.size() - 1; i++) {
			a1.tail[i].nexts = copy_other.tail[i - (this->tail.size() - 1)].nexts;
		}
		*this = a1;
		return *this;
	}

	automata& concat(const automata& other) {
		automata copy_other = other;
		copy_other.change_index(this->tail.size() - 1);

		automata a1(this->tail.size() + other.tail.size() - 1);

		a1.set_language(this->language);
		if (copy_other.check_if_zero_final() == true) {
			for (int i = 0; i < this->final_states.size(); i++) {
				a1.final_states.push_back(this->final_states[i]);
			}
			for (int i = 0; i < copy_other.final_states.size(); i++) {
				a1.final_states.push_back(copy_other.final_states[i]);
			}
		}
		else {
			for (int i = 0; i < copy_other.final_states.size(); i++) {
				a1.final_states.push_back(copy_other.final_states[i]);
			}
		}
		for (int i = 0; i < this->tail.size(); i++) {
			if (check_if_state_is_final(this->tail[i].number) == false) {
				a1.tail[i].nexts = this->tail[i].nexts;
			}
			else {
				a1.tail[i].nexts = copy_other.tail[0].nexts;
			}
		}

		for (int i = this->tail.size(); i < copy_other.tail.size() + this->tail.size() - 1; i++) {
			a1.tail[i].nexts = copy_other.tail[i - (this->tail.size() - 1)].nexts;
		}
		*this = a1;
		return *this;

	}

	bool check_if_totalized() {
		bool check = true;
		for (int i = 0; i < tail.size(); i++) {
			if (tail[i].nexts.size() < language.size()) check = false;
		}
		return check;
	}
	automata& totalize() {
		if (this->check_if_totalized() == true) {
			return *this;
		}
		else {
			automata other(this->tail.size() + 1);
			other.set_language(this->language);
			for (int i = 0; i < this->final_states.size(); i++) {
				other.final_states.push_back(this->final_states[i]);
			}
			for (int i = 0; i < this->tail.size(); i++) {
				vector<char> connections = tail[i].xd();
				vector<char> lang;
				vector<char> new_vec;
				for (int j = 0; j < language.size(); j++) {
					lang.push_back(language[j]);
				}
				set_difference(lang.begin(), lang.end(), connections.begin(), connections.end(),
					inserter(new_vec, new_vec.begin()));

				other.tail[i] = this->tail[i];
				for (int j = 0; j < new_vec.size(); j++) {
					other.tail[i].add(new_vec[j], other.tail.size() - 1);
				}
			}

			for (int z = 0; z < language.size(); z++) {
				other.tail[other.tail.size() - 1].add(language[z], other.tail.size() - 1);
			}

			*this = other;
			return *this;
		}
	}

	automata& reverse() {
		this->totalize();
		vector<int> new_vec;
		vector<int> big;
		for (int i = 0; i < tail.size(); i++) {
			big.push_back(tail[i].number);
		}
		set_difference(big.begin(), big.end(), final_states.begin(), final_states.end(),
			inserter(new_vec, new_vec.begin()));
		final_states = new_vec;
		return *this;
	}

	automata& iterate() {
		//nakudeto sochi nachalnoto natam sochat i finalnite

		for (auto i : final_states) {
			for (auto j : tail[0].nexts) {
				tail[i].add(j.first, j.second);
			}
		}
		set_final(0);
		return *this;
	}
};





int main() {
	/*
	automata a1(3);
	a1.set_final(0);

	a1.set_language("ab");

	a1.insert_connection(0);
	a1.insert_connection(0);

	a1.insert_connection(1);

	a1.insert_connection(2);

	a1.print_automata();
	*/
	/*
	automata a1(3);
	automata a2(3);
	a1.set_final(2);
	a2.set_final(2);
	a1.set_language("ab");
	a2.set_language("ab");

	a1.insert_connection(0,'a',1);
	a1.insert_connection(1,'b',2);

	a2.insert_connection(0,'b',1);
	a2.insert_connection(1,'a',2);

	cout << endl;
	a1.uninion(a2);
	a1.print_automata();
	cout << a1.check_word("ab")<<a1.check_word("ba")<<a1.check_word("baab");
	*/
	
	/*
	automata a1(2);
	a1.set_language("ab");
	a1.set_final(1);

	a1.insert_connection(0,'a',1);

	a1.reverse();
	a1.print_automata();
	cout << a1.check_word("") << a1.check_word("a") << a1.check_word("ab");
	*/

	/*
	automata a1(4);
	a1.set_language("ab");
	a1.set_final(2);
	a1.set_final(3);

	a1.insert_connection(0,'a',1);
	a1.insert_connection(1, 'b', 2);
	a1.insert_connection(0, 'b', 3);

	a1.iterate();
	a1.print_automata();
	cout << a1.check_word("") << a1.check_word("a") << a1.check_word("ab");
	*/


	return 0;
}
