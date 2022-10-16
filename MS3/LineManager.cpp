#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "LineManager.h"
#include "Utilities.h"

using namespace std;
namespace sdds
{
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
	{

		if (file.empty())
		{
			throw std::string("ERROR: No filename provided.");
		}
		std::ifstream filein(file);
		if (!filein)
			throw std::string("Unable to open [") + file + "] file.";

		std::string line;
		while (std::getline(filein, line))
		{
			// the tokens from the record(line) 
			std::string workstation = line.substr(0, line.find_first_of('|'));
			std::string nextWorkstation = line.erase(0, workstation.size() + 1);

			//loop through stations, find the station witch the name match the record token "workstation"
			//push it into activeLine
			auto ws = std::find_if(stations.begin(), stations.end(), [&](Workstation* ws1)
				{
					return ws1->getItemName() == workstation;
				});
			m_activeLine.push_back(*ws);

			//if the string nextworkstaion is not empty
			//loop through stations agian, find the station wich the name mathc the record token "next workstation"
			//set this station as the next station of this record

			//else(nextworkstation string is empty), set the next station pointer point to nullptr			
			if (!nextWorkstation.empty())
			{
				for_each(stations.begin(), stations.end(), [&](Workstation* ws1)
					{
						if (ws1->getItemName() == nextWorkstation)
							m_activeLine.back()->setNextStation(ws1);
					});
			}
			else
			{
				m_activeLine.back()->setNextStation(nullptr);
			}
		}

		//find the m_firstStation
		for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws)
			{
				auto first = std::find_if(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws1)
					{
						return ws == ws1->getNextStation();
					});
				if (first == m_activeLine.end())
				{
					m_firstStation = ws;
				}
			});

		//record the number of customer orders needs to be filled
		m_cntCustomerOrder = g_pending.size();		
	}

	void LineManager::reorderStations()
	{
		//create a vector of workstations named temp
		//copy the activeLine to this temp
		//loop throu the temp, find the station which name matched name of staion in the current station's pnextWorkstation pointer
		//store this matched station as the next station in the activeLine
		//repeat this process for each station in the original activeLine station, and get a new ordered line of stations

		std::vector<Workstation*> sortedStations;

		sortedStations.push_back(m_firstStation);
		if (m_firstStation->getNextStation())
		{
			sortedStations.push_back(m_firstStation->getNextStation());
		}

		std::for_each(m_activeLine.begin(), m_activeLine.end() - 2, [&](Workstation* ws)
			{
				sortedStations.push_back(sortedStations.back()->getNextStation());
			});

		m_activeLine = sortedStations;
	}

	bool LineManager::run(std::ostream& os)
	{
		//set a local variable for counting iterations
		//if there is orders in the g_pending vector, move the first one to the m_firstStation, remove this order from g_pending
		//loop through the line to fill
		//loop through the line to move the order(using attemptToMoveOrder())

		static unsigned int COUNTER = 0u;
		os << "Line Manager Iteration: " << ++COUNTER << std::endl;
		if (!g_pending.empty())
		{
			*m_firstStation += std::move(g_pending.front());
			g_pending.pop_front();

		}
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* ws)
			{
				ws->fill(os);
			});
		std::for_each(m_activeLine.begin(), m_activeLine.end(), [](Workstation* ws)
			{
				ws->attemptToMoveOrder();
			});
		//check if all the orders are finished processing - the sum of orders in complete and incomplete equals the number in the g_pending at the beginning
		return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;

	}

	void LineManager::display(std::ostream& os)const
	{
		//print the workstations in the line
		for_each(m_activeLine.begin(), m_activeLine.end(), [&](const Workstation* ws)
			{
				ws->display(os);
			});

	}
}
