#pragma once
/*!	\file fixed_priority_multi_queue.hpp
	\author Garth Santor
	\date 2017-08-31
	\version 1.0.2

	fixed_priority_multi_queue template class.
*/

#include <cstddef>
#include <queue>
#include <vector>


template <class ELEMENT_T>
class fixed_priority_multi_queue {

	// TYPES
public:
	using value_type = ELEMENT_T;
	using size_type = std::size_t;
	using reference = value_type & ;
	using const_reference = const value_type&;

	// ATTRIBUTES
private:
	std::vector<std::queue<ELEMENT_T>>	queues;

	// OPERATIONS
public:
	// constructors
	~fixed_priority_multi_queue() = default;
	fixed_priority_multi_queue() = default;
	fixed_priority_multi_queue(fixed_priority_multi_queue const& other) = default;
	fixed_priority_multi_queue(fixed_priority_multi_queue && other) noexcept : queues(move(other.queues)) {}
	template <class FORWARD>
	fixed_priority_multi_queue(FORWARD first, FORWARD last);

	// member operators
	fixed_priority_multi_queue& operator = (fixed_priority_multi_queue const& other);
	fixed_priority_multi_queue& operator = (fixed_priority_multi_queue && other) noexcept;

	// element access
	reference top() noexcept;
	const_reference top() const noexcept;

	// capacity
	bool empty() const noexcept { return size() == 0; }
	size_type size() const noexcept;
	size_type max_priority() const noexcept { return queues.size(); }

	// modifiers
	void push(value_type const& value, size_type priority);
	void push(value_type && value, size_type priority);
	void pop() noexcept;
	void swap(fixed_priority_multi_queue& other) noexcept;
};



// Helper functions
template <class ELEMENT_T>
inline void swap(fixed_priority_multi_queue<ELEMENT_T>& lhs, fixed_priority_multi_queue<ELEMENT_T>& rhs) noexcept {
	lhs.swap(rhs);
}

// =============================================================================================================
// IMPLEMENTATIONS
// =============================================================================================================


// fixed_priority_multi_queue<ELEMENT_T>::fixed_priority_multi_queue(FORWARD beg, FORWARD end)
template <class ELEMENT_T>
template <class FORWARD>
fixed_priority_multi_queue<ELEMENT_T>::fixed_priority_multi_queue(FORWARD beg, FORWARD end) {
	while (beg != end) {
		push(beg->first, beg->second);
		++beg;
	}
}



// fixed_priority_multi_queue<ELEMENT_T>::pop()
template <class ELEMENT_T>
void fixed_priority_multi_queue<ELEMENT_T>::pop() noexcept {
	auto qptr = queues.begin();
	while (qptr != queues.end() && qptr->empty())
		++qptr;
	return qptr->pop();
}



// L-value fixed_priority_multi_queue<ELEMENT_T>::push()
template <class ELEMENT_T>
void fixed_priority_multi_queue<ELEMENT_T>::push(value_type const& value, size_type priority) {
	if (priority >= queues.size())
		queues.resize(priority + 1);

	queues[priority].push(value);
}



// R-value fixed_priority_multi_queue<ELEMENT_T>::push()
template <class ELEMENT_T>
void fixed_priority_multi_queue<ELEMENT_T>::push(value_type && value, size_type priority) {
	if (priority >= queues.size())
		queues.resize(priority + 1);

	queues[priority].push(std::move(value));
}



// fixed_priority_multi_queue<ELEMENT_T>::size()
template <class ELEMENT_T>
typename fixed_priority_multi_queue<ELEMENT_T>::size_type fixed_priority_multi_queue<ELEMENT_T>::size() const noexcept {
	size_type nElements = 0;
	for (auto const& q : queues)
		nElements += q.size();

	return nElements;
}



// fixed_priority_multi_queue<ELEMENT_T>::top()
template <class ELEMENT_T>
typename fixed_priority_multi_queue<ELEMENT_T>::reference fixed_priority_multi_queue<ELEMENT_T>::top() noexcept {
	auto qptr = queues.begin();
	while (qptr != queues.end() && qptr->empty())
		++qptr;
	return qptr->front();
}



// fixed_priority_multi_queue<ELEMENT_T>::top()
template <class ELEMENT_T>
typename fixed_priority_multi_queue<ELEMENT_T>::const_reference fixed_priority_multi_queue<ELEMENT_T>::top() const noexcept {
	auto qptr = queues.begin();
	while (qptr != queues.end() && qptr->empty())
		++qptr;
	return qptr->front();
}



// fixed_priority_multi_queue::operator = (copy)
template <class Key>
fixed_priority_multi_queue<Key>& fixed_priority_multi_queue<Key>::operator = (fixed_priority_multi_queue<Key> const& other) {
	queues = other.queues;
	return *this;
}



// fixed_priority_multi_queue::operator = (move)
template <class Key>
fixed_priority_multi_queue<Key>& fixed_priority_multi_queue<Key>::operator = (fixed_priority_multi_queue<Key> && other) noexcept {
	queues = std::move(other.queues);
	return *this;
}


//Swap method implementation
template <class Key>
inline void fixed_priority_multi_queue<Key>::swap(fixed_priority_multi_queue& other) noexcept {
	std::swap(queues, other.queues);
}
