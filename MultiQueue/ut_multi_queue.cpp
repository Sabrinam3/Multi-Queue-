/*!	\file	ut_multi_queue.cpp
	\author	Sabrina Tessier/
	\date	2018-11-11

	vector based multi_queue unit test.
*/
#define BOOST_TEST_MODULE MultiQueueUnitTests
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <list>
#include <map>
using namespace std;

#include <boost/mpl/list.hpp>
#include "multi_queue.hpp"


/*Brief tests that the value-type alias is being constructed with the same type as the queue*/
using value_type_test_types = boost::mpl::list<int, long, unsigned char, double, string>;
BOOST_AUTO_TEST_CASE_TEMPLATE(value_type_test, T, value_type_test_types)
{
	BOOST_CHECK_EQUAL(typeid(fixed_priority_multi_queue<T>::value_type).name(), typeid(T).name());
}

//=========================================
//SIZE TESTS
//=========================================

/*Brief: Basic size test- constructs an empty queue and checks the size is 0*/
BOOST_AUTO_TEST_CASE(basic_size_test)
{	
	for (auto i = 0; i < 100; ++i)
	{
		fixed_priority_multi_queue<int> queue;
		BOOST_CHECK_EQUAL(queue.size(), 0);
	}
}

/*
Brief- checks that when a queue is loaded with 40 elements, that the size returns with the correct result
*/
BOOST_AUTO_TEST_CASE(size_test)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 1; j <= 10; ++j)
		{
			queue.push(j, i);
		}
	}
	BOOST_CHECK_EQUAL(queue.size(), 40);
}

/*Brief- checks that when a large queue is constructed and loaded, the size returns the correct result*/
BOOST_AUTO_TEST_CASE(large_size_test)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 100; ++i)
		for (auto j = 1; j <= 100; ++j)
			queue.push(j, i);
	BOOST_CHECK_EQUAL(queue.size(), 10000);
}

/*Brief- constructs a const queue and then checks that size returns correctly*/
BOOST_AUTO_TEST_CASE(const_size_test)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 10; ++i)
		for (auto j = 1; j <= 10; ++j)
			queue.push(j, i);

	fixed_priority_multi_queue<int> const constQueue(queue);
	BOOST_CHECK_EQUAL(constQueue.size(), 100);
}

//========================================
//DEFAULT CONSTRUCTOR TEST
//========================================
/*Brief- default constructs a queue with many different types and ensures they are all being constructed correctly*/
using default_constructor_test_types = boost::mpl::list<int, long, long long, char, short, unsigned char, double, string>;
BOOST_AUTO_TEST_CASE_TEMPLATE(default_constructor_test, T, default_constructor_test_types)
{
	for (auto i = 0; i < 100; ++i)
	{
		fixed_priority_multi_queue<T> queue;
		BOOST_CHECK_EQUAL(queue.size(), 0);
	}	
}

//===================================
//EMPTY TESTS
//===================================
/*Brief constructs a queue and then checks that empty returns true*/
BOOST_AUTO_TEST_CASE(basic_empty_test)
{
	fixed_priority_multi_queue<int> queue;
	BOOST_CHECK(queue.empty());
}

/*Brief constructs a const queue and then checks that empty returns true*/
BOOST_AUTO_TEST_CASE(basic_empty_const_test)
{
	fixed_priority_multi_queue<int> const constQueue;
	BOOST_CHECK(constQueue.empty());
}

/*
Brief- tests the empty method by default constructing an empty queue of many different types and checking that empty returns true
*/
using empty_test_types = boost::mpl::list<int, long, long long, char, short, unsigned char, double, string>;
BOOST_AUTO_TEST_CASE_TEMPLATE(default_constructor_empty_test, T, empty_test_types)
{
	for (auto i = 0; i < 100; ++i)
	{
 		fixed_priority_multi_queue<T> queue;
		BOOST_CHECK(queue.empty());
	}
}

//==============================================
//PUSH(COPY) METHOD TESTS
//==============================================

/*
Brief- tests the copy push method by adding an element to the queue and then checking that the size is 1
*/
BOOST_AUTO_TEST_CASE(basic_push_test_L)
{
	fixed_priority_multi_queue<int> queue;
	queue.push(2, 0);
	BOOST_CHECK_EQUAL(queue.size(), 1);
}

