#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>


// struct or class?
struct Node
{
	/*		Functions		*/
	// creating new node
	Node(std::string _title, std::string _description = "", std::string _tags = "");

	// adding node as a child
	void add_Child(Node* _child);

	/*		Members			*/
	// main card title
	std::string m_title;
	
	// description of the card
	std::string m_description;
	
	// tags
	std::string m_tag;
	
	// parent
	Node* m_parent = nullptr;

	// children
	std::vector<Node*> m_children;


};


class Tree
{
public:

	Tree();
	
	~Tree();

	void traverse(std::string _node="");

	// moving Nodes to other parent
	void move_Node(std::string _sourceNode, std::string _targetNode);
	
	// deleting Nodes and whole branch
	void delete_Node(std::string _targetNode);

	// new Nodes
	void add_Node(std::string _title, std::string _description = "", std::string _tags = "", std::string _target_Node = "");
	// edit/modify node
	void edit_Node(std::string _target_Node, std::string _title, std::string _description , std::string _tags = "");

	// get string node parent title
	std::string get_parent_title(std::string _child_parent);
	std::string get_title(std::string _node);

	// vector size in asked node
	size_t get_size(std::string _title= "");


private:
	Node* m_root = nullptr;


	void traverse(Node* _node);
	
	// moving Nodes to other parent
	void move_Node(Node* _sourceNode, Node* _targetNode);
	
	// return value how far node is from root
	unsigned int count_deep_Node(Node* _sourceNode);

	
	// all nodes for quick reseach
	std::map<std::string, Node*> m_all_Nodes;
};

