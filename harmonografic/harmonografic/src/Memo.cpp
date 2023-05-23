#include "Memo.h"

Node::Node(std::string _title, std::string _description, std::string _tags) : m_title{ _title }, m_description{ _description }, m_tag{ _tags }
{
}

void Node::add_Child(Node* _child)
{

	m_children.push_back(_child);
	_child->m_parent = this;

}





Tree::Tree()
{
	m_root = new Node("Main Root");
	m_all_Nodes[m_root->m_title] = m_root;
}

Tree::~Tree()
{
	
	delete_Node(m_root->m_title);
}

void Tree::traverse(Node* _node)
{
	unsigned int depth = count_deep_Node(_node);
	
	std::string tab_text{ "" };

	while (depth)
	{
		tab_text += "  ";
		depth--;
	}

	std::cout << "  " <<_node->m_title << " : " << _node->m_description << "\n" << tab_text << "  Tags: " << _node->m_tag << "\n";
	
	if (!_node->m_children.empty())
	{
		tab_text+= "|->";

		for (Node* child : _node->m_children)
		{
			std::cout << tab_text;
			traverse(child);
		}
	}
}

void Tree::traverse(std::string _node)
{
	if (_node.empty())
	{
		_node = m_root->m_title;
	}

	if (get_title(_node) != _node)
		return;


	traverse(m_all_Nodes.at(_node));
}

void Tree::move_Node(Node* _sourceNode, Node* _targetNode)
{
	std::vector<Node*>& siblings = _sourceNode->m_parent->m_children;
	siblings.erase(std::remove(siblings.begin(), siblings.end(), _sourceNode), siblings.end());

	_targetNode->add_Child(_sourceNode);
}

unsigned int Tree::count_deep_Node(Node* _sourceNode)
{
	if (_sourceNode->m_parent==nullptr) return 0;

	unsigned int ret = count_deep_Node(_sourceNode->m_parent) + 1;
	return ret;
}

void Tree::move_Node(std::string _sourceNode, std::string _targetNode)
{
	if (get_title(_targetNode) != _targetNode && get_title(_sourceNode) != _sourceNode)
		return;
	
	move_Node(m_all_Nodes.at(_sourceNode), m_all_Nodes.at(_targetNode));
}

void Tree::delete_Node(std::string _targetNode)
{
	if (get_title(_targetNode) != _targetNode)
		return;

	Node* to_erase = m_all_Nodes.at(_targetNode);

	// delete from parent list if list exist
	if (to_erase->m_parent != nullptr)
	{
		std::vector<Node*>& siblings = to_erase->m_parent->m_children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), to_erase), siblings.end());
	}


	while (to_erase->m_children.size() > 0)
	{
		delete_Node(to_erase->m_children[0]->m_title);
	}

	// delete from all_nodes list
	m_all_Nodes.erase(_targetNode);

	// delete pointer - perm
	delete to_erase;

}

void Tree::add_Node(std::string _title, std::string _description, std::string _tags, std::string _target_Node )
{
	//test if string target is set, if not add child to main root
	if (_target_Node.empty())
	{
		_target_Node = m_root->m_title;
	}

	if (get_title(_title) == _title)
	{
		std::cout << "Title: { " << _title << " } exist. Creating Note terminated.\n";
		return;
	}

	// new Node
	m_all_Nodes[_title] = new Node(_title, _description, _tags);
	// assaning new Node to targeted parent
	m_all_Nodes.at(_target_Node)->add_Child(m_all_Nodes[_title]);
}

void Tree::edit_Node(std::string _target_Node, std::string _new_title, std::string _new_description, std::string _new_tags)
{
	if (get_title(_target_Node) != _target_Node)
		return;
	// catch pointer
	Node* modify_node = m_all_Nodes.at(_target_Node);

	// delete old naming from m_all_Nodes
	m_all_Nodes.erase(_target_Node);

	// modify node
	modify_node->m_title = _new_title;
	modify_node->m_description = _new_description;
	modify_node->m_tag = _new_tags;

	// put old Node with new title
	m_all_Nodes[_new_title] = modify_node;

}

std::string Tree::get_parent_title(std::string _child_parent)
{
	if (m_all_Nodes.find(_child_parent) != m_all_Nodes.end())
	return m_all_Nodes.at(_child_parent)->m_parent->m_title;

	return std::string();
}

std::string Tree::get_title(std::string _node)
{
	if (m_all_Nodes.find(_node) != m_all_Nodes.end())
		return m_all_Nodes.at(_node)->m_title;

	return std::string();

}

size_t Tree::get_size(std::string _title)
{
	if (_title.empty()) _title = m_root->m_title;
	
	size_t ret = 0;
	
	if (get_title(_title) != _title) return ret;

	ret = m_all_Nodes.at(_title)->m_children.size();
	return ret;
}