/*Brief- push onto an already populated multi_queue that is large and make sure that the size is correct*/
BOOST_AUTO_TEST_CASE(push_onto_large_queue)
{
	fixed_priority_multi_queue<int>queue;
	for (auto i = 0; i < 1000; ++i)
	{
		for (auto k = 100; k < 200; ++k)
		{
			queue.push(i, k);
		}
	}
	queue.push(201, 1);
	BOOST_CHECK_EQUAL(queue.size(), 100001);
}

//==============================================
//TOP METHOD TESTS
//==============================================
/*
Brief- tests the top method by returning the top of the queue into a variable and then checking that the variable has been populated with the right value
*/
BOOST_AUTO_TEST_CASE(basic_top_test)
{
	fixed_priority_multi_queue<string> queue;
	string test = "";
	queue.push("test", 0);
	test = queue.top();
	BOOST_CHECK(!test.empty());
	BOOST_CHECK_EQUAL(test, "test");
}

/*Brief- checks that the top method returns the top priority item i.e. the first item added to the top priority queue*/
BOOST_AUTO_TEST_CASE(top_priority_test)
{
	fixed_priority_multi_queue<int> queue;
	int priority = 0;
	for (auto i = 1; i <= 10; ++i)
	{
		if (i >= 4 && i <= 6)
			priority = 1;
		if (i >= 7 && i <= 10)
			priority = 2;
		queue.push(i, priority);
	}

	for (auto i = 1; i <= 10; ++i)
	{
		BOOST_CHECK_EQUAL(queue.top(), i);
		queue.pop();
	}
}

/*
Brief- tests the const version of the top method by creating two queues(one a const copy of the other),
		  and then checking that the top method returns the same value for both
*/
BOOST_AUTO_TEST_CASE(const_top_test)
{
	fixed_priority_multi_queue<string> queue;
	queue.push("test", 0);
	fixed_priority_multi_queue<string> const constQueue(queue);
	BOOST_CHECK(queue.top() == constQueue.top());
}

//===========================================
//PUSH(MOVE) TESTS
//===========================================

/*
Brief- tests the move push method by adding an element to the queue and then checking that the top of the queue has the
		  original value of the string and that the string is now empty
*/
BOOST_AUTO_TEST_CASE(basic_push_test_R)
{
	fixed_priority_multi_queue<string>queue;
	string test = "test";
	queue.push(std::move(test), 0);
	BOOST_CHECK_EQUAL(queue.top(), "test");
	BOOST_CHECK(test.empty());
}

/*Brief- calls r-value push on an empty object and then checks to ensure the top of multi-queue is an empty object */
BOOST_AUTO_TEST_CASE(empty_push_test_r)
{
	fixed_priority_multi_queue<string>queue;
	string test = "test";
	queue.push(std::move(test), 0);
	BOOST_CHECK_EQUAL(queue.size(), 1);
	BOOST_CHECK(test.empty());
	queue.pop();
	BOOST_CHECK(queue.empty());
	queue.push(std::move(test), 0);
	BOOST_CHECK(queue.top().empty());
}

//=============================================
//ITERATOR CONSTRUCTOR TESTS
//=============================================
/*
Brief- tests the iterator version of the constructor by loading a map with value, priority pairs and then using that to create a multi_queue.
		  Then test that the size of the queue is the same as the map and that all values in the queue match the map
*/
BOOST_AUTO_TEST_CASE(iterator_constructor_test)
{
	using map = map<string, int>;
	map loadMap;
	loadMap["TestVal0"] = 0;
	loadMap["TestVal1"] = 1;
	loadMap["TestVal2"] = 2;
	loadMap["TestVal3"] = 3;
	loadMap["TestVal4"] = 4;
	fixed_priority_multi_queue<string> queue(loadMap.begin(), loadMap.end());
	BOOST_CHECK_EQUAL(queue.size(), 5);
	for (map::iterator it = loadMap.begin(); it != loadMap.end(); ++it)
	{
		BOOST_CHECK_EQUAL(it->first, queue.top());
		queue.pop();
	}
}

/*Brief- load a priority multi queue from a vector of pairs and check that the first element in is the first element out*/
BOOST_AUTO_TEST_CASE(vector_pairs_test)
{
	vector<pair<string, int>> loadVector;
	for (auto i = 0; i < 10; ++i)
		for (auto k = 1000; k <= 2000; ++k)
			loadVector.push_back(make_pair(to_string(k), i));
	fixed_priority_multi_queue<string> queue(loadVector.begin(), loadVector.end());
	BOOST_CHECK_EQUAL(queue.top(), loadVector.front().first);
}

