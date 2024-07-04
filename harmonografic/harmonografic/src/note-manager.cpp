#include "../include/note-manager.h"

note_manager::note_manager()
{};

note_manager::~note_manager()
{};

void note_manager::update_notes()
{
  // update size of the list
 if(this->m_size <= this->m_notes_tree.size()) 
        this->m_size = this->m_notes_tree.size();
  
 // if size is 0 just return
  if(this->m_size == 0) return;

  // is size is greater than 0 check if there are missing ids and add them to list
  for (unsigned long i=1; i <= this->m_size; ++i)
  {
    // TODO: if larges id is samller than size update the size. 
    note_tree_list::iterator it_to_find = this->m_notes_tree.find(i);
    if(it_to_find != m_notes_tree.end()) continue;
    this->m_missing_ids.push_back(i);
  }
}

void note_manager::add_note(note_card& _note){
  
  // create new note and add to map list
  
  // add id for new note or TODO: find if updated is old
  _note.m_id = this->m_size + 1;

  // add note to tree
  this->m_notes_tree[_note.m_id] = _note;
  
  // update note_tree_list members
  this->update_notes();
}
