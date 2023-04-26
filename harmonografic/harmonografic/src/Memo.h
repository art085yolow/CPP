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
	void addChild(Node* _child);

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
	Tree(Node* _root);
	
	~Tree();

	void traverse(std::string _node="");

	// moving Nodes to other parent
	void moveNode(std::string _sourceNode, std::string _targetNode);
	// deleting Nodes and whole branch
	void deleteNode(std::string _targetNode);

	// new Nodes
	void addNode(std::string _title, std::string _description = "", std::string _tags = "", std::string _target_Node = "");

	// get string node parent title
	std::string get_title(std::string _child_parent);

	// vector size in asked node
	size_t get_size(std::string _title= "");


private:
	Node* m_root = nullptr;


	void traverse(Node* _node);
	
	// moving Nodes to other parent
	void moveNode(Node* _sourceNode, Node* _targetNode);

	
	// all nodes for quick reseach
	std::map<std::string, Node*> m_all_Nodes;
};