/*Brief- load a priority queue from a list of pairs and check that the last element in the queue is the same as the last element in the list*/
BOOST_AUTO_TEST_CASE(list_pairs_test)
{
	using list = list<pair<string, int>>;
	list  loadList;
	for (auto i = 0; i < 10; ++i)
		for (auto k = 1000; k <= 2000; ++k)
			loadList.push_back(make_pair(to_string(k), i));
	fixed_priority_multi_queue<string>queue(loadList.begin(), loadList.end());
	//Test elements for equality
	BOOST_CHECK_EQUAL(queue.size(), loadList.size());
	for (list::const_iterator lit = loadList.begin(); lit != loadList.end(); ++lit)
	{
		BOOST_CHECK_EQUAL(queue.top(), lit->first);
		queue.pop();
	}
	BOOST_CHECK(queue.empty());
}

/*Brief- constructs a vector of pairs and makes sure that the types are the same before constructing the queue*/
using iterator_test_types = boost::mpl::list<int, long, unsigned char, double, string>;
BOOST_AUTO_TEST_CASE_TEMPLATE(iterator_constructor_test_types, T, iterator_test_types)
{
	using vect = vector<pair<T, int>>;
	vect loadVector;
	fixed_priority_multi_queue<T> queue;
	for(auto pair : loadVector)
		BOOST_CHECK_EQUAL(typeid(pair.first).name(), typeid(T).name());
}

//=======================================
//COPY CONSTRUCTOR TESTS
//======================================= 

/*Brief - Copy constructor test - check that sizes are the same*/
BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 4; ++i)
		for (auto j = 1; j <= 10; ++j)
			queue.push(j, i);
	fixed_priority_multi_queue<int> copyQueue(queue);
	BOOST_CHECK_EQUAL(queue.size(), copyQueue.size());
}

/*Brief- copy construct an empty queue and make sure both are empty*/
BOOST_AUTO_TEST_CASE(copy_construct_empty_queue)
{
	fixed_priority_multi_queue<string>queue;
	fixed_priority_multi_queue<string>copyQueue(queue);
	BOOST_CHECK(queue.empty());
	BOOST_CHECK(copyQueue.empty());
}

/*Brief- copy construct a very large queue and make sure both are the same size at the end*/
BOOST_AUTO_TEST_CASE(copy_construct_large_queue)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 1000; ++i)
		for (auto j = 100; j <= 200; ++j)
			queue.push(j, i);
	fixed_priority_multi_queue<int>copyQueue(queue);
	BOOST_CHECK_EQUAL(queue.size(), copyQueue.size());
	BOOST_CHECK(!copyQueue.empty());
	BOOST_CHECK_EQUAL(queue.top(), copyQueue.top());
}

//====================================
//MOVE CONSTRUCTOR TEST
//====================================

/*Brief- Move constructor test - original should be empty after*/
BOOST_AUTO_TEST_CASE(copy_move_constructor_test)
{
	fixed_priority_multi_queue<string> queue;
	queue.push(to_string(1), 0);
	queue.push(to_string(2), 0);
	queue.push(to_string(3), 0);
	fixed_priority_multi_queue<string> copyQueue(std::move(queue));
	BOOST_CHECK_EQUAL(queue.size(), 0);
	BOOST_CHECK_EQUAL(copyQueue.size(), 3);
}

/*Brief- move construct on an empty queue*/
BOOST_AUTO_TEST_CASE(move_construct_empty)
{
	fixed_priority_multi_queue<int> queue;
	fixed_priority_multi_queue<int> copyQueue(std::move(queue));
	BOOST_CHECK_EQUAL(queue.empty(), copyQueue.empty());
}

/*Brief- construct one const queue from another*/
BOOST_AUTO_TEST_CASE(const_constructor_tests)
{
	fixed_priority_multi_queue<int> queue;
	for (auto i = 0; i < 10; ++i)
		queue.push(i, 0);
	fixed_priority_multi_queue<int> constQueueOne(queue);
	fixed_priority_multi_queue<int> constQueueTwo(constQueueOne);
	BOOST_CHECK(!constQueueOne.empty());
	BOOST_CHECK(!constQueueTwo.empty());
	BOOST_CHECK_EQUAL(constQueueOne.size(), constQueueTwo.size());
}

