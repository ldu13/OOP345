// Project - Milestone 3
// CustomerOrder.cpp

// Name: Lei Du
// Seneca Student ID: 047587134
// Seneca email: ldu13@myseneca.ca
// Date of completion: Apr. 10, 2022
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include <iostream>
#include <iomanip>
#include "CustomerOrder.h"
#include "Utilities.h"

namespace sdds
{
	size_t CustomerOrder::m_widthField = 0;

	CustomerOrder::CustomerOrder(){}

	// recieve a single order record as string, 
	// using Utility object to get tokens for each attribute
	CustomerOrder::CustomerOrder(const std::string& str)
	{
		Utilities u;
		size_t next_pos = 0;
		auto more = true;

		m_name = u.extractToken(str, next_pos, more);
		m_product = u.extractToken(str, next_pos, more);
		m_cntItem = 0;

		auto start_pos = next_pos;

		while (more)
		{
			// count the delimiter number to get the items number in this order
			u.extractToken(str, next_pos, more);
			m_cntItem++;
		}

		// create new Iems objects for each object in the order
		m_lstItem = new Item * [m_cntItem];
		more = true;

		for (size_t i = 0; i < m_cntItem; i++)
		{
			m_lstItem[i] = new Item(u.extractToken(str, start_pos, more));
		}

		// set the field width for displaying
		if (m_widthField < u.getFieldWidth())
		{
			m_widthField = u.getFieldWidth();
		}
	}

	CustomerOrder::CustomerOrder(const CustomerOrder& src)
	{
		throw std::string("Cannot make copies.");
	}

	CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept
	{
		*this = std::move(src);
	}

	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept
	{
		if (this != &src)
		{
			for (size_t i = 0; i < m_cntItem; i++)
			{
				delete m_lstItem[i];
			}
			if (m_lstItem != nullptr)
				delete[] m_lstItem;
			m_lstItem = src.m_lstItem;
			m_name = src.m_name;
			m_product = src.m_product;
			m_cntItem = src.m_cntItem;
			src.m_lstItem = nullptr;
			src.m_name = {};
			src.m_product = {};
			src.m_cntItem = 0;
		}
		return *this;
	}

	CustomerOrder::~CustomerOrder()
	{
		// delete each Item first then delete the m_lstItem array
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i] != nullptr)
			{
				delete m_lstItem[i];
				m_lstItem[i] = nullptr;
			}
		}
		if (m_lstItem != nullptr)
		{
			delete[] m_lstItem;
			m_lstItem = nullptr;
		}
	}

	bool CustomerOrder::isOrderFilled() const
	{
		// loop through the items in the list, check if all items are filled
		bool orderFilled = true;
		for (size_t i = 0; i < m_cntItem && orderFilled; i++)
		{
			if (m_lstItem[i]->m_isFilled == false)
			{
				orderFilled = false;
			}
		}
		return orderFilled;
	}

	bool CustomerOrder::isItemFilled(const std::string& itemName) const
	{
		// loop through the items, check if any item matches the string
		// if match found, then the item with "itemName" in the order is filled
		bool itemFilled = true;
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == itemName)
			{
				itemFilled = m_lstItem[i]->m_isFilled;
			}
		}
		return itemFilled;
	}

	void CustomerOrder::fillItem(Station& station, std::ostream& os)
	{
		// find the item whose name is matched the name of recieved station
		// if found, and the quantity in this station is not zero
		// fill this item by setting m_isFilled to true
		// then update the quantity in this station (decrease the quantity by one)
		// and print out this item is filled
		// else, print out this item is not filled

		for (auto i = 0u; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == station.getItemName())
			{
				if (station.getQuantity() > 0 && m_lstItem[i]->m_isFilled == false)
				{
					station.updateQuantity();
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					m_lstItem[i]->m_isFilled = true;
					os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
					break;
				}
				else if (station.getQuantity() == 0)
				{
					os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
				}
			}
		}
	}

	void CustomerOrder::display(std::ostream& os) const
	{
		// loop through the items in the order, prin out the information in required format
		os << m_name << " - " << m_product << std::endl;
		for (size_t i = 0; i < m_cntItem; i++)
		{
			os << "[" << std::setw(6) << std::right <<std::setfill('0') << m_lstItem[i]->m_serialNumber << "] " << std::setw(m_widthField) << std::left << std::setfill(' ') << m_lstItem[i]->m_itemName << " - ";
			if (m_lstItem[i]->m_isFilled)
			{
				os << "FILLED" << std::endl;
			}
			else
			{
				os << "TO BE FILLED" << std::endl;
			}
		}
	}
}