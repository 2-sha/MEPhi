#pragma once
#include "Card.h"

namespace laba3 
{
	class Deck 
	{
	private:
		static const int MAX_LEN = 52;
		const int QUOTA = 2;
		int max_len;
		int len;
		Card* data;
	public:
		// init
		Deck(int = 0);
		Deck(Card);
		Deck(const Deck&);
		Deck(Deck&&);

		// destruct
		~Deck() { delete[] data; };

		//get
		int get_QUOTA() const { return QUOTA; };
		int get_MAX_LEN() const { return MAX_LEN; };
		int get_max_len() const { return max_len; };
		int get_len() const { return len; };
		Card* get_data() { return data; };
		Card get_card(const int i) const { return data[i]; };
		int find_card(const Card&) const;

		// set
		void set_card(const Card, const int i);

		// another
		Deck& operator =(const Deck&);
		Deck& operator =(Deck&&);
		int delete_card(const Card&);
		int delete_card(const int);
		Deck& operator++();
		int operator()(const int);
		int operator[](const int);
		int add_new_card(const Card); // 0 ok, 1 max cards, 2 already exists
		void swap(Card&, Card&);
		void sort(int = 0, int = MAX_LEN-1);

		// friend
		friend std::ostream& operator<<(std::ostream&, const Deck&);
	};
}