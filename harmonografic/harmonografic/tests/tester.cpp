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

		std::cout << "\n Tree Record with new brach 'test2' title" << " | \n\n";
		std::cout << "\n test2 -> new branch : \n" ;
		tr.addNode(test2);

		tr.traverse();
		pass_test(tr.get_size() == 1);
		std::cout << " \n---\n\n";

	}

	// test 3
	{
		std::string test_text{ "Child id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test3 | \n";

		std::cout << "\n test3 -> new branch nr: ";
		
		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));
			std::cout << " " << idx;
		}

		std::cout << "\n branches equel to size_test (" << size_test << ") : ";
		std::cout << "\n nodes: \n";
		tr.traverse();

		pass_test(tr.get_size() == size_test);
		std::cout << " \n---\n\n";

	}

	// test 4
	{
		std::string test_text{ "Child id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test4 | moving Nodes \n\n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));
		}

		std::cout << "\n nodes before: \n";
		tr.traverse();
		std::cout << " \n";

		std::cout<< " Moving - Child id: 9 to Child id: 0\n\n";
		tr.moveNode("Child id: 9", "Child id: 0");

		std::cout << "\n nodes after: \n";
		tr.traverse();
		std::cout << " \n";

		//pass_test(tr.m_root->m_children.size() == 9 && tr.m_root->m_children[0]->m_children.size() == 1);
		pass_test(tr.get_size() == 9 && tr.get_size("Child id: 0") == 1);
		
		std::cout << " \n---\n\n";

	}

	{
		// test 5
		std::string test_text{ "Child id: " };
		unsigned int size_test = 10;

		Tree tr;

		std::cout << "\n Tree Record test5 | moving Nodes by title \n\n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));

			for (size_t idy = 0; idy < 5; idy++)
			{
				tr.addNode(test_text + std::to_string(idx) + std::to_string(idy), "", "", test_text + std::to_string(idx));
			}
		}

		tr.traverse();


		// res = 10 parents with 5 children
		// moving: child 42 to parent 2, child 53 to child 84, child 91 to parent 0

		tr.moveNode("Child id: 42", "Child id: 2");
		tr.moveNode("Child id: 53", "Child id: 84");
		tr.moveNode("Child id: 91", "Child id: 0");

		tr.traverse();

		std::cout << " \n";

		pass_test(tr.get_title("Child id: 42") == "Child id: 2" && tr.get_title("Child id: 53") == "Child id: 84" && tr.get_title("Child id: 91") == "Child id: 0");
		
		std::cout << " \n---\n\n";

	}


	{
		//test 6

		std::string test_text{ "Child id: " };
		unsigned int size_test = 5;

		Tree tr;

		std::cout << "\n Tree Record test6 | try to add Node with the same title \n\n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));
						
		}
		std::cout << " \n";

		tr.addNode("Child id: 4");
		
		std::cout << " \n";

		pass_test(tr.get_size() == size_test);
		
		std::cout << " \n---\n\n";

	}

	/// from this moment m_root and function calling by *Node are private.

	{
		//test 7
		
		std::string test_text{ "Child id: " };
		unsigned int size_test = 5;

		Tree tr;

		std::cout << "\n Tree Record test7 | deleting Node by title \n\n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));

		}
		std::cout << " \n";

		tr.traverse();

		tr.deleteNode("Child id: 3");

		tr.traverse();

		pass_test(tr.get_size() == size_test - 1);
	}

	{
		//test 8

		std::string test_text{ "Child id: " };
		unsigned int size_test = 5;

		Tree tr;

		std::cout << "\n Tree Record test8 | deleting whole tree Node \n\n";

		for (size_t idx = 0; idx < size_test; idx++)
		{
			tr.addNode(test_text + std::to_string(idx));
			if (idx == 3)
			{
				std::string name_text = test_text + std::to_string(idx);
				
				tr.addNode(name_text + std::to_string(0),"","", name_text);
				tr.addNode(name_text + std::to_string(0) + std::to_string(0),"","", name_text + std::to_string(0));
				tr.addNode(name_text + std::to_string(0) + std::to_string(0) + std::to_string(0),"","", name_text + std::to_string(0) + std::to_string(0));
			}
		}
		std::cout << " \n";

		tr.traverse();

		tr.deleteNode("Child id: 3");

		tr.traverse();

		pass_test(tr.get_size() == size_test - 1);
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

