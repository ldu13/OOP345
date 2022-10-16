#include <iostream>
#include "Workstation.h"

namespace sdds
{
	std::deque<CustomerOrder> g_pending{};
	std::deque<CustomerOrder> g_completed{};
	std::deque<CustomerOrder> g_incomplete{};

	Workstation::Workstation(const std::string& record) : Station(record){}

	//fills the order at the front of the queue, if there are CustomerOrders in the queue; 
	//otherwise, do nothing
	void Workstation::fill(std::ostream& os)
	{
		if (!m_orders.empty())
		if (m_orders.size() != 0)
		{
			m_orders.front().fillItem(*this, os);
		}
	}

	bool Workstation::attemptToMoveOrder()
	{
		bool moved = false;
		if (!m_orders.empty())
		{
			CustomerOrder& order = m_orders.front();
			// if the order is filled at this station, or the invetory of this station is insufficent (==0)
			if (order.isItemFilled(this->getItemName()) || this->getQuantity() == 0)
			{
				//if there is a nextStation, move to next station
				if (m_pNextStation != nullptr)
				{
					*m_pNextStation += (std::move(order));

				}
				//if at the end of the managerLine
				//move ot complete que if all items in the order is filled
				//else move to incomplete que
				else
				{
					if (order.isOrderFilled())
					{
						g_completed.push_back(std::move(order));
					}
					else
					{
						g_incomplete.push_back(std::move(order));
					}
				}
				moved = true;
				m_orders.pop_front();
			}
		}
		return moved;
	}

	void Workstation::setNextStation(Workstation* station)
	{
		m_pNextStation = station;
	}

	Workstation* Workstation::getNextStation()const
	{
		return m_pNextStation;
	}

	void Workstation::display(std::ostream& os) const
	{
		if (m_pNextStation == nullptr)
		{
			os << getItemName() << " --> " << "End of Line" << std::endl;
		}
		else
		{
			os << getItemName() << " --> " << m_pNextStation->getItemName() << std::endl;
		}
	}

	Workstation& Workstation::operator+=(CustomerOrder&& newOrder)
	{
		m_orders.push_back(std::move(newOrder));
		return *this;
	}
}