//=====================================
//POP TESTS
//=====================================

/*
Brief- tests that the pop element can clear all elements from the queue
*/
BOOST_AUTO_TEST_CASE(pop_test_clear_all_elements)
{
	fixed_priority_multi_queue<string> queue;
	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 1; j <= 10; ++j)
		{
			queue.push(to_string(j), i);
		}
	}

	while (!queue.empty())
	{
		queue.pop();
	}
	BOOST_CHECK_EQUAL(queue.size(), 0);
}

/*Brief- checks that the pop method reduces the size of the queue by 1 each time*/
BOOST_AUTO_TEST_CASE(pop_should_reduce_size_by_one)
{
	fixed_priority_multi_queue<double> popQueue;
	for (auto i = 0; i < 10; ++i)
		for (auto j = 10; j <= 20; ++j)
			popQueue.push(j, i);
	int queueSize = popQueue.size();

	while (!popQueue.empty())
	{
		popQueue.pop();
		BOOST_CHECK_EQUAL(popQueue.size(), --queueSize);
	}
}

//===========================================
//MAX PRIORITY TESTS
//===========================================

/*
Brief- Check that the max_priority function successfully allocates one more priority than the value specified in the push method
*/
BOOST_AUTO_TEST_CASE(max_priority_test)
{
	fixed_priority_multi_queue<string> queue;
	queue.push("test", 0);
	BOOST_CHECK_EQUAL(queue.max_priority(), 1);
	queue.push("test", 2);
	BOOST_CHECK_EQUAL(queue.max_priority(), 3);
	BOOST_CHECK_EQUAL(queue.size(), 2);
}

/*Brief- check that push places element in the right spot according to priority*/
using priority_test_types = boost::mpl::list<int, long, unsigned int, double, unsigned char>;
BOOST_AUTO_TEST_CASE_TEMPLATE(push_priority_test, T, priority_test_types)
{
	fixed_priority_multi_queue<T> queue;
	for (auto i = 0; i < 5; ++i)
		for (auto j = 1; j <= 10; ++j)
			queue.push(j, i);
	BOOST_CHECK_EQUAL(queue.max_priority(), 5);
	queue.push(1, 5);
	BOOST_CHECK_EQUAL(queue.max_priority(), 6);
}

//=========================================
//COPY ASSIGNMENT TESTS
//=========================================
/*
Brief- tests the l-value copy assignment operation by checking that the two queues have the same size and the top of each queue is the same
*/
BOOST_AUTO_TEST_CASE(copy_assignment_L_test)
{
	fixed_priority_multi_queue<int>queue;
	for (auto i = 0; i < 4; ++i)
		for (auto j = 1; j <= 10; ++j)
			queue.push(j, i);

	fixed_priority_multi_queue<int>copyQueue;
	copyQueue = queue;
	BOOST_CHECK_EQUAL(queue.size(), copyQueue.size());
	BOOST_CHECK_EQUAL(queue.top(), copyQueue.top());
}

/*Brief - test the copy assignment by assigning a smaller queue to a previously larger queue*/
BOOST_AUTO_TEST_CASE(copy_assignment_override_smaller)
{
	fixed_priority_multi_queue<int> largerQueue;
	for (auto i = 0; i < 4; ++i)
		for (auto j = 1; j <= 10; ++j)
			largerQueue.push(j, i);
	fixed_priority_multi_queue<int> smallerQueue;
	for (auto i = 0; i < 3; ++i)
		for (auto j = 1; j <= 5; ++j)
			smallerQueue.push(j, i);
	largerQueue = smallerQueue;
	BOOST_CHECK_EQUAL(largerQueue.size(), 15);
	BOOST_CHECK_EQUAL(largerQueue.size(), smallerQueue.size());
	BOOST_CHECK_EQUAL(smallerQueue.max_priority(), largerQueue.max_priority());
}

/*Brief- test the copy assignment by assigning a larger queue to a previously smaller queue*/
BOOST_AUTO_TEST_CASE(copy_assignment_override_larger)
{
	fixed_priority_multi_queue<int> smallerQueue;
	for (auto i = 0; i < 5; ++i)
		for (auto j = 1; j <= 5; ++j)
			smallerQueue.push(j, i);
	fixed_priority_multi_queue<int> largerQueue;
	for (auto i = 0; i < 20; ++i)
		for (auto j = 1; j <= 100; ++j)
			largerQueue.push(j, i);
	smallerQueue = largerQueue;
	BOOST_CHECK_EQUAL(smallerQueue.size(), 2000);
	BOOST_CHECK(smallerQueue.size(), largerQueue.size());
	BOOST_CHECK_EQUAL(smallerQueue.max_priority(), largerQueue.max_priority());
}

