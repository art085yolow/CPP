#include "Memo.h"

Node::Node(std::string _title, std::string _description, std::string _tags) : m_title{ _title }, m_description{ _description }, m_tag{ _tags }
{
}

void Node::addChild(Node* _child)
{

	m_children.push_back(_child);
	_child->m_parent = this;

}





Tree::Tree()
{
	m_root = new Node("Main Root");
	m_all_Nodes[m_root->m_title] = m_root;
}

Tree::Tree(Node* _root)
{
	m_root = _root;
}

Tree::~Tree()
{
	// diffrent way
	delete m_root;
}

void Tree::traverse(Node* _node)
{
	std::cout << _node->m_title << " : \n";
	for (Node* child : _node->m_children)
	{
		traverse(child);
	}
	std::cout << std::endl;
}

void Tree::traverse(std::string _node)
{
	if (_node.empty())
	{
		_node = m_root->m_title;
	}

	traverse(m_all_Nodes.at(_node));
}

void Tree::moveNode(Node* _sourceNode, Node* _targetNode)
{
	std::vector<Node*>& siblings = _sourceNode->m_parent->m_children;
	siblings.erase(std::remove(siblings.begin(), siblings.end(), _sourceNode), siblings.end());

	_targetNode->addChild(_sourceNode);
}

void Tree::moveNode(std::string _sourceNode, std::string _targetNode)
{
	moveNode(m_all_Nodes.at(_sourceNode), m_all_Nodes.at(_targetNode));
}

void Tree::deleteNode(std::string _targetNode)
{
	Node* to_erase = m_all_Nodes[_targetNode];
	
	// delete from parent list
	std::vector<Node*>& siblings = to_erase->m_parent->m_children;
	siblings.erase(std::remove(siblings.begin(), siblings.end(), to_erase), siblings.end());

	// delete from all_nodes list
	m_all_Nodes.erase(_targetNode);

	while (to_erase->m_children.size() > 0)
	{
		deleteNode(to_erase->m_children[0]->m_title);
	}

	delete to_erase;

}

void Tree::addNode(std::string _title, std::string _description, std::string _tags, std::string _target_Node )
{
	//test if string target is set, if not add child to main root
	if (_target_Node.empty())
	{
		_target_Node = m_root->m_title;
	}

	if (m_all_Nodes.find(_title) != m_all_Nodes.end())
	{
		std::cout << "Title: { " << _title << " } exist. Creating Note terminated.\n";
		return;
	}

	// new Node
	m_all_Nodes[_title] = new Node(_title, _description, _tags);
	// assaning new Node to targeted parent
	m_all_Nodes.at(_target_Node)->addChild(m_all_Nodes[_title]);
}

std::string Tree::get_title(std::string _child_parent)
{
	return m_all_Nodes.at(_child_parent)->m_parent->m_title;
}

size_t Tree::get_size(std::string _title)
{
	if (_title.empty()) _title = m_root->m_title;
	size_t ret = m_all_Nodes.at(_title)->m_children.size();
	return ret;
}

