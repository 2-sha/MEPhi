#pragma once
#include <string>

using namespace std;

namespace laba3
{
	class Card
	{
	private:
		int suit_int;      // 1  2  3  4
		char suit_ch;      // S♠ H♥ C♣ D♦
		string rang_str;   // 2 .. 10, J, Q, K, A
		int  rang_int;     // 2 .. 14
	
	public:
	// init
		Card(bool = 0); // 0 null, 1 random
		Card(char, string);
		Card(char, int);
		Card(int, string);
		Card(int, int);
	// get
		int    get_suit_int() const { return suit_int; };
		char   get_suit_ch() const { return suit_ch ; };
		string get_rang_str() const { return rang_str; };
		int    get_rang_int() const { return rang_int; };
	// set
		void set_suit_int(const int);
		void set_suit_ch(const char);
		void set_rang_str(const string);
		void set_rang_int(const int);
	// another
		int	compare(const Card&, bool = 0) const;
		int rang_str_to_int(const string) const;
		string rang_int_to_str(const int) const;
		int suit_ch_to_int(const char) const;
		char suit_int_to_ch(const int) const;
		friend std::ostream& operator<<(std::ostream&, const Card&);
	// friend
	};
}