/*
Brief- tests the r-value copy assignment operation by checking that the copied queue has the appropriate size and the original queue is empty
*/
BOOST_AUTO_TEST_CASE(copy_assignment_R_test)
{
	fixed_priority_multi_queue<int>queue;
	for (auto i = 0; i < 4; ++i)
		for (auto j = 1; j <= 10; ++j)
			queue.push(j, i);

	fixed_priority_multi_queue<int>copyQueue;
	copyQueue = std::move(queue);
	BOOST_CHECK_EQUAL(copyQueue.size(), 40);
	BOOST_CHECK(queue.empty());
}

/*Brief- checks the const version of the copy assignment operator*/
BOOST_AUTO_TEST_CASE(copy_assignment_const_test)
{
	fixed_priority_multi_queue<int> queue;
	queue.push(10, 0);
	queue.push(20, 0);
	queue.push(30, 0);

	fixed_priority_multi_queue<int> const constQueue(queue);
	fixed_priority_multi_queue<int> queueTwo = constQueue;
	BOOST_CHECK_EQUAL(constQueue.size(), queueTwo.size());
	BOOST_CHECK_EQUAL(queue.size(), queueTwo.size());
}

//===========================================
//SWAP TESTS
//===========================================

/*
Brief- tests the swap method by swapping two queues and then comparing each value to ensure the swap was successful
*/
BOOST_AUTO_TEST_CASE(swap_test)
{
	fixed_priority_multi_queue<string> oneQueue;
	fixed_priority_multi_queue<string> twoQueue;
	//Load oneQueue
	oneQueue.push("QueueOneVal1", 0);
	oneQueue.push("QueueOneVal2", 0);
	//Load twoQueue
	twoQueue.push("QueueTwoVal1", 0);
	twoQueue.push("QueueTwoVal2", 0);
	//Swap the queues
	swap(oneQueue, twoQueue);
	//Check to verify that the queues have been successfully swapped
	bool successful = true;
	successful = oneQueue.top() == "QueueTwoVal1";
	oneQueue.pop();
	successful = oneQueue.top() == "QueueTwoVal2";
	successful = twoQueue.top() == "QueueOneVal1";
	twoQueue.pop();
	successful = twoQueue.top() == "QueueOneVal2";
	BOOST_CHECK(successful);
}

/*Brief- swaps a larger queue with a smaller one and checks for accurate resizing and relocating of elements*/
BOOST_AUTO_TEST_CASE(swap_size_test)
{
	fixed_priority_multi_queue<double> smallerQueue;
	fixed_priority_multi_queue<double> largerQueue;
	for (auto i = 0; i < 10; ++i)
		for (auto j = 1; j <= 10; ++j)
			smallerQueue.push(j, i);

	for (auto i = 0; i < 20; ++i)
		for (auto j = 1; j <= 100; ++j)
			largerQueue.push(j, i);

	swap(smallerQueue, largerQueue);
	BOOST_CHECK_EQUAL(smallerQueue.size(), 2000);
	BOOST_CHECK_EQUAL(largerQueue.size(), 100);

	//Iterate through each of the queues, checking elements to ensure a successful swap
	while (!largerQueue.empty())
	{
		for (auto i = 1; i <= 10; ++i)
		{
			BOOST_CHECK_EQUAL(i, largerQueue.top());
			largerQueue.pop();
		}
			
	}

	while (!smallerQueue.empty())
	{
		for (auto i = 1; i <= 100; ++i)
		{
			BOOST_CHECK_EQUAL(i, smallerQueue.top());
			smallerQueue.pop();
		}
	}
}

//=============================================
//DESTRUCTOR TEST - check for memory leaks
//=============================================
//Destructor test - should be cleaned up automatically. No memory leaks
BOOST_AUTO_TEST_CASE(memory_leak_check)
{
	fixed_priority_multi_queue<string> stringQueue;
	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 1; j <= 10; ++j)
		{
			stringQueue.push(to_string(j), i);
		}
	}
	//Destructor should be called- no leaks
}






