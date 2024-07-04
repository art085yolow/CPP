#pragma once

#include <string>
#include <map>
#include "note-card.h"

#define note_tree_list std::map<unsigned long, note_card>

class note_manager
{
 public:
  note_manager();
  ~note_manager();

  void add_note(note_card& _note);
 private:
  note_tree_list m_notes_tree;
  
  void update_notes();

  unsigned long m_size;
  std::vector<unsigned long> m_missing_ids;

};
