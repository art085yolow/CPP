#include "tester.h"

tester::tester()
{

	//test 1
	{
		std::string test1{ "test1" };

		Node m{ test1 };

		std::cout << "\n empty memo with 'test1' title" << " | \n";
		std::cout << "\n test1 :: title-> " << m.m_title << " | description-> " << m.m_description << " | tags-> " << m.m_tag << " | \n\n";

		pass_test(m.m_title == test1);

		std::cout << " \n---\n\n";

	}

	// test 2
	{

		std::string test2{ "New Branch Starter" };

		Tree tr;

		std::cout << "\n Tree Record with new brach 'test2' title" << " | \n";
		std::cout << "\n test2 -> new branch : \n" ;
		tr.m_root->addChild(new Node(test2));

		tr.traverse(tr.m_root);
		pass_test(tr.m_root->m_children.size() == 1);
		std::cout << " \n---\n\n";

	}

	// test 3
	{
		std::string test3{ "Branch id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test3 | \n";

		std::cout << "\n test3 -> new branch nr: ";
		
		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.m_root->addChild(new Node(test3 + std::to_string(idx)));
			std::cout << " " << idx;
		}

		std::cout << "\n branches equel to size_test (" << size_test << ") : ";
		std::cout << "\n nodes: \n";
		tr.traverse(tr.m_root);

		pass_test(tr.m_root->m_children.size() == size_test);
		std::cout << " \n---\n\n";

	}

	// test 4
	{
		std::string test4{ "Child id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test4 | moving Nodes \n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.m_root->addChild(new Node(test4 + std::to_string(idx)));
		}

		std::cout << "\n nodes before: \n";
		tr.traverse(tr.m_root);
		std::cout << " \n";

		tr.moveNode(tr.m_root->m_children[9], tr.m_root->m_children[0]);

		std::cout << "\n nodes after: \n";
		tr.traverse(tr.m_root);
		std::cout << " \n";

		pass_test(tr.m_root->m_children.size() == 9 && tr.m_root->m_children[0]->m_children.size() == 1);
		
		std::cout << " \n---\n\n";

	}

	{
		// test 5
		std::string test5{ "Child id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test5 | moving Nodes by title \n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test5 + std::to_string(idx));

			for (size_t idy = 0; idy < 5; idy++)
			{
				tr.addNode(test5 + std::to_string(idx) + std::to_string(idy), "", "", test5 + std::to_string(idx));
			}
		}

		tr.traverse();


		// res = 10 parents with 5 children
		// moving: child 42 to parent 2, child 53 to child 84, child 91 to parent 0

		tr.moveNode("Child id: 42", "Child id: 2");
		tr.moveNode("Child id: 53", "Child id: 84");
		tr.moveNode("Child id: 91", "Child id: 0");

		tr.traverse();

		pass_test(tr.get_title("Child id: 42") == "Child id: 2" && tr.get_title("Child id: 53") == "Child id: 84" && tr.get_title("Child id: 91") == "Child id: 0");
		
	}


	{
		//test 6
	}

}

tester::~tester()
{
}

void tester::pass_test(bool b)
{
	if (b)
	{
		std::cout << "PASS\n\n";
	}
	else
	{
		std::cout << "FAIL!!!!\n\n";
	}
}
