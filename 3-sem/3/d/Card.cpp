#include "Card.h"
#include <string> 
#include <iostream>

using namespace std;

namespace laba3
{
	Card::Card(bool a) 
	{	
		if (a) 
		{
			set_suit_int(rand() % 4 + 1);
			set_rang_int(rand() % 13 + 2);
		}
		else 
		{
			suit_int = 0;
			suit_ch  = '0';
			rang_str = "0";
			rang_int = 0;
		}
	}

	Card::Card(char suit_ch_temp, string rang_str_temp)
	{
		set_suit_ch (suit_ch_temp);
		set_rang_str(rang_str_temp);
	}

	Card::Card(char suit_ch_temp, int rang_int_temp)
	{
		set_suit_ch (suit_ch_temp);
		set_rang_int(rang_int_temp);
	}

	Card::Card(int suit_int_temp, string rang_str_temp)
	{
		set_suit_int(suit_int_temp);
		set_rang_str(rang_str_temp);
	}

	Card::Card(int suit_int_temp, int rang_int_temp)
	{
		set_suit_int(suit_int_temp);
		set_rang_int(rang_int_temp);
	}

	void Card::set_suit_ch(char suit_ch_temp)
	{
		suit_int = suit_ch_to_int(suit_ch_temp);
		suit_ch  = suit_ch_temp;
	}

	void Card::set_suit_int(int suit_int_temp)
	{
		suit_ch  = suit_int_to_ch(suit_int_temp);
		suit_int = suit_int_temp;
	}

	void Card::set_rang_str(string rang_str_temp)
	{
		rang_int = rang_str_to_int(rang_str_temp);
		rang_str = rang_str_temp;
	}

	void Card::set_rang_int(int rang_int_temp) 
	{
		rang_str = rang_int_to_str(rang_int_temp);
		rang_int = rang_int_temp;
	}
	
	int	Card::compare(const Card& card, bool cons_the_suit) const // -1 left more, 0 equal, 1 right more, 2 incomparable
	{
		if (this->suit_int != card.suit_int)
		{
			if (cons_the_suit == 1) {
				if (this->suit_int < card.suit_int) {
					return 1;
				}
				else 
				{
					return -1;
				}
			}
			else 
			{
				return 2;
			}
			
		}

		if (this->rang_int < card.rang_int) 
		{
			return 1;
		}
		else if (this->rang_int == card.rang_int)
		{
			return 0;
		} 
		else if (this->rang_int > card.rang_int)
		{
			return -1;
		}
		throw("Error compare");
	}

	int Card::rang_str_to_int(string rang_str_temp) const
	{
		for (int i = 2; i < 11; i += 1) 
		{
			if (rang_str_temp == to_string(i)) 
			{
				return i;
			}
		}
		if (rang_str_temp == "J")
		{
			return 11;
		}
		else if (rang_str_temp == "Q")
		{
			return 12;
		}
		else if (rang_str_temp == "K") 
		{
			return 13;
		}
		else if (rang_str_temp == "A") 
		{
			return 14;
		}
		throw("Error rang_str");
	}

	string Card::rang_int_to_str(int rang_int_temp) const
	{
		if (1 < rang_int_temp && rang_int_temp < 11) 
		{
			return to_string(rang_int_temp);
		}
		if (rang_int_temp == 11) 
		{
			return ("J");
		}
		else if (rang_int_temp == 12)
		{
			return ("Q");
		}
		else if (rang_int_temp == 13) 
		{
			return ("K");
		}
		else if (rang_int_temp == 14) 
		{
			return ("A");
		}
		throw("Error rang_int");
	}

	int Card::suit_ch_to_int(char suit_ch_temp) const
	{
		if (suit_ch_temp == 'S') {
			return 1;
		}
		else if (suit_ch_temp == 'H')
		{
			return 2;
		}
		else if (suit_ch_temp == 'C')
		{
			return 3;
		}
		else if (suit_ch_temp == 'D')
		{
			return 4;
		}
		throw ("Error suit_ch");
	}

	char Card::suit_int_to_ch(int suit_int_temp) const
	{
		switch (suit_int_temp) 
		{
		case 1:
			return 'S';
		case 2:
			return 'H';
		case 3:
			return 'C';
		case 4:
			return 'D';
		}
		throw ("Error suit_int");
	}
	std::ostream& operator<<(std::ostream& out, const Card& card)
	{
		out << card.get_suit_ch() << card.get_rang_str();
		return out;
	}

}