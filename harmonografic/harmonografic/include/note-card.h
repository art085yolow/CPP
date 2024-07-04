#pragma once
#include <string>
#include <vector>


struct note_card
{
	/*		Members			*/
	
  // ID for card
  unsigned long m_id=0;

  // main card title
	std::string m_title = "";
	
	// description of the card
	std::string m_description = "";
	
	// tags
	std::string m_tag = "";
	
	// parent - branch - slot position
  unsigned long m_parent_id = 0;

	// children - connections
	std::vector<unsigned long> m_children;


};


