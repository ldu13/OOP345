// Project - Milestone 3
// Station.cpp

// Name: Lei Du
// Seneca Student ID: 047587134
// Seneca email: ldu13@myseneca.ca
// Date of completion: Apr. 10, 2022
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include <iostream>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"

namespace sdds
{
	size_t Station::m_widthField = 0;
	size_t Station::id_generator = 0;

	Station::Station(const std::string& str)
	{
		Utilities u;
		size_t pos = 0;
		bool more = true;
		m_name = u.extractToken(str, pos, more);
		m_serialNumber = stoi(u.extractToken(str, pos, more));
		m_noOfItems = stoi(u.extractToken(str, pos, more));
		m_widthField = (m_widthField > u.getFieldWidth()) ? m_widthField : u.getFieldWidth();
		m_desc = u.extractToken(str, pos, more);
		id_generator++;
		m_id = id_generator;
	}

	const std::string& Station::getItemName() const
	{
		return m_name;
	}

	size_t Station::getNextSerialNumber()
	{
		return m_serialNumber++;
	}

	size_t Station::getQuantity() const
	{
		return m_noOfItems;
	}

	void Station::updateQuantity()
	{
		if (m_noOfItems > 0)
		{
			m_noOfItems--;
		}
	}

	void Station::display(std::ostream& os, bool full) const
	{
		if (!full)
		{
			os << std::setw(3) << std::right << std::setfill('0') << m_id << " | " << std::setw(m_widthField) << std::left << std::setfill(' ') << m_name << " | " << std::setw(6) << std::right << std::setfill('0') << m_serialNumber << " | ";
		}
		else
		{
			os << std::setw(3) << std::right << std::setfill('0') << m_id << " | " << std::setw(m_widthField) << std::left << std::setfill(' ') << m_name << " | " << std::setw(6) << std::right << std::setfill('0') << m_serialNumber << " | " << std::setw(4) << std::right << std::setfill(' ') << m_noOfItems << " | " << m_desc;
		}
		os << std::endl;
	}
}