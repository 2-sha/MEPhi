#include "Deck.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace laba3 
{
	Deck::Deck(int len_temp)
	{
		if (len_temp > 52 || len_temp < 0) 
		{
			throw("Error len");
		}
		len = 0;
		max_len = min(MAX_LEN, QUOTA);
		data = new Card[max_len];
		for (int i = 0; i < len_temp; i += 1) {
			++(*this);
		}
	}

	Deck::Deck(Card card)
	{
		len = 1;
		max_len = min(MAX_LEN, QUOTA);
		data = new Card[max_len];
		data[0] = card;
	}

	Deck::Deck(const Deck& deck) : max_len(deck.max_len), len(deck.len)
	{
		data = new Card[len];
		for (int i = 0; i < len; i += 1) 
		{
			data[i] = deck.data[i];
		}
		//cout << "Deck&" << endl;
	}

	Deck::Deck(Deck&& deck) : max_len(deck.max_len), len(deck.len), data(deck.data)
	{
		deck.data = nullptr;
		//cout << "Deck&&" << endl;
	}

	Deck& Deck::operator =(const Deck& deck)
	{
		if (this != &deck) {
			max_len = deck.max_len;
			len = deck.len;
			delete[] data;
			data = new Card[max_len];
			for (int i = 0; i < len; ++i)
				data[i] = deck.data[i];
		}
		//cout << "=Deck&" << endl;
		return *this;
	}
	Deck& Deck::operator =(Deck&& deck)
	{
		if (&deck == this)
			return *this;
		std::swap(max_len, deck.max_len);
		std::swap(len, deck.len);
		std::swap(data, deck.data);
		//cout << "=Deck&&" << endl;
		return *this;
	}

	int Deck::find_card(const Card& card) const
	{
		for (int i = 0; i < len; i += 1) {
			if (data[i].compare(card) == 0) 
			{
				return i;
			}
		}
		return -1;
	}

	void Deck::set_card(Card card, int i) 
	{
		if (i < 0 || i > len) {
			throw ("Error index");
		}
		data[i] = card;
	}

	int Deck::delete_card(const Card& card)
	{
		int s1 = find_card(card);
		if (s1 == -1)
		{
			return 1;
		}
		for (int i = s1; i < len - 1; i += 1)
		{
			data[i] = data[i + 1];
		}
		len -= 1;
		data[len] = Card(0);
		return 0;
	}
	
	int Deck::delete_card(int s1)
	{
		if (s1 < 0 || s1 >= len)
		{
			return 1;
		}
		for (int i = s1; i < len - 1; i += 1)
		{
			data[i] = data[i + 1];
		}
		len -= 1;
		data[len] = Card(0);
		return 0;
	}

	Deck& Deck::operator++()
	{
		if (len == 52) {
			return *this;
		}
		Card temp = Card(1);
		while (find_card(temp) != -1)
		{
			temp = Card(1);
		}
		add_new_card(temp);
		return *this;
	}

	int Deck::operator()(int i)
	{
		if (i < 0 || i >= len)
		{
			return 0;
		}
		return get_card(i).get_suit_int();
	}

	int Deck::operator[](int i)
	{
		if (i < 0 || i >= len)
		{
			return 0;
		}
		return get_card(i).get_rang_int();
	}

	int Deck::add_new_card(Card card)
	{
		if (len == 52) {
			return 1;
		}
		if (find_card(card) != -1) 
		{
			return 2;
		}
		if (len == max_len) 
		{
			Card* old = data;
			max_len = min(max_len + QUOTA, MAX_LEN);
			data = new Card[max_len];
			for (int i = 0; i < len; i += 1) 
			{
				data[i] = old[i];
			}
			delete[] old;
		}
		data[len] = card;
		len += 1;
		return 0;
	}

	void Deck::swap(Card& a, Card& b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}

	void Deck::sort(int start, int end) 
	{
		if (end > len - 1)
		{
			end = len - 1;
		}
		if (start < end) 
		{
			Card temp = data[start];
			int s = start, e = end;
			do 
			{
				while (data[s].compare(temp, 1) == 1) s += 1;
				while (data[e].compare(temp, 1) == -1) e -= 1;
				if (s <= e) 
				{
					swap(data[s], data[e]);
					s += 1;
					e -= 1;
				}
			} while (s <= e);
			sort(start, e);
			sort(s, end);
		}
	}
	std::ostream& operator<<(std::ostream& out, const Deck& deck)
	{
		for (int i = 0; i < deck.len; i += 1) 
		{
			out << deck.data[i] << std::endl;
		}
		return out;
	}
